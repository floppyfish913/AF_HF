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

    // Signalok, melyek a kiadott parancsokat jelzik és a nyomógombok
    //  eseménykezelői aktiválják őket.
    signal powerSwitch;
    signal setPID;
    signal refreshPID;

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
        width: 400
        height: 400
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.top: parent.top
        anchors.left: parent.left
         ColumnLayout {
            id: columnLayout6
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            transform: Rotation {
                angle: 20 // ide jön KalmanX
                origin.x: columnLayout6.width/2
                origin.y: columnLayout6.height/2
            }
            Rectangle {
                id: angrect1
                width: 150
                height: 200
                color: "blue"
                anchors.verticalCenterOffset: -50
                anchors.verticalCenter: parent.verticalCenter
                anchors.fill: parent.fill
            }
            Rectangle {
                id: angrect2
                width: 150
                height: 150
                color: "brown"
                anchors.verticalCenterOffset: 75
                anchors.verticalCenter: parent.verticalCenter
                anchors.fill: parent.fill
                transform: Translate {
                    y: -20 // ide jön KalmanY
                }
            }
        }
    }

    // A bekapcsoló gomb és a tápfeszültség kiírása
    GroupBox {
        id: power
        height: 80
        anchors.left: pidParameters.right
        anchors.right: parent.right
        anchors.top: parent.top        
        anchors.leftMargin: 200
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
                text: "Battery: " + ((container.state == "on") ? "15.1" : "0.0" ) + " V " // ide a beolvasott érték kerül!
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
        anchors.leftMargin: 0
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
                onClicked: setPID()
            }
            Button {
                id: refreshPID
                anchors.right: parent.right                
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Refresh PID")
                onClicked: refreshPID()
            }
        }
    }

    // PID paraméterek beírása
    GroupBox {
        id: pidParameters
        anchors.left: angoffset.right
        anchors.top : parent.top
        anchors.leftMargin: 0
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

    // X és Y adatok kijelzése
    GroupBox {
        id: gyr
        width: 400
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
                text: " X: KalmanX " // a KalmanX változó beillesztése szükséges!!!              
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
                text: " Y: KalmanY " // a KalmanY változó beillesztése szükséges!!!
                font.bold: true
                font.pointSize: 12
            }
        }
    }

    Component {
        id: stateDelegate
        Row {
            Text { text: model.statusName }
            Text { text: " X: " }
            Text { text: " V: " }
            Text { text: " A: " }
        }
    }

    GroupBox {
        id: graphic
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: gyr.bottom
        anchors.topMargin: 0
        // Oldalra és lefelé kitölti a szülőt.

        // Sorban egymás mellett van a lista és a grafikon
        RowLayout {
            anchors.fill: parent
            spacing: 20
            ColumnLayout {
                id: columnLayout3
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
                Text{ text: "Angle" ; anchors.horizontalCenter: parent.horizontalCenter; font.bold: true;font.pointSize: 10}
            }

            ColumnLayout {
                id: columnLayout4
                HistoryGraph {
                    id: historyGraphGyr
                    width: 315
                    objectName: "historyGraphGyr"

                    Layout.minimumHeight: 150
                    Layout.fillHeight: true
                    Layout.minimumWidth: 200
                    Layout.fillWidth: true

                    graphA: historyGraphGyroX // adatsorok megadása szükséges!!!
                    graphB: historyGraphGyroY
                    graphC: historyGraphGyroZ
                }
                Text{ text: "Gyroscope" ; font.bold: true; font.pointSize: 10;anchors.horizontalCenter: parent.horizontalCenter}
            }
            ColumnLayout {
                id: columnLayout5
                HistoryGraph {
                    id: historyGraphAcc
                    width: 315
                    objectName: "historyGraphAcc"

                    Layout.minimumHeight: 150
                    Layout.fillHeight: true
                    Layout.minimumWidth: 200
                    Layout.fillWidth: true

                    graphA: historyGraphAccelX // adatsorok megadása szükséges!!!
                    graphB: historyGraphAccelY
                }
                Text{ text: "Acceleration" ; font.bold: true; font.pointSize: 10;anchors.horizontalCenter: parent.horizontalCenter}
            }
        }
    }
}
