import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

ApplicationWindow {
    objectName: "ApplicationWindow"
    title: qsTr("Alkalmazásfejlesztés HF")
    width: 940
    height: 630
    visible: true

    // QML oldali signalok
    signal powerSwitchCpp()
    signal setPIDCpp()
    signal refreshPIDCpp()
    signal sendIPCpp()
    signal connection_failCpp();

    // felső menüsorban egy elem, azon belül egy opció
    menuBar: MenuBar {
        Menu {
            title: qsTr("&Menu")
            MenuItem {
                text: qsTr("&Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    /* a MainForm ami az egész ablak meghatározó eleme, külön qml fájlba
       a MainForm jelei innen átvezetve kerülnek a C++ oldalra */
    MainForm {
            id: mainFormControl
            anchors.fill: parent

             onPowerSwitch: {
                powerSwitchCpp();
            }
             onSet_PID: {
                setPIDCpp();
            }
             onRefresh_PID: {
                refreshPIDCpp();
            }
             onSend_IP: {
                sendIPCpp();
            }
             onConnection_fail:  {
                connection_failCpp();
            }

    }
}
