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

module contributors: verityRF, jlebar, lee-matthews, Edwin Chiu

The purpose of this module is to allow calibrated readings from the different
pressure sensors in the ventilator design. It is designed to be used with the
Arduino Nano and the MPXV5004GP and MPXV7002DP pressure sensors.
*/

#ifndef SENSORS_H
#define SENSORS_H

#include "hal.h"
#include "units.h"

struct SensorReadings {
  Pressure patient_pressure;
  // Pressure differences read at the inflow/outflow venturis.
  Pressure inflow_pressure_diff;
  Pressure outflow_pressure_diff;

  // fraction of inspired oxygen (fiO2)
  float fio2;

  // Inflow and outflow at the two venturis, calculated from inflow/outflow
  // pressure diff.
  //
  // These are "uncorrected" values.  We account for high-frequency noise by
  // e.g. averaging many samples, but we don't account here for low-frequency
  // sensor zero-point drift.
  VolumetricFlow inflow;
  VolumetricFlow outflow;
};

// Provides calibrated sensor readings, including tidal volume (TV)
// integrated from flow.
class Sensors {
public:
  Sensors();

  // Perform some initial sensor calibration.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void Calibrate();

  // Read the sensors.
  SensorReadings GetReadings() const;

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  constexpr static Pressure kMinPressure{kPa(0.0f)};
  constexpr static Pressure kMaxPressure{kPa(3.92f)};

  /*
   * @brief Method implements Bernoulli's equation assuming the Venturi Effect.
   * https://en.wikipedia.org/wiki/Venturi_effect
   *
   * Solves for the volumetric flow rate since A1/A2, rho, and differential
   * pressure are known. Q = sqrt(2/rho) * (A1*A2) * 1/sqrt(A1^2-A2^2) *
   * sqrt(p1-p2); based on (p1 - p2) = (rho/2) * (v2^2 - v1^2); where A1 > A2
   *
   * @return the volumetric flow in [meters^3/s]. Can be negative, indicating
   * direction of flow, depending on how the differential sensor is attached to
   * the venturi.
   */
  static VolumetricFlow PressureDeltaToFlow(Pressure delta);

private:
  enum class Sensor {
    kPatientPressure,
    kInflowPressureDiff,
    kOutflowPressureDiff,
    kFIO2,
  };
  // Keep this in sync with the Sensor enum!
  constexpr static int kNumSensors{4};

  static AnalogPin PinFor(Sensor s);
  Pressure ReadPressureSensor(Sensor s) const;
  float ReadOxygenSensor(Pressure p_ambient) const;

  // Calibrated average sensor values in a zero state.
  Voltage sensors_zero_vals_[kNumSensors];
};

#endif // SENSORS_H
