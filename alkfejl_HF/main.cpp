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
    TCP_com Connection;
    //Connection.setServer("192.168.0.5");
    Connection.setServer("localhost");
    Connection.setPort(1234);

    appWindow = new ApplicationWindow(engine.rootObjects()[0], engine.rootContext()[0]);
    QObject::connect(appWindow,SIGNAL(TCP_send(QString)),&Connection,SLOT(send(QString)));
    QObject::connect(&Connection,SIGNAL(Get_State_received(Quadro_msg)),appWindow,SLOT(GetState(Quadro_msg)));
    Connection.connect_to();
    //ApplicationWindow appWindow(engine.rootObjects()[0], engine.rootContext()[0]);
    appWindow->StateChanged();

    return app.exec();
}
