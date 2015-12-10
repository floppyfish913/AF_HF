#pragma once
#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H
#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QQuickView>
#include "quadro_msg.h"

class ApplicationWindow : public QObject
{
    Q_OBJECT

public:
    ApplicationWindow(QObject *rootObject, QQmlContext& qmlContext);
    ~ApplicationWindow() = default;
    void connectQmlSignals(QObject *rootObject);
public slots:
    void PowerSwitchCommand();
    void SetPIDCommand();
    void RefreshPIDCommand();
    void SendIPCommand();
    void StateChanged();
    //tcp slots
    void GetState(Quadro_msg msg);
signals:
    void ContextChanged();
    void TCP_send(QString msg);
private:
    QQuickItem* findItemByName(const QString& name);
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    QQuickItem* mainWindowObject;
    QQuickItem* object;
    QQuickView* myObject;
    QQmlContext& qmlContext;
};

#endif // APPLICATIONWINDOW_H

