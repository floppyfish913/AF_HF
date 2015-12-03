#pragma once
#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H
#include <QObject>
#include <QQuickItem>

class ApplicationWindow : public QObject
{
    Q_OBJECT

public:
    ApplicationWindow(QObject *rootObject);
    ~ApplicationWindow() = default;

public slots:
    /* Eseménykezelő a QML oldali addGreenEntry signalhoz. */
    void PowerSwitchCommand();
    void SetPIDCommand();
    void RefreshPIDCommand();
private:
    QQuickItem* findItemByName(const QString& name);
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    QQuickItem* mainWindowObject;
};

#endif // APPLICATIONWINDOW_H

