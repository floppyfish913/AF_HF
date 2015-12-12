#include <QQmlProperty>
#include <QQmlContext>
#include "ApplicationWindow.h"
#include "quadro_msg.h"

/** Konstruktor alapján létrehozva és átadva a szükséges paraméterek */

ApplicationWindow::ApplicationWindow(QObject *rootObject, QQmlContext& qmlContext)
    : QObject(nullptr), qmlContext(qmlContext)
{
    if (!rootObject)
    {
        qDebug() << "Nem találom a rootObject-et.";
    }

    mainWindowObject = findItemByName(rootObject, QString("ApplicationWindow"));

    if (!mainWindowObject)
    {
        qDebug() << "Nem találom a ApplicationWindow objektumot.";
    }

    /** QML oldali jelek csatlakoztatása signal-slot rendszerben */

    qDebug() << "QML signal csatlakoztatása";
    QObject::connect(mainWindowObject, SIGNAL(powerSwitchCpp()),
        this, SLOT(PowerSwitchCommand()));
    QObject::connect(mainWindowObject, SIGNAL(setPIDCpp()),
        this, SLOT(SetPIDCommand()));
    QObject::connect(mainWindowObject, SIGNAL(refreshPIDCpp()),
        this, SLOT(RefreshPIDCommand()));
    QObject::connect(mainWindowObject, SIGNAL(sendIPCpp()),
        this, SLOT(SendIPCommand()));
    QObject::connect(mainWindowObject, SIGNAL(connection_failCpp()),
        this, SLOT(connection_failCommand()));
    qDebug() << "ApplicationWindow inicializálva.";
}

/** Az egyes signalokhoz tartozó parancsok általában
 * egy üzenetet kell, hogy összerakjanak, melyet
 * JSON Stringként kap meg az eszköz, amelyet aztán majd értelmez.
 * FindChild metódussal találjuk meg a mainWindowObejcten belül a keresett objektumot,
 * melynek valamely beállítását változtatjuk, hogy legyen jele az esemény megtörténtének
 * a GUI-n is. A TCP_send kibocsátásával küldjük el az üzenetet.
 */

void ApplicationWindow::PowerSwitchCommand()
{
    QString msg = "{\"MessageType\":\"SetMainPower\",\"MainPowerStatus\":\"";
    QObject * inputP = mainWindowObject->findChild<QObject*>("PW_button");
    if (inputP->property("state") == "on")
        msg += "false";
    else
        msg += "true";
    msg +="\"}";
    emit TCP_send(msg);
}

void ApplicationWindow::SetPIDCommand()
{
    QString msg = "{\"MessageType\":\"SetPID\",\"Kp\":";
    QObject * inputP = mainWindowObject->findChild<QObject*>("textInputP");
    msg += inputP->property("text").toString();

    inputP = mainWindowObject->findChild<QObject*>("textInputI");
    msg += ",\"Ki\":" + inputP->property("text").toString();

    inputP = mainWindowObject->findChild<QObject*>("textInputD");
    msg += ",\"Kd\":" + inputP->property("text").toString();

    msg +="}";
    emit TCP_send(msg);
}

void ApplicationWindow::RefreshPIDCommand()
{
    QString msg = "{\"MessageType\":\"GetPID\"}";
    emit TCP_send(msg);
}

void ApplicationWindow::SendIPCommand()
{
    QObject * inputP = mainWindowObject->findChild<QObject*>("iptext");
    QString ip = inputP->property("text").toString();
    inputP = mainWindowObject->findChild<QObject*>("porttext");
    int port= inputP->property("text").toInt();
    emit SendConnect(ip,port);
}

/** Itt az ablakok újranyitása történik helytelen IP-cím megadása után. */

void ApplicationWindow::connection_failed(){
    QObject * inputP = mainWindowObject->findChild<QObject*>("failedDialog");
    inputP->setProperty("visible",QVariant::fromValue(true));
}

