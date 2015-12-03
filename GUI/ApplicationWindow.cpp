#include <QQmlProperty>
#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(QObject *rootObject)
    : QObject(nullptr)
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

    qDebug() << "ApplicationWindow inicializálva.";
}

void ApplicationWindow::PowerSwitchCommand()
{
    // ide jön majd a JSON string küldése!
}

void ApplicationWindow::SetPIDCommand()
{
    // ide jön majd a JSON string küldése!
}

void ApplicationWindow::RefreshPIDCommand()
{
    // ide jön majd a JSON string küldése!
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
        // Gyerekekben keresés
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
