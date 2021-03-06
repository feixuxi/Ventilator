/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "controller.h"

#include <math.h>

static constexpr Duration LOOP_PERIOD = milliseconds(10);

static DebugFloat dbg_forced_blower_power(
    "forced_blower_power",
    "Force the blower fan to a particular power [0,1].  Specify a value "
    "outside this range to let the controller control it.",
    -1.f);
static DebugFloat dbg_forced_exhale_valve_pos(
    "forced_exhale_valve_pos",
    "Force the exhale valve to a particular position [0,1].  Specify a value "
    "outside this range to let the controller control it.",
    -1.f);
static DebugFloat dbg_forced_blower_valve_pos(
    "forced_blower_valve_pos",
    "Force the blower valve to a particular position [0,1].  Specify a value "
    "outside this range to let the controller control it.",
    -1.f);
static DebugFloat dbg_forced_psol_pos(
    "forced_psol_pos",
    "Force the O2 psol to a particular position [0,1].  (Note that psol.cpp "
    "scales this further; see psol_pwm_closed and psol_pwm_open.)  Specify a "
    "value outside this range to let the controller control the psol.",
    -1.f);

// Unchanging outputs - read from external debug program, never modified here.
static DebugUInt32 dbg_per("loop_period", "Loop period, read-only (usec)",
                           static_cast<uint32_t>(LOOP_PERIOD.microseconds()));

// Outputs - read from external debug program, modified here.
static DebugFloat dbg_sp("pc_setpoint", "Pressure control setpoint (cmH2O)");
static DebugFloat dbg_net_flow("net_flow", "Net flow rate, cc/sec");
static DebugFloat dbg_volume("volume", "Patient volume, ml");
static DebugFloat
    dbg_net_flow_uncorrected("net_flow_uncorrected",
                             "Net flow rate w/o correction, cc/sec");
static DebugFloat dbg_volume_uncorrected("uncorrected_volume",
                                         "Patient volume w/o correction, ml");
static DebugFloat dbg_flow_correction("flow_correction",
                                      "Correction to flow, cc/sec");

static DebugUInt32 dbg_breath_id("breath_id",
                                 "ID of the current breath, read-only", 0);

/*static*/ Duration Controller::GetLoopPeriod() { return LOOP_PERIOD; }

