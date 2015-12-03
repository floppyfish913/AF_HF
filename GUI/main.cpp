#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "ApplicationWindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    ApplicationWindow appWindow(engine.rootObjects()[0]);

    return app.exec();
}
