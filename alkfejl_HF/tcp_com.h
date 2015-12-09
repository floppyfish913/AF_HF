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


class TCP_com : public QThread
{
    Q_OBJECT
public:
    explicit TCP_com(QObject *parent = 0);
    void setPort(int port);
    void setServer(QString server);

    bool connect_to();
    bool disconnect();
    void handler(Quadro_msg &msg);
    int status();

    void send_msg(QString msg);
    QByteArray receive();

private:
    QString Server;
    int port;
    QTcpSocket * socket;
    Message_Handler * MSG_parser;
signals:
    Get_State_received(Quadro_msg msg);
    Get_PID_received(Quadro_msg msg);
    Set_PID_received(Quadro_msg msg);
    Set_Main_Power_received(Quadro_msg msg);
public slots:
    void readyRead();
    void send(QString command);
};

#endif // TCP_COM_H
