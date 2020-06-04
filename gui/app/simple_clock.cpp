#include "simple_clock.h"
#include <QTime>

SimpleClock::SimpleClock(QObject *parent)
    : QObject(parent), enabled_(true), update_interval_(1000), hour_(0),
      minute_(0), second_(0), timer_(this) {

  connect(&timer_, &QTimer::timeout, this, &SimpleClock::update_timer);
  timer_.start();
}

SimpleClock::~SimpleClock() { timer_.stop(); }

bool SimpleClock::GetEnabled() const { return enabled_; }

int SimpleClock::GetUpdateInterval() const { return update_interval_; }

int SimpleClock::GetHour() const { return hour_; }

int SimpleClock::GetMinute() const { return minute_; }

int SimpleClock::GetSecond() const { return second_; }

void SimpleClock::SetUpdateInterval(int update_interval) {
  if (update_interval_ != update_interval) {
    update_interval_ = update_interval;
    emit updateIntervalChanged();
    update_timer();
  }
}

void SimpleClock::SetEnabled(bool enabled) {
  if (enabled_ != enabled) {
    enabled_ = enabled;
    emit enabledChanged();
    update_timer();
  }
}

void SimpleClock::update_timer() {
  auto time = QTime::currentTime();

  hour_ = time.hour();
  emit hourChanged();

  minute_ = time.minute();
  emit minuteChanged();

  second_ = time.second();
  emit secondChanged();

  timer_.setInterval(update_interval_);

  if (enabled_ && !timer_.isActive())
    timer_.start();

  if (!enabled_ && timer_.isActive())
    timer_.stop();

  emit updated();
}
