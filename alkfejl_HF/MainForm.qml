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
        visible: true // szükséges, hogy a GUI indításakor megjelenjen
        modality: Qt.ApplicationModal // amíg nem töltjük ki, nem mehetünk tovább
        title: "Connecting"
        onAccepted: send_IP() // OK gomb nyomására elküldi a megadott IP-címet és port számot az
                              // ApplicationWindow osztály felé ez a signal
        // Két oszlop a két mezőnek és sorban egymás után a mező meghatározása szöveggel, majd maga a mező
        // Az IP-s mezőben mint látható, csak valid IP-címek adhatóak meg, ugyanígy a port számának megadásánál is
        ColumnLayout {
            id: connectColumn
            spacing: 15
            RowLayout {
                id: connectRow1
                Text { text: "IP Address: "}
                TextField {
                    objectName: "iptext" // szükséges, hogy elérjük C++ oldalról
                    validator: RegExpValidator {
                        regExp: /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/gm
                        }
                    // A mező stílusa, fehér háttér, fekete keret
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
                width: connectRow1.width // a két sor szélessége legyen ugyanakkora
                Text { text: "Port: "}
                TextField {
                    objectName: "porttext" // szükséges, hogy elérjük C++ oldalról
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
        visible: false // nem szükséges, hogy indulásnál egyből látható legyen
        title: "Power Off Dialog"
        Text {
            text: "Connection Failed! Try Again!"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter // a szöveg legyen középre helyezve
            anchors.verticalCenter: parent.verticalCenter
        }
        standardButtons: StandardButton.Ok
        onAccepted: connection_fail() // OK gomb megnyomására kiad egy signalt, ami az IP-cím megadó felületre visszanavigál
    }

    // Motor kikapcsolásakor felugró ablak három gombbal és egy kérdéssel
    Dialog {
        id: powerOffDialog
        width: 400
        height: 100
        visible: false // nem szükséges, hogy indulásnál egyből látható legyen
        title: "Power Off Dialog"
        Text {
            text: "Are you sure you want to turn off the power?"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter // a szöveg legyen középre helyezve
            anchors.verticalCenter: parent.verticalCenter
        }
        standardButtons: StandardButton.Yes | StandardButton.No | StandardButton.Cancel
        onYes: container.toggle() // csak Igen gomb nyomására változik a bekapcsoló gomb állapota
    }

    // A dőlésszögmérő ábrázolása
    GroupBox {
        id: angoffset
        width: 600
        height: 600
        anchors.leftMargin: 0 // az elem igazodjon a szülő (jelen esetben az egész ablak) bal és felső oldalához
        anchors.topMargin: 0
        anchors.top: parent.top
        anchors.left: parent.left
        // Ezen az elemen eltolást és forgatást is kell végezni a megfelelő paraméterekkel,
        // ezért egy Row Layoutba került amely kitölti a szülő objektumot
        RowLayout {
            id: rotation
            anchors.fill: parent // ki kell töltenie a szülő objektumot, hogy egyszerűen megadható legyen a forgatás tengelye
            transform: Rotation {
                            origin.x: rotation.width/2
                            origin.y: rotation.height/2
                            angle: kalmanAngle
            } // forgatás a középpont körül
            // A dőlésszögmérő ábrája különböző rétegekből van összerakva,
            // a rajtuk végzett műveletek szerint csoportosítva
            Image {
                id: raise
                anchors.left: parent.left // az elem igazodjon a szülő bal és felső oldalához
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "/pics/raise.png"
                anchors.fill: parent // az elem töltse ki a szülőt
                transform: Translate {
                    y: kalmanOffset
                }
            }
        }
        Image {
            id: rot
            anchors.left: parent.left // igazodjon a szülő bal és felső oldalához és töltse is ki azt
            anchors.top: parent.top
            source: "/pics/rot.png"
            anchors.fill: parent
            transform: Rotation { angle: kalmanAngle; origin.x: rot.width/2; origin.y: rot.height/2 } // forgatás a középpont körül
        }
        Image {
            id: frame
            anchors.left: parent.left // igazodjon a szülő bal és felső oldalához és töltse is ki azt
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
        anchors.left: pidParameters.right // legyen a PID paraméter megadó felület mellett 5 pont kihagyással
        anchors.leftMargin: 5
        anchors.top: pidParameters.top // legyen a PID paraméter megadó felület felső oldalánál 5 ponttal lejjebb a teteje
        anchors.topMargin: 5
                Rectangle {
                    objectName: "PW_button" // szükséges, hogy elérjük C++ oldalról
                    id: container
                    width: 100
                    height: 25
                    border.color: "black" // fekete, két pont széles keret
                    border.width: 2
                    radius: 5 // 5 pont sugarú, lekerekített szélek
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    function toggle() { // ezt a függvényt hívjuk meg, ha váltani kell a GUI-n az állapotát
                        if (state == "on") {
                            state = "off"
                            powerSwitch() // ez a signal megy át C++ oldalra
                        }
                        else {
                            state = "on"
                            powerSwitch()
                        }
                    }
                    state: "on" // alaphelyzetben legyen bekapcsolt állapotban
                    states: [
                        State { // ez adja meg a két állapot bekövetkeztekor létrejövő változtatásokat
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
                    // a MouseArea érzékeli a kattintást, elengedésre a pillanatnyi állapottól függő esemény jön létre
                    MouseArea { id: region; anchors.verticalCenter: parent.verticalCenter; anchors.top: parent.top; anchors.right: parent.right; anchors.left: parent.left; onReleased: (container.state == "off") ? container.toggle() : powerOffDialog.open() }                   
                }
            }

    // A PID parancsai
    GroupBox {
        id: pidCommands
        anchors.left: angoffset.right // legyen tíz pontnyira a dőlésszögmérőtől jobbra
        width: 200
        height: 50
        anchors.top: pidParameters.bottom // legyen a PID paraméter megadó felület alatt közvetlenül
        anchors.leftMargin: 10
        anchors.topMargin: 0

        // Egy sorban két gomb, egy beállító és egy lekérő funkcióval rendelkező
        RowLayout {
            id: rowLayout1
            anchors.fill: parent
            spacing: 20 // A gombok között legyen hely kihagyva
            // A Set gombbal beállítható a beírt érték, a Refresh-sel pedig lekérjük a jelenleg érvényes beállításokat
            Button {
                id: setPID
                anchors.left: parent.left // a szülő bal szélén legyen
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Set PID")
                onClicked: set_PID() // ez a signal megy át C++ oldalra
            }
            Button {
                id: refreshPID
                anchors.right: parent.right // a szülő jobb szélén legyen
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Refresh PID")
                onClicked: refresh_PID() // ez a signal megy át C++ oldalra
            }
        }
    }

    // PID paraméterek beírására szolgáló felület
    GroupBox {
        id: pidParameters
        anchors.left: angoffset.right // legyen tíz pontnyira a dőlésszögmérőtől jobbra
        anchors.top : parent.top // igazodjon a felső oldala a szülő felső oldalához
        anchors.leftMargin: 10
        anchors.topMargin: 0
        width: 200
        height: 200

        // Egy oszlopban P, I és D paraméterek megadására szolgáló mezők és a hozzájuk tartozó felirat
        ColumnLayout {
            id: columnLayout2
            anchors.fill: parent // töltse ki a szülő elemet
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
                    anchors.top: parent.top // a szülő tetejétől öt pontnyira
                    anchors.topMargin: 5
                    horizontalAlignment: Text.AlignHCenter // horizontálisan középre helyezve, vertikálisan felső elhelyezés
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
                        anchors.left: parent.left // 60 pontnyira a szülő bal oldalától kezdődjön
                        anchors.leftMargin: 60
                        horizontalAlignment: Text.AlignHCenter // beírás esetén középre helyezve tudjunk beírni
                        font.bold: true
                        font.pointSize: 18
                        validator: IntValidator {
                                        bottom: 1
                                        top: 50
                                   } // az érték egész szám 1-50 között
                        style: TextFieldStyle { // fehér hátterű és fekete keretű legyen
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
                anchors.top: rowLayout2.bottom // 10 ponttal a P paraméter alatt
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
                anchors.top: rowLayout3.bottom // 10 ponttal a D paraméter alatt
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
        anchors.left: angoffset.right // elhelyezkedés leírása
        anchors.leftMargin: 10
        anchors.right: rectangle1.right
        anchors.top: pidCommands.bottom
        anchors.topMargin: 0
        anchors.bottom: angoffset.bottom

    // Egy oszlopban az összes adat kijelzése 45 pontnyi eltéréssel a sorok között
       ColumnLayout {
           id: gyrColumn
           anchors.fill: parent
            Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: " X: " + kalmanXState
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 20
                elide: Text.ElideMiddle
                font.bold: true
                font.pointSize: 12
            }
            Text {
                width: 100
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 0
                anchors.topMargin: 65
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
                anchors.topMargin: 110
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
                anchors.topMargin: 155
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
                anchors.topMargin: 200
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
                anchors.topMargin: 245
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
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 28
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
