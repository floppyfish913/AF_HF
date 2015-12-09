import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Item {
    id: item1
    width: 1000
    height: 700
    anchors.fill: parent

    signal powerSwitch; // a kezelőfelületen megnyomható gombokhoz tartozó jelek
    signal set_PID;
    signal refresh_PID;

    // kezdésként külön ablakban meg kell adni az eszköz IP-címét és a csatlakoztatni kívánt port számát
    Dialog {
        id: connectionDialog
        width: 300
        height: 150
        visible: false
        modality: Qt.ApplicationModal
        title: "Connecting"
        onAccepted: nop
        ColumnLayout {
            id: connectColumn
            spacing: 15
            RowLayout {
                id: connectRow1
                Text { text: "IP Address: "}
                TextField {
                    validator: RegExpValidator {
                        regExp: /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/gm
                        }
                    style: TextFieldStyle {
                       background: Rectangle {
                             implicitWidth: 100
                             implicitHeight: 25
                             color: "white"
                             border.color: "black"
                             }
                    }
                }
            }
            RowLayout {
                id: connectRow2
                width: connectRow1.width
                Text { text: "Port: "}
                TextField {
                    anchors.right: parent.right
                    validator: IntValidator {
                        bottom: 0
                        top: 65535
                        }
                    style: TextFieldStyle {
                       background: Rectangle {
                             implicitWidth: 100
                             implicitHeight: 25
                             color: "white"
                             border.color: "black"
                             }
                    }
                }
            }
        }
    }

    // Tápfesz kikapcsolásakor felugró ablak
    Dialog {
        id: powerOffDialog
        width: 400
        height: 100
        visible: false
        title: "Power Off Dialog"
        Text {
            text: "Are you sure you want to turn off the power?"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        standardButtons: StandardButton.Yes | StandardButton.No | StandardButton.Cancel
        onYes: container.toggle()
    }

    // A dőlésszögmérő ábrázolása
    GroupBox {
        id: angoffset
        width: 600
        height: 600
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.top: parent.top
        anchors.left: parent.left
        Image {
            id: raise
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "/pics/raise.png"
            anchors.fill: parent
            transform: Rotation {
                origin.x: raise.width/2
                origin.y: raise.height/2
                angle: kalmanAngle
            }
        }
        Image {
            id: rot
            anchors.left: parent.left
            anchors.top: parent.top
            source: "/pics/rot.png"
            anchors.fill: parent
            transform:  Rotation { angle: kalmanAngle; origin.x: rot.width/2; origin.y: rot.height/2  }
                        Translate { y: kalmanOffset }
        }
        Image {
            id: frame
            anchors.left: parent.left
            anchors.top: parent.top
            source: "/pics/frame.png"
            anchors.fill: parent
        }
    }

    // A bekapcsoló gomb és a tápfeszültség kiírása
    GroupBox {
        id: power
        height: 80
        anchors.left: pidParameters.right
        anchors.leftMargin: 150
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.topMargin: 0

        // Oszlopba rendezés
        ColumnLayout {
            id: columnLayout1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0          
            // A bekapcsoló gomb Toggle Button, vagyis benyomva marad bekapcsolás után.
            // Az állapotától függ a felirata, a színe és a klikkelésre történő változás is.
            Rectangle {
                height: 25
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 24
                anchors.rightMargin: 24                               
                Rectangle {
                    objectName: "PW_button"
                    id: container
                    anchors.fill: parent
                    border.color: "black"
                    border.width: 2
                    radius: 5
                    function toggle() {
                        if (state == "on") { 
                            state = "off"
                            powerSwitch()
                        }
                        else {
                            state = "on"
                            powerSwitch()
                        }
                    }
                    state: "off"
                    states: [
                        State {
                            name: "on";
                            PropertyChanges { target: container; color: "green";}
                            PropertyChanges { target: btnText; text: "Power On";}
                        },
                        State {
                            name: "off";
                            PropertyChanges { target: container; color: "red";}
                            PropertyChanges { target: btnText; text: "Power Off";}
                        }
                    ]
                    Text { id: btnText; text: "Power Off"; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter; anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; }
                    MouseArea { id: region; anchors.fill: parent; onReleased: (container.state == "off") ? container.toggle() : powerOffDialog.open() }
                }
            }
            // A tápfesz kiírása
            Text {
                id: battery
                width: 160
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 15                
                anchors.bottomMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Battery: " + batteryState.toString() + " V "
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pointSize: 11
            }
        }
    }

    // A PID parancsai
    GroupBox {
        id: pidCommands
        anchors.left: angoffset.right
        width: 200
        height: 50
        anchors.top: pidParameters.bottom
        anchors.leftMargin: 10
        anchors.topMargin: 0

        // Sorba rendezés
        RowLayout {
            id: rowLayout1
            anchors.fill: parent
            spacing: 20 // A gombok között legyen hely kihagyva
            // A Set gombbal beállítható a beírt érték, a Refresh-sel pedig a visszaállítást akadályozzuk meg
            Button {
                id: setPID
                anchors.left: parent.left                
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Set PID")
                onClicked: set_PID()
            }
            Button {
                id: refreshPID
                anchors.right: parent.right                
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Refresh PID")
                onClicked: refresh_PID()
            }
        }
    }

    // PID paraméterek beírására szolgáló felület
    GroupBox {
        id: pidParameters
        anchors.left: angoffset.right
        anchors.top : parent.top
        anchors.leftMargin: 10
        anchors.topMargin: 0
        width: 200
        height: 200

        // Oszlopba rendezés
        ColumnLayout {
            id: columnLayout2
            anchors.fill: parent
            spacing: 10
            // Az oszlopon belül a P, I és D paramétereket jelző betűk
            // és a beírásra szolgáló mezők egymás mellé helyezése.            
            RowLayout {
                id: rowLayout2
                width: 150
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 10
                Text {
                    width: 50
                    height: 50
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    text: " P: "                         
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                        id: textInput1
                        objectName: "textInputP"
                        width: 100
                        height: 50
                        anchors.left: parent.left
                        anchors.leftMargin: 60
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: 18
                        validator: IntValidator {
                                        bottom: 0
                                        top: 50
                                   } // az érték egész szám 0-50 között
                        style: TextFieldStyle {
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 50
                                color: "white"
                                border.color: "black"
                            }
                        }
                }
            }
            RowLayout {
                id: rowLayout3
                width: 150
                height: 50
                anchors.top: rowLayout2.bottom
                anchors.topMargin: 10
                Text {
                    width: 50
                    height: 50
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    text: " I: " 
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                    id: textInput2
                    objectName: "textInputI"
                    width: 100
                    height: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 60
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    font.pointSize: 18
                    validator: IntValidator {
                                    bottom: 0
                                    top: 50
                               } // az érték egész szám 0-50 között
                    style: TextFieldStyle {
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 50
                            color: "white"
                            border.color: "black"
                        }
                    }
                }
            }
            RowLayout {
                id: rowLayout4
                width: 150
                height: 50
                anchors.top: rowLayout3.bottom
                anchors.topMargin: 10
                Text {
                    width: 50
                    height: 50
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    text: " D: "                  
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                    id: textInput3
                    objectName: "textInputD"
                    width: 100
                    height: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 60
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    font.pointSize: 18   
                    validator: IntValidator {
                                    bottom: 0
                                    top: 50
                               } // az érték egész szám 0-50 között
                    style: TextFieldStyle {
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 50
                            color: "white"
                            border.color: "black"
                        }
                    }
                }
            }
       }
   }

    // KalmanX és KalmanY adatok kijelzése
    GroupBox {
        id: gyr
        width: angoffset.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.top: angoffset.bottom
        anchors.topMargin: 0

    // Sorba rendezés
        RowLayout {
            id: rowLayout5
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 2         
            spacing: 70
            Text {
                width: 100
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 30
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: " X: " + kalmanXState
                font.bold: true
                font.pointSize: 12
            }
            Text {
                width: 100
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 30
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: " Y: " + kalmanYState
                font.bold: true
                font.pointSize: 12
            }
        }
    }

    // Magasság és motorteljesítmény paraméterek kijelzése
    GroupBox {
        id: heightMotorParameters
        anchors.left: pidParameters.right
        anchors.leftMargin: 150
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: power.bottom
        anchors.topMargin: 0
        ColumnLayout {
            id: columnLayout3
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 30
            Text {
                text: " Height: " + heightState + " m"
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 1 Performance: " + (motor1State) + " %"
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 2 Performance: " + motor2State + " %"
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 3 Performance: " + motor3State + " %"
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 4 Performance: " + motor4State + " %"
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
        }
    }

    // Grafikonok megjelenítése
    GroupBox {
        id: graphic
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: angoffset.right
        anchors.right: parent.right
        anchors.top: heightMotorParameters.bottom
        anchors.topMargin: 0

            ColumnLayout {
                id: graphsMainColumn
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.fill: parent
                RowLayout {
                    id: graphsUpperRow
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    spacing: 20
                    ColumnLayout {
                        id: graphColumn1
                        HistoryGraph {
                            id: historyGraphAngle
                            width: 315
                            objectName: "historyGraphAngle"

                            Layout.minimumHeight: 150
                            Layout.fillHeight: true
                            Layout.minimumWidth: 200
                            Layout.fillWidth: true

                            graphA: AngleXGraphs
                            graphB: AngleYGraphs
                        }
                        Text{ text: "Angle" ; anchors.horizontalCenter: parent.horizontalCenter; font.bold: true; font.pointSize: 10}
                    }
                    ColumnLayout {
                        id: graphColumn2
                        HistoryGraph {
                            id: historyGraphGyr
                            width: 315
                            objectName: "historyGraphGyr"

                            Layout.minimumHeight: 150
                            Layout.fillHeight: true
                            Layout.minimumWidth: 200
                            Layout.fillWidth: true

                            graphA: GyroXGraphs
                            graphB: GyroYGraphs
                            graphC: GyroZGraphs
                        }
                        Text{ text: "Gyroscope" ; font.bold: true; font.pointSize: 10; anchors.horizontalCenter: parent.horizontalCenter}
                    }
                }
                ColumnLayout {
                    id: graphsLower
                    anchors.right: parent.right
                    anchors.rightMargin: 200
                    anchors.left: parent.left
                    anchors.leftMargin: 200
                    HistoryGraph {
                        id: historyGraphAcc
                        width: 315
                        objectName: "historyGraphAcc"

                        Layout.minimumHeight: 150
                        Layout.fillHeight: true
                        Layout.minimumWidth: 200
                        Layout.fillWidth: true

                        graphA: AccXGraphs
                        graphB: AccYGraphs
                    }
                        Text{ text: "Acceleration" ; font.bold: true; font.pointSize: 10;anchors.horizontalCenter: parent.horizontalCenter}
                }
            }
    }
}
