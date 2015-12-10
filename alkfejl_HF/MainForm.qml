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
    signal send_IP;
    signal connection_fail;

    // Kezdésként külön ablakban meg kell adni az eszköz IP-címét és a csatlakoztatni kívánt port számát
    Dialog {
        objectName : "connectionDialog"
        id: connectionDialog
        width: 300
        height: 150
        visible: false
        modality: Qt.ApplicationModal // amíg nem töltjük ki, nem mehetünk tovább
        title: "Connecting"
        onAccepted: send_IP()
        // Két oszlop a két mezőnek és sorban egymás után a mező meghatározása szöveggel, majd maga a mező
        // az IP-s mezőben mint látható, csak valid IP-címek adhatóak meg, ugyanígy a port számának megadásánál is
        ColumnLayout {
            id: connectColumn
            spacing: 15
            RowLayout {
                id: connectRow1
                Text { text: "IP Address: "}
                TextField {
                    objectName: "iptext"
                    validator: RegExpValidator {
                        regExp: /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/gm
                        }
                    // a mező stílusa, fehér háttér, fekete keret
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
                    objectName: "porttext"
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

    // Helytelen IP-cím megadásakor felugró ablak egy felirattal és OK gombbal
    Dialog {
        objectName: "failedDialog"
        id: failedDialog
        width: 400
        height: 100
        visible: false
        title: "Power Off Dialog"
        Text {
            text: "Connection Failed! Try Again!"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        standardButtons: StandardButton.Ok
        onAccepted: connection_fail()
    }

    // Motor kikapcsolásakor felugró ablak három hombbal és egy kérdéssel
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
        // Ezt az elemet tolni és forgatni is kell a megfelelő paraméterekkel, ezért egy Raw Layoutba került
        // amely kitölti a szülő objektumot
        RowLayout {
            id: rotation
            anchors.fill: parent
            transform: Rotation {
                            origin.x: rotation.width/2
                            origin.y: rotation.height/2
                            angle: kalmanAngle
            }
            // A dőlésszögmérő ábrája különböző rétegekből van összerakva,
            // a rajtuk végzett műveletek szerint csoportosítva
            Image {
                id: raise
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "/pics/raise.png"
                anchors.fill: parent
                transform: Translate {
                    y: kalmanOffset
                }
            }
        }
        Image {
            id: rot
            anchors.left: parent.left
            anchors.top: parent.top
            source: "/pics/rot.png"
            anchors.fill: parent
            transform: Rotation { angle: kalmanAngle; origin.x: rot.width/2; origin.y: rot.height/2 }
        }
        Image {
            id: frame
            anchors.left: parent.left
            anchors.top: parent.top
            source: "/pics/frame.png"
            anchors.fill: parent
        }
    }

    // A bekapcsoló gomb Toggle Button, vagyis benyomva marad bekapcsolás után.
    // Az állapotától függ a felirata, a színe és a klikkelésre történő változás is.
    Rectangle {
        id: rectangle1
        width: 100
        height: 25
        anchors.left: pidParameters.right
        anchors.leftMargin: 5
        anchors.top: pidParameters.top
        anchors.topMargin: 5
                Rectangle {
                    objectName: "PW_button"
                    id: container
                    width: 100
                    height: 25
                    border.color: "black"
                    border.width: 2
                    radius: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
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
                    state: "on"
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
                    Text { id: btnText; text: "Power Off"; anchors.left: parent.left; anchors.leftMargin: 0; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter; anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; }
                    MouseArea { id: region; anchors.verticalCenter: parent.verticalCenter; anchors.top: parent.top; anchors.right: parent.right; anchors.left: parent.left; onReleased: (container.state == "off") ? container.toggle() : powerOffDialog.open() }
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
                                        bottom: 1
                                        top: 50
                                   } // az érték egész szám 1-50 között
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
                                    bottom: 1
                                    top: 50
                               } // az érték egész szám 1-50 között
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
                                    bottom: 1
                                    top: 50
                               } // az érték egész szám 1-50 között
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

    // Az eszközről bejövő adatok kijelzése
    GroupBox {
        id: gyr
        anchors.left: angoffset.right
        anchors.leftMargin: 10
        anchors.right: rectangle1.right
        anchors.top: pidCommands.bottom
        anchors.topMargin: 0
        anchors.bottom: angoffset.bottom

    // Oszlopba rendezés 30 pontnyi kihagyással a sorok között
       ColumnLayout {
           id: gyrColumn
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 0
           anchors.top: parent.top
           anchors.topMargin: 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: 30
            Text {
                width: 100
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: " X: " + kalmanXState
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 0
                elide: Text.ElideMiddle
                font.bold: true
                font.pointSize: 12
            }
            Text {
                width: 100
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 0
                anchors.topMargin: 0
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
                text: " Y: " + kalmanYState
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Height: " + heightState + " cm"
                anchors.topMargin: 0
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            // Motorteljesítmények %-ban kifejezve, 2 tizedesjegyig
            Text {
                text: " Motor 1 Performance: " + (motor1State/40.95).toFixed(2) + " %"
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 2 Performance: " + (motor2State/40.95).toFixed(2) + " %"
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 3 Performance: " + (motor3State/40.95).toFixed(2) + " %"
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
            Text {
                text: " Motor 4 Performance: " + (motor4State/40.95).toFixed(2) + " %"
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 12
            }
        }
    }
}
