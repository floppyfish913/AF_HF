import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

ApplicationWindow {
    objectName: "ApplicationWindow"
    title: qsTr("Alkalmazásfejlesztés HF")
    width: 500
    height: 500
    visible: true

    signal powerSwitchCpp()
    signal setPIDCpp()
    signal refreshPIDCpp()

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Menu")
            MenuItem {
                text: qsTr("&Exit")
                onTriggered: Qt.quit();
            }
        }
    }

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
    }
}