void ApplicationWindow::connection_failCommand(){
    QObject * inputP = mainWindowObject->findChild<QObject*>("connectionDialog");
    inputP->setProperty("visible",QVariant::fromValue(true));
}

void ApplicationWindow::connected(){

    QObject * inputP = mainWindowObject;
    inputP->setProperty("visible",QVariant::fromValue(true));

}

QQuickItem* ApplicationWindow::findItemByName(const QString& name)
{
    Q_ASSERT(mainWindowObject != nullptr);
    if (mainWindowObject->objectName() == name)
    {
        return mainWindowObject;
    }
    return findItemByName(mainWindowObject->children(), name);
}

QQuickItem* ApplicationWindow::findItemByName(QObject *rootObject, const QString& name)
{
    Q_ASSERT(rootObject != nullptr);
    if (rootObject->objectName() == name)
    {
        return (QQuickItem*)rootObject;
    }
    return findItemByName(rootObject->children(), name);
}

QQuickItem* ApplicationWindow::findItemByName(QList<QObject*> nodes, const QString& name)
{
    for(int i = 0; i < nodes.size(); i++)
    {
        // Node keresése
        if (nodes.at(i) && nodes.at(i)->objectName() == name)
        {
            return dynamic_cast<QQuickItem*>(nodes.at(i));
        }
        // Gyerek elemekben keresés
        else if (nodes.at(i) && nodes.at(i)->children().size() > 0)
        {
            QQuickItem* item = findItemByName(nodes.at(i)->children(), name);
            if (item)
                return item;
        }
    }
    // Nem találtuk.
    return nullptr;
}

/** A visszakapott üzenetet , amely szintén egy JSON String, szétbontja a message_handler
 * így egyesével tudjuk csatlakoztatni a setContextProperty függvénnyel a QML oldali változókhoz.
 * A változókat ez a függvény jegyzi be QML oldalon is.
 */

void ApplicationWindow::GetState(Quadro_msg msg){
    qmlContext.setContextProperty(QStringLiteral("motor1State"), QVariant::fromValue(QString::number(msg.MotorState0)));
    qmlContext.setContextProperty(QStringLiteral("motor2State"), QVariant::fromValue(QString::number(msg.MotorState1)));
    qmlContext.setContextProperty(QStringLiteral("motor3State"), QVariant::fromValue(QString::number(msg.MotorState2)));
    qmlContext.setContextProperty(QStringLiteral("motor4State"), QVariant::fromValue(QString::number(msg.MotorState3)));
    qmlContext.setContextProperty(QStringLiteral("kalmanAngle"), QVariant::fromValue(msg.KalmanX));
    qmlContext.setContextProperty(QStringLiteral("kalmanOffset"), QVariant::fromValue(msg.KalmanY));
    qmlContext.setContextProperty(QStringLiteral("kalmanXState"), QVariant::fromValue(QString::number(msg.KalmanX)));
    qmlContext.setContextProperty(QStringLiteral("kalmanYState"), QVariant::fromValue(QString::number(msg.KalmanY)));
    qmlContext.setContextProperty(QStringLiteral("heightState"), QVariant::fromValue(QString::number(msg.Altitude)));

}

/** PID paraméterek lekérdezése után szeretnénk, ha
 * a kiolvasott értékek megjelennének a GUI-n, ezért
 * a paraméterek beállítására szolgáló TextField-ekbe
 * beleírjuk azokat.
 */

void ApplicationWindow::GetPID(Quadro_msg msg)
{
     QObject * inputP = mainWindowObject->findChild<QObject*>("textInputP");
    inputP->setProperty("text",QVariant::fromValue(QString::number(msg.Kp)));
    inputP = mainWindowObject->findChild<QObject*>("textInputI");
    inputP->setProperty("text",QVariant::fromValue(QString::number(msg.Ki)));
    inputP = mainWindowObject->findChild<QObject*>("textInputD");
    inputP->setProperty("text",QVariant::fromValue(QString::number(msg.Kd)));
}