std::pair<ActuatorsState, ControllerState>
Controller::Run(Time now, const VentParams &params,
                const SensorReadings &sensor_readings) {
  VolumetricFlow uncorrected_net_flow =
      sensor_readings.inflow - sensor_readings.outflow;
  flow_integrator_->AddFlow(now, uncorrected_net_flow);
  uncorrected_flow_integrator_->AddFlow(now, uncorrected_net_flow);

  Volume patient_volume = flow_integrator_->GetVolume();
  VolumetricFlow net_flow =
      uncorrected_net_flow + flow_integrator_->FlowCorrection();

  BlowerSystemState desired_state = fsm_.DesiredState(
      now, params, {.patient_volume = patient_volume, .net_flow = net_flow});

  if (desired_state.is_end_of_breath) {
    // The "correct" volume at the breath boundary is 0.
    flow_integrator_->NoteExpectedVolume(ml(0));
    breath_id_ = now.microsSinceStartup();
  }

  // Precision loss 64->32 bits ok: we only care about equality of these values,
  // not their absolute value, and the top 32 bits will change with each new
  // breath.
  // TODO: Maybe we should just make breath id a 32-bit value.
  dbg_breath_id.Set(static_cast<uint32_t>(breath_id_));

  // Gain scheduling of blower Ki based on PIP and PEEP settings.  Artisanally
  // hand-tuned by Edwin.
  //
  // Note that we use desired_state.pip/peep and not params.pip/peep because
  // desired_state updates at breath boundaries, whereas params updates
  // whenever the user clicks the touchscreen.

  float blower_ki = 0;

  if (dbg_blower_valve_ki.Get() < 0) {
    blower_ki = std::clamp(
        (desired_state.pip - desired_state.peep).cmH2O() - 5.0f, 10.0f, 20.0f);

    dbg_blower_valve_computed_ki.Set(blower_ki);

  } else {
    blower_ki = dbg_blower_valve_ki.Get();
  }

  blower_valve_pid_.SetKP(dbg_blower_valve_kp.Get());
  blower_valve_pid_.SetKI(blower_ki);
  blower_valve_pid_.SetKD(dbg_blower_valve_kd.Get());
  psol_pid_.SetKP(dbg_psol_kp.Get());
  psol_pid_.SetKI(dbg_psol_ki.Get());
  psol_pid_.SetKD(dbg_psol_kd.Get());

  ActuatorsState actuators_state;
  if (desired_state.pressure_setpoint == std::nullopt) {
    // System disabled.  Disable blower, close inspiratory pinch valve, and
    // open expiratory pinch valve.  This way if someone is hooked up, they can
    // breathe through the expiratory branch, and they can't contaminate the
    // inspiratory branch.
    //
    // If the pinch valves are not yet homed, this will home them and then move
    // them to the desired positions.
    blower_valve_pid_.Reset();
    psol_pid_.Reset();

    actuators_state = {
        .fio2_valve = 0,
        .blower_power = 0,
        .blower_valve = 0,
        .exhale_valve = 1,
    };
    ventilator_was_on_ = false;
  } else {
    if (!ventilator_was_on_) {
      // reset volume integrators
      flow_integrator_.emplace();
      uncorrected_flow_integrator_.emplace();
    }

    // At the moment we don't support oxygen mixing -- we deliver either pure
    // air or pure oxygen.  For any fio2 < 1, deliver air.
    if (params.fio2 < 1) {
      // Delivering pure air.
      psol_pid_.Reset();

      // Calculate blower valve command using calculated gains
      float blower_valve =
          blower_valve_pid_.Compute(now, sensor_readings.patient_pressure.kPa(),
                                    desired_state.pressure_setpoint->kPa());

      actuators_state = {
          .fio2_valve = 0,
          // In normal mode, blower is always full power; pid controls pressure
          // by actuating the blower pinch valve.
          .blower_power = 1,
          .blower_valve = std::clamp(blower_valve + 0.05f, 0.0f, 1.0f),
          // coupled control: exhale valve tracks inhale valve command
          .exhale_valve = 1.0f - 0.55f * blower_valve - 0.4f,
      };
    } else {
      // Delivering pure oxygen.
      blower_valve_pid_.Reset();

      float psol_valve =
          psol_pid_.Compute(now, sensor_readings.patient_pressure.kPa(),
                            desired_state.pressure_setpoint->kPa());
      actuators_state = {
          // Force psol to stay very slightly open to avoid the discontinuity
          // caused by valve hysteresis at very low command.  The exhale valve
          // compensates for this intentional leakage by staying open when the
          // psol valve is closed.
          .fio2_valve = std::clamp(psol_valve + 0.05f, 0.0f, 1.0f),
          .blower_power = 0,
          .blower_valve = 0,
          .exhale_valve = 1.0f - 0.6f * psol_valve - 0.4f,
      };
    }

    // Start controlling pressure.
    ventilator_was_on_ = true;
  }

  ControllerState controller_state = {
      .pressure_setpoint = desired_state.pressure_setpoint.value_or(kPa(0)),
      .patient_volume = patient_volume,
      .net_flow = net_flow,
      .flow_correction = flow_integrator_->FlowCorrection(),
      .breath_id = breath_id_,
  };

  dbg_sp.Set(desired_state.pressure_setpoint.value_or(kPa(0)).cmH2O());
  dbg_net_flow.Set(controller_state.net_flow.ml_per_sec());
  dbg_net_flow_uncorrected.Set(
      (sensor_readings.inflow - sensor_readings.outflow).ml_per_sec());
  dbg_volume.Set(controller_state.patient_volume.ml());
  dbg_volume_uncorrected.Set(uncorrected_flow_integrator_->GetVolume().ml());
  dbg_flow_correction.Set(flow_integrator_->FlowCorrection().ml_per_sec());

  // Handle DebugVars that force the actuators.
  auto set_force = [](const DebugFloat &var, auto &state) {
    float v = var.Get();
    if (v >= 0 && v <= 1) {
      state = v;
    }
  };
  set_force(dbg_forced_blower_power, actuators_state.blower_power);
  set_force(dbg_forced_blower_valve_pos, actuators_state.blower_valve);
  set_force(dbg_forced_exhale_valve_pos, actuators_state.exhale_valve);
  set_force(dbg_forced_psol_pos, actuators_state.fio2_valve);

  return {actuators_state, controller_state};
}
