#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "ApplicationWindow.h"
#include "tcp_com.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    ApplicationWindow * appWindow;
    /** Létre kell hozni az alkalmazás ablaka mellett egy kapcsolatot is,
     * melyen keresztül kommunikálunk az eszközzel.
     */
    TCP_com Connection;
    appWindow = new ApplicationWindow(engine.rootObjects()[0], engine.rootContext()[0]);
    QObject::connect(appWindow,SIGNAL(TCP_send(QString)),&Connection,SLOT(send(QString)));
    QObject::connect(appWindow,SIGNAL(SendConnect(QString,int)),&Connection,SLOT(connect_to_device(QString,int)));
    QObject::connect(&Connection,SIGNAL(Get_State_received(Quadro_msg)),appWindow,SLOT(GetState(Quadro_msg)));
    QObject::connect(&Connection,SIGNAL(Get_PID_received(Quadro_msg)),appWindow,SLOT(GetPID(Quadro_msg)));
    QObject::connect(&Connection,SIGNAL(connection_failed()),appWindow,SLOT(connection_failed()));

    return app.exec();
}
