/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Palette       1.0

ColumnLayout {
    spacing: _rowSpacing

    function saveSettings() {
        // No need
    }

    QGCLabel {
        Layout.preferredWidth: _secondColumnWidth
        Layout.fillWidth:       true
        font.pointSize:         ScreenTools.smallFontPointSize
        wrapMode:               Text.WordWrap
        text:                   qsTr("Note: For best perfomance, please disable AutoConnect to UDP devices on the General page.")
    }

    QGCLabel { text: qsTr("AirLink authentication") }

    Repeater {
        model: subEditConfig.hostList

        delegate: RowLayout {
            spacing: _colSpacing

            QGCLabel {
                Layout.preferredWidth:  _secondColumnWidth
                text:                   modelData
            }

            QGCButton {
                text:       qsTr("Remove")
                onClicked:  subEditConfig.removeHost(modelData)
            }
        }
    }

    RowLayout {
        spacing: _colSpacing

        QGCTextField {
            id:                     loginField
            Layout.preferredWidth:  _secondColumnWidth
            placeholderText:        qsTr("Login")
        }
        QGCTextField {
            id:                     passField
            Layout.preferredWidth:  _secondColumnWidth
            placeholderText:        qsTr("Password")
        }

        QGCButton {
            text:       qsTr("Sign in")
            onClicked: {
                subEditConfig.addHost("air-link.space",10000)
                subEditConfig.authorize(loginField.text,passField.text)
            }
        }
    }
}
