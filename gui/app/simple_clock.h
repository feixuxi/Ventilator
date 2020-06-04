#ifndef CLOCK_H
#define CLOCK_H

#include <QObject>
#include <QTimer>

/**
 * @brief Clock class that emit current datetime in a
 * given interval
 */
class SimpleClock : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      bool enabled READ GetEnabled WRITE SetEnabled NOTIFY enabledChanged)
  Q_PROPERTY(int updateInterval READ GetUpdateInterval WRITE SetUpdateInterval
                 NOTIFY updateIntervalChanged)
  Q_PROPERTY(int hour READ GetHour NOTIFY hourChanged)
  Q_PROPERTY(int minute READ GetMinute NOTIFY minuteChanged)
  Q_PROPERTY(int second READ GetSecond NOTIFY secondChanged)

public:
  explicit SimpleClock(QObject *parent = nullptr);
  virtual ~SimpleClock();

  bool GetEnabled() const;
  int GetUpdateInterval() const;
  int GetHour() const;
  int GetMinute() const;
  int GetSecond() const;

public slots:
  void SetUpdateInterval(int update_interval);
  void SetEnabled(bool enabled);

signals:

  void enabledChanged();
  void updateIntervalChanged();
  void hourChanged();
  void minuteChanged();
  void secondChanged();
  void updated();

private:
  bool enabled_;
  int update_interval_;
  int hour_;
  int minute_;
  int second_;
  QTimer timer_;

  void update_timer();
};

#endif // CLOCK_H
