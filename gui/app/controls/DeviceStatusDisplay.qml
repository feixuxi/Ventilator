import QtQuick 2.0

import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

/*!
    \qmltype DeviceStatusDisplay
    \brief Display device status information such as battery and clock
*/
Row {
    width: 166; height: 24
    spacing: 4

    Row {
        id: clock
        width: 70; height: 22
        spacing: 4

        Text {
            id: time
            text: format(SimpleClock.hour, SimpleClock.minute)
            font: Style.clockFont
            color: Style.theme.color.textPrimary

            function format(hour, minute) {
                var twelveHour = hour > 12 ? hour -12 : hour
                var hStr = twelveHour.toString()
                var mStr = minute.toString()
                if (hour < 10) {
                    hStr = "0" + hour
                }

                if (minute < 10) {
                    mStr = "0" + minute
                }
                return "%1:%2".arg(hStr).arg(mStr)
            }
        }

        Text {
            id: period
            font: Style.clockFont
            color: Style.theme.color.textAlternative
            text: SimpleClock.hour >= 12 ? "PM" : "AM"
        }
    }

    Text {
        width: 8; height: 22
        font: Style.clockFont
        color: Style.theme.color.textPrimary
        text: "•"
    }

    Row {
        id: battery
        spacing: 4

        Text {
            text: BatteryInfo.percentage.toString()
            font: Style.clockFont
            color: Style.theme.color.textPrimary
        }

        Text {
            text: "%"
            font: Style.clockFont
            color: Style.theme.color.textAlternative
        }

        Item { width: 2; height: 1 }

        Image {
            width: 20; height: 14
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectCrop
            source: 'qrc:/images/Logo.png'
        }
    }
}
