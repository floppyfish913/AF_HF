#include <QQmlProperty>
#include <QQmlContext>
#include "ApplicationWindow.h"
#include "quadro_msg.h"

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

    qDebug() << "QML signal csatlakoztatása";
    QObject::connect(mainWindowObject, SIGNAL(powerSwitchCpp()),
        this, SLOT(PowerSwitchCommand()));
    QObject::connect(mainWindowObject, SIGNAL(setPIDCpp()),
        this, SLOT(SetPIDCommand()));
    QObject::connect(mainWindowObject, SIGNAL(refreshPIDCpp()),
        this, SLOT(RefreshPIDCommand()));
    QObject::connect(mainWindowObject, SIGNAL(sendIPCpp()),
        this, SLOT(SendIPCommand()));

    qDebug() << "ApplicationWindow inicializálva.";
}

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
    QString msg = "{\"MessageType\":\"GetPID\",\"Kp\":4,\"Ki\":12,\"Kd\":44}";
    emit TCP_send(msg);
}

void ApplicationWindow::SendIPCommand()
{
    QObject * inputP = mainWindowObject->findChild<QObject*>("iptext");
    // msg += inputP->property("text").toString();
    inputP = mainWindowObject->findChild<QObject*>("porttext");
    // msg += inputP->property("text").toString();
}

void ApplicationWindow::StateChanged()
{
    QString qka = "5.3";
    qka.toFloat();
    QString qka2 = "158.15";
    qmlContext.setContextProperty(QStringLiteral("kalmanAngle"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("kalmanOffset"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("kalmanXState"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("kalmanYState"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("heightState"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("motor1State"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("motor2State"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("motor3State"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("motor4State"), QVariant::fromValue(qka2));
    qmlContext.setContextProperty(QStringLiteral("AccXGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("AccYGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("GyroXGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("GyroYGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("GyroZGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("AngleXGraphs"), QVariant::fromValue(qka));
    qmlContext.setContextProperty(QStringLiteral("AngleYGraphs"), QVariant::fromValue(qka));
    emit ContextChanged();
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
void ApplicationWindow::connectQmlSignals(QObject *rootObject)
{
    QQuickItem *historyGraphGyr = findItemByName(rootObject,QString("historyGraphGyr"));
    QQuickItem *historyGraphAcc = findItemByName(rootObject,QString("historyGraphAcc"));
    QQuickItem *historyGraphAngle = findItemByName(rootObject,QString("historyGraphAngle"));
    if (historyGraphGyr)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraphGyr, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "HIBA: Nem találom a historyGraphGyr objektumot a QML környezetben.";
    }
    if (historyGraphAcc)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraphAcc, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "HIBA: Nem találom a historyGraphAcc objektumot a QML környezetben.";
    }
    if (historyGraphAngle)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraphAngle, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "HIBA: Nem találom a historyGraphAngle objektumot a QML környezetben.";
    }
}

void ApplicationWindow::GetState(Quadro_msg msg){
    // qDebug()<< "Motorstate0 : " << QString::number(msg.MotorState0) << endl;
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
