#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include <QObject>

/**
 * @brief The BatteryInfo class
 *
 * Keeps remaining percetange of battery time.
 */
class BatteryInfo : public QObject {
  Q_OBJECT
  Q_PROPERTY(int percentage READ GetPercentage NOTIFY percentageChanged)
public:
  explicit BatteryInfo(QObject *parent = nullptr);

  int GetPercentage() const;
signals:
  void percentageChanged();

private:
  int current_percentage_;
};

#endif // BATTERYINFO_H
