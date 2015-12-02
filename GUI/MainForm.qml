import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {
    id: item1
    width: 1000
    height: 700
    anchors.fill: parent

    // Signalok, melyek a kiadott parancsokat jelzik és a nyomógombok
    //  eseménykezelői aktiválják őket.
    signal powerSwitch;
    signal setPID;
    signal refreshPID;

    // A dőlésszögmérő ábrázolása
    GroupBox {
        id: angoffset
        width: 400
        height: 400
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.top: parent.top
        anchors.left: parent.left
    }

    // A bekapcsoló gomb és a tápfeszültség kiírása
    GroupBox {
        id: power
        anchors.right: parent.right
        anchors.top: parent.top
        height: 80
        anchors.left: pidParameters.right
        anchors.leftMargin: 200
        anchors.rightMargin: 0
        anchors.topMargin: 0

        // Oszlopba rendezés
        ColumnLayout {
            id: columnLayout1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            // A bekapcsoló gomb Toggle Button, vagyis benyomva marad bekapcsolás után.
            // Az állapotától függ a felirata, a színe és a klikkelésre történő változás is.
            Rectangle {
                height: 25
                anchors.right: parent.right
                anchors.rightMargin: 24
                anchors.left: parent.left
                anchors.leftMargin: 24
                Rectangle {
                    id: container
                    anchors.fill: parent
                    border.color: "black"
                    border.width: 2
                    radius: 5
                    function toggle() {
                        if (state=="on") {
                            state = "off"
                            powerSwitch()
                        }
                        else {
                            state ="on"
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
                    Text{ id: btnText; text: "Power Off"; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter; anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; }
                    MouseArea { id: region; anchors.fill: parent; onClicked: container.toggle() }
                }
            }
            // A tápfesz kiírása
            Text {
                id: battery
                width: 160
                anchors.leftMargin: 15
                text: "Tápfeszültség: " + ((container.color == "green") ? "15.1" : "0.0" ) + " V "
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
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
        anchors.topMargin: 0
        anchors.leftMargin: 0

        // Sorba rendezés
        RowLayout {
            id: rowLayout1
            anchors.fill: parent
            spacing: 20 // A gombok között legyen hely kihagyva
            // A Set gombbal beállítható a beírt érték, a Refresh-sel pedig a visszaállítást akadályozzuk meg
            Button {
                id: setPID
                anchors.left: parent.left
                text: qsTr("Set PID")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                onClicked: setPID()
            }
            Button {
                id: refreshPID
                anchors.right: parent.right
                text: qsTr("Refresh PID")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                onClicked: refreshPID()
            }
        }
    }

    // PID paraméterek beírása
    GroupBox {
        id: pidParameters
        anchors.top : parent.top
        width: 200
        height: 200
        anchors.left: angoffset.right
        anchors.leftMargin: 0
        anchors.topMargin: 0

        // Oszlopba rendezés
        ColumnLayout {
            id: columnLayout2
            anchors.fill: parent
            // Az oszlopon belül a P, I és D paramétereket jelző betűk
            // és a beírásra szolgáló mezők egymás mellé helyezése.
            spacing: 10
            RowLayout {
                id: rowLayout2
                width: 150
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 10
                Text {
                    width: 50
                    height: 50
                    text: " P: "
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                        id: textInput1
                        width: 100
                        height: 50
                        font.pointSize: 18
                        anchors.left: parent.left
                        anchors.leftMargin: 60
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        validator: IntValidator {
                            bottom: 0
                                        top: 50
                                   } // az érték egész szám 0-50 között
                        style: TextFieldStyle {
                            background: Rectangle {
                                 color: "white"
                                 border.color: "black"
                                 implicitHeight: 50
                                 implicitWidth: 100
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
                    text: " I: "
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                    id: textInput2
                    width: 100
                    height: 50
                    font.pointSize: 18
                    anchors.left: parent.left
                    anchors.leftMargin: 60
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    validator: IntValidator {
                        bottom: 0
                                    top: 50
                               } // az érték egész szám 0-50 között
                    style: TextFieldStyle {
                        background: Rectangle {
                             color: "white"
                             border.color: "black"
                             implicitHeight: 50
                             implicitWidth: 100
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
                    text: " D: "
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    font.bold: true
                    font.pointSize: 24
                }
                TextField {
                    id: textInput3
                    width: 100
                    height: 50
                    font.pointSize: 18
                    anchors.left: parent.left
                    anchors.leftMargin: 60
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    validator: IntValidator {
                        bottom: 0
                                    top: 50
                               } // az érték egész szám 0-50 között
                    style: TextFieldStyle {
                        background: Rectangle {
                             color: "white"
                             border.color: "black"
                             implicitHeight: 50
                             implicitWidth: 100
                        }
                    }
                }
            }
       }
   }

    // X és Y adatok kijelzése
    GroupBox {
        id: gyr
        width: 400
        anchors.top: angoffset.bottom
        anchors.topMargin: 0
        anchors.left: parent.left

        // Sorba rendezés
        RowLayout {
            id: rowLayout5
            anchors.verticalCenterOffset: 2
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 15
            spacing: 70

            Text {
                width: 100
                text: " X: KalmanX " // a KalmanX változó beillesztése szükséges!!!
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 30
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                width: 100
                text: " Y: KalmanY " // a KalmanY változó beillesztése szükséges!!!
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 30
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pointSize: 12
            }
        }
    }

    Component {
        id: stateDelegate
        Row {
            Text { text: model.statusName }
            Text { text: " X: " + model.x.toFixed(3) }
            Text { text: " V: " + model.v.toFixed(3) }
            Text { text: " A: " + model.a.toFixed(3) }
        }
    }

    GroupBox {
        id: graphic
        anchors.top: gyr.bottom
        anchors.topMargin: 0
        // Oldalra és lefelé kitölti a szülőt.
        anchors.right: parent.right
        anchors.left: parent.left

        // Sorban egymás mellett van a lista és a grafikon
        RowLayout {
            anchors.fill: parent
            spacing: 20

            HistoryGraph {
                id: historyGraphAngle
                width: 315                
                objectName: "historyGraphAngle"

                Layout.minimumHeight: 150                
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.fillWidth: true                
                
                graphA: historyGraphTimestamps // adatsorok megadása szükséges!!!
                graphB: historyGraphVelocity
                graphC: historyGraphAcceleration
            }

            HistoryGraph {
                id: historyGraphGyr
                width: 315
                objectName: "historyGraphGyr"
                
                Layout.minimumHeight: 150
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.fillWidth: true
                
                graphA: historyGraph1Timestamps // adatsorok megadása szükséges!!!
                graphB: historyGraph1Velocity
                graphC: historyGraph1Acceleration
            }

            HistoryGraph {
                id: historyGraphAcc
                width: 315
                objectName: "historyGraphAcc"
                
                Layout.minimumHeight: 150
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.fillWidth: true
                
                graphA: historyGraph1Timestamps // adatsorok megadása szükséges!!!
                graphB: historyGraph1Velocity
                graphC: historyGraph1Acceleration
            }
        }
    }
}
