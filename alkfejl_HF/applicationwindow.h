#pragma once
#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H
#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QQuickView>
#include "quadro_msg.h"

/**
 * @brief A GUI eseménykezelőinek osztálya.
 *
 * A QML oldalról jövő signalokhoz kapcsolódik, és az egyes signalokhoz
 * tartozó függvények leírása is itt található meg.
 */

class ApplicationWindow : public QObject
{
    Q_OBJECT

public:
    /** Konstruktor
         * @param rootObject A QML motortól kapott gyökér objektum
         * (ez alapján találja meg a többi objektumot a konstruktor)
         * @param qmlContext    A QML context objektum
         */
    ApplicationWindow(QObject *rootObject, QQmlContext& qmlContext);
    ~ApplicationWindow() = default;
public slots:
    /** A bekapcsoló gomb eseménykezelője */
    void PowerSwitchCommand();
    /** A PID paraméter beállító gomb eseménykezelője */
    void SetPIDCommand();
    /** A PID paraméter lekérő gomb eseménykezelője */
    void RefreshPIDCommand();
    /** A GUI indításakor felugró ablak gombjának eseménykezelője */
    void SendIPCommand();
    /** A GUI indításakor felugró ablakban rossz IP-cím megadásakor létrejövő event eseménykezelője */
    void connection_failed();
    /** A rossz IP-cím megadása után bekövetkező event eseménykezelője */
    void connection_failCommand();

     /** Seikeres kapcsolódás esemény kezelése */
    void connected();
    /** TCP slotok
     *  Összes állapotjelző érték lekérésének eseménykezelője
     */
    void GetState(Quadro_msg msg);
    /** A PID paraméterek lekérésének eseménykezelője */
    void GetPID(Quadro_msg msg);
signals:
    /** Adatváltozást jelez */
    void ContextChanged();
    /** Üzenetküldés jele */
    void TCP_send(QString msg);
    /** GUI indításkor felugró ablaknál IP-cím és port számának küldésének jele */
    void SendConnect(QString ip,int port);
private:
    /** Segédfüggvény QML elemek rekurzív megkeresésére. */
    QQuickItem* findItemByName(const QString& name);
    /** Segédfüggvény QML elemek rekurzív megkeresésére.
         * @param rootObject A QML gyökérelem, amivel a keresést kezdjük.
         * @param name Az objectName tulajdonság, amit keresünk.
         */
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    /** Segédfüggvény QML elemek rekurzív megkeresésére.
         * A fentebbi FindItemByName(QObject *rootObject, const QString& name) függvény használja.
         * @param nodes Azon node-ok listája, melyekben (rekurzívan) az adott nevű elemet keressük.
         * @param name  Az objectName tulajdonság, amit keresünk.
         */
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    /** A fő objektum, melynek gyerekei közt találjuk az összes többi objektumot */
    QQuickItem* mainWindowObject;
    /** A használt QML context objektum */
    QQmlContext& qmlContext;
};

#endif // APPLICATIONWINDOW_H

