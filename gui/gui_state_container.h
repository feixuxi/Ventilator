#ifndef GUI_STATE_CONTAINER_H
#define GUI_STATE_CONTAINER_H

#include <QObject>
#include <stdint.h>

#include "chrono.h"
#include "controller_history.h"

#include <mutex>

#define RR_DEFAULT 0
#define PEEP_DEFAULT 0
#define PIP_DEFAULT 0
#define IER_DEFAULT 1.1

#define RR_ALARM_HIGH_DEFAULT 0
#define RR_ALARM_LOW_DEFAULT 0
#define TV_ALARM_HIGH_DEFAULT 0
#define TV_ALARM_LOW_DEFAULT 0


// A thread-safe container for readable and writable state
// of the GUI.
//
// The rest of the GUI must bind itself to accessors and mutators
// of this class - e.g. render graphs from GetControllerStatusHistory(),
// and when a parameter is changed in the UI, call a mutator on this
// object.
//
// In other words, this is essentially an MVC "Model".
//
// GUI rendering and callbacks happen asynchronously to interaction
// with the controller, so thread safety is important, and it is easiest
// to centralize it in the current class, simply protecting everything
// by a single mutex.
class GuiStateContainer : public QObject {
  Q_OBJECT;

public:
  // Initializes the state container to keep the history of controller
  // statuses in a given time window.
  GuiStateContainer(DurationMs history_window, QObject *parent = nullptr)
      : QObject(parent), startup_time_(SteadyClock::now()),
        history_(history_window) {}

  // Returns when the GUI started up.
  SteadyInstant GetStartupTime() { return startup_time_; }

  // Adds a data point of controller status to the history.
  void AppendHistory(const ControllerStatus &status) {
    std::unique_lock<std::mutex> l(mu_);
    history_.Append(SteadyClock::now(), status);
  }

  // Returns the current GuiStatus to be sent to the controller.
  GuiStatus GetGuiStatus() {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_;
  }

  // TODO: Add mutators of GuiStatus when there are any UI elements
  // that change parameters.

  // Returns the recent history of ControllerStatus.
  std::vector<std::tuple<SteadyInstant, ControllerStatus>>
  GetControllerStatusHistory() {
    std::unique_lock<std::mutex> l(mu_);
    return history_.GetHistory();
  }

  Q_PROPERTY(quint32 rr READ get_rr WRITE set_rr NOTIFY params_changed)
  Q_PROPERTY(quint32 peep READ get_peep WRITE set_peep NOTIFY params_changed)
  Q_PROPERTY(quint32 pip READ get_pip WRITE set_pip NOTIFY params_changed)
  Q_PROPERTY(qreal ier READ get_ier WRITE set_ier NOTIFY params_changed)

signals:
  void params_changed();

private:
  quint32 get_rr() const {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_.desired_params.breaths_per_min;
  }
  void set_rr(quint32 value) {
    {
      std::unique_lock<std::mutex> l(mu_);
      gui_status_.desired_params.breaths_per_min = value;
    }
    params_changed();
  }

  quint32 get_peep() const {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_.desired_params.peep_cm_h2o;
  }
  void set_peep(quint32 value) {
    {
      std::unique_lock<std::mutex> l(mu_);
      gui_status_.desired_params.peep_cm_h2o = value;
    }
    params_changed();
  }

  quint32 get_pip() const {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_.desired_params.pip_cm_h2o;
  }
  void set_pip(quint32 value) {
    {
      std::unique_lock<std::mutex> l(mu_);
      gui_status_.desired_params.pip_cm_h2o = value;
    }
    params_changed();
  }

  qreal get_ier() const {
    std::unique_lock<std::mutex> l(mu_);
    return gui_status_.desired_params.inspiratory_expiratory_ratio;
  }
  void set_ier(qreal value) {
    {
      std::unique_lock<std::mutex> l(mu_);
      gui_status_.desired_params.inspiratory_expiratory_ratio = value;
    }
    params_changed();
  }

  const SteadyInstant startup_time_;

  mutable std::mutex mu_;
  // TODO: Use thread safety annotations here and throughout the project.
  // https://clang.llvm.org/docs/ThreadSafetyAnalysis.html
  // They only work with clang, so we'll need a wrapper macro to have them
  // be no-ops on gcc.
  GuiStatus gui_status_ = GuiStatus_init_zero;
  ControllerHistory history_;
};

#endif // GUI_STATE_CONTAINER_H
