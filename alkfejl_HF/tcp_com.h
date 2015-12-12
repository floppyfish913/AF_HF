#ifndef TCP_COM_H
#define TCP_COM_H

#include <QObject>
#include <QQueue>
#include <QString>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QThread>
//#include <QDebug>
#include <QByteArray>
#include "message_handler.h"
#include "quadro_msg.h"

/**
 * @brief Üzenet feldolgozó .
 * 
 * Kommunikációs csatorna 
 * Adatokat feldolgozása Quadro_msg objektumba
 * A jelek bekötésével kommuniál a GUI-val
 */

class TCP_com : public QThread
{
    Q_OBJECT
public:
/**
*Konstruktor
*/
    explicit TCP_com(QObject *parent = 0);
/**
* TCP kapcsolat portjának beállítója
*/
    void setPort(int port);
/**
* TCP kapcsolat IP portjának beállítója
*/
    void setServer(QString server);
/**
* TCP kapcsolódás
*/
    bool connect_to();
/**
* TCP lekapcsolódás
*/
    bool disconnect();
/**
* üzenetkezelő
* üzenetekhez megfelelő eventeket generál
*/
    void handler(Quadro_msg &msg);

private:
    QString Server;
    int port;
    QTcpSocket * socket;
    Message_Handler * MSG_parser;
signals:
    void Get_State_received(Quadro_msg msg);
    void Get_PID_received(Quadro_msg msg);
    void Set_PID_received(Quadro_msg msg);
    void Set_Main_Power_received(Quadro_msg msg);
    void connection_failed();
    void connected();
public slots:
    void readyRead();
    void disconnect_from();
    void send(QString command);
    void connect_to_device(QString, int);
};

#endif // TCP_COM_H
