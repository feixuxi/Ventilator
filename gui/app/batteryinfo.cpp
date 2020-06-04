#include "batteryinfo.h"

BatteryInfo::BatteryInfo(QObject *parent)
    : QObject(parent), current_percentage_(70) {}

int BatteryInfo::GetPercentage() const { return current_percentage_; }
