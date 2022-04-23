/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtGraphicalEffects       1.0
import QtMultimedia             5.5
import QtQuick                  2.3
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
import QtQuick.Layouts          1.2
import QtLocation               5.3
import QtPositioning            5.3

import QGroundControl                       1.0
import QGroundControl.Airspace              1.0
import QGroundControl.Controllers           1.0
import QGroundControl.Controls              1.0
import QGroundControl.FactControls          1.0
import QGroundControl.FactSystem            1.0
import QGroundControl.FlightMap             1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.SettingsManager       1.0

Item {
    id:                 _root
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    property real _labelWidth:                  ScreenTools.defaultFontPixelWidth * 20
    property real _editFieldWidth:              ScreenTools.defaultFontPixelWidth * 20
    property real _buttonWidth:                 ScreenTools.defaultFontPixelWidth * 18
    property real _panelWidth:                  _root.width * _internalWidthRatio
    property var  _authStatus:                  QGroundControl.airlinkManager.authStatus

    readonly property real _internalWidthRatio:          0.8

    QGCFlickable {
        clip:               true
        anchors.fill:       parent
        contentHeight:      settingsColumn.height
        contentWidth:       settingsColumn.width
        Column {
            id:                 settingsColumn
            width:              _root.width
            spacing:            ScreenTools.defaultFontPixelHeight * 0.5
            anchors.margins:    ScreenTools.defaultFontPixelWidth
            //-----------------------------------------------------------------
            //-- General

            //-----------------------------------------------------------------
            //-- Connection Status
            Item {
                width:                      _panelWidth
                height:                     statusLabel.height
                anchors.margins:            ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter:   parent.horizontalCenter
                visible:                    true
                QGCLabel {
                    id:                     statusLabel
                    text:                   qsTr("Connection Status")
                    font.family:            ScreenTools.demiboldFontFamily
                }
            }
            Rectangle {
                height:                     statusCol.height + (ScreenTools.defaultFontPixelHeight * 2)
                width:                      _panelWidth
                color:                      qgcPal.windowShade
                visible:                    true
                anchors.margins:            ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter:   parent.horizontalCenter
                Column {
                    id:                     statusCol
                    spacing:                ScreenTools.defaultFontPixelHeight * 0.5
                    width:                  parent.width
                    anchors.centerIn:       parent
                    QGCLabel {
                        text:                       QGroundControl.airlinkManager.isconnect ? qsTr("Connected") : qsTr("Not Connected")
                        color:                      QGroundControl.airlinkManager.isconnect ? qgcPal.colorGreen : qgcPal.colorRed
                        anchors.horizontalCenter:   parent.horizontalCenter
                    }
                }
            }
            //-----------------------------------------------------------------
            //-- Login / Registration
            Item {
                width:                      _panelWidth
                height:                     loginLabel.height
                anchors.margins:            ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter:   parent.horizontalCenter
                QGCLabel {
                    id:             loginLabel
                    text:           qsTr("Login / Registration")
                    font.family:    ScreenTools.demiboldFontFamily
                }
            }
            Rectangle {
                height:                     loginGrid.height + (ScreenTools.defaultFontPixelHeight * 2)
                width:                      _panelWidth
                color:                      qgcPal.windowShade
                anchors.margins:            ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter:   parent.horizontalCenter
                GridLayout {
                    id:                 loginGrid
                    columns:            3
                    columnSpacing:      ScreenTools.defaultFontPixelWidth
                    rowSpacing:         ScreenTools.defaultFontPixelHeight * 0.25
                    anchors.centerIn:   parent
                    QGCLabel        { text: qsTr("User Name:") }
                    FactTextField {
                        id:             _userText
                        fact:           _usernameFact
                        width:          _editFieldWidth
                        visible:        _usernameFact.visible
                        Layout.fillWidth:    true
                        Layout.minimumWidth: _editFieldWidth
                        property Fact _usernameFact: QGroundControl.settingsManager.airLinkSettings.userName
                    }
                    QGCLabel {
                        text: {
                            if(!QGroundControl.airspaceManager.connected)
                                return qsTr("Not Connected")
                            switch(_authStatus) {
                            case AirspaceManager.Authenticated:
                                return qsTr("Authenticated")
                            default:
                                return qsTr("Authentication Error")
                            }
                        }
                        Layout.rowSpan:     2
                        Layout.alignment:   Qt.AlignVCenter
                    }
                    QGCLabel { text: qsTr("Password:") }
                    FactTextField {
                        id:             _passText
                        fact:           _passwordFact
                        width:          _editFieldWidth
                        visible:        _passwordFact.visible
                        echoMode:       TextInput.Password
                        Layout.fillWidth:    true
                        Layout.minimumWidth: _editFieldWidth
                        property Fact _passwordFact: QGroundControl.settingsManager.airLinkSettings.password
                    }

                    Item {
                        width:  1
                        height: 1
                    }
                    Item {
                        width:  1
                        height: 1
                        Layout.columnSpan: 3
                    }
                    QGCLabel {
                        text:               qsTr("Forgot Your AirLink Password?")
                        Layout.alignment:   Qt.AlignHCenter
                        Layout.columnSpan:  3
                    }
                    Item {
                        width:  1
                        height: 1
                        Layout.columnSpan:  3
                    }

                    QGCButton {
                        text:               qsTr("Connect")
                        Layout.alignment:   Qt.AlignHCenter
                        Layout.columnSpan:  3
                        onClicked: {
                            QGroundControl.airlinkManager.connect(_userText.text,_passText.text)
                        }
                    }

                    QGCButton {
                        text:               qsTr("Disconnect")
                        Layout.alignment:   Qt.AlignHCenter
                        Layout.columnSpan:  3
                        onClicked: {
                            QGroundControl.airlinkManager.disconnect()
                        }
                    }

                    QGCButton {
                        text:               qsTr("Register for an AirLink Account")
                        Layout.alignment:   Qt.AlignHCenter
                        Layout.columnSpan:  3
                        onClicked: {
                            Qt.openUrlExternally("https://air-link.space/registration");
                        }

                    }
                }
            }

        }
    }
}

