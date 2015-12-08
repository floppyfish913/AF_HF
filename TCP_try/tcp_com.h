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

    int status();

    void send_msg(QString msg);
    QByteArray receive();
signals:

    void send(QString command);
private:
    QString Server;
    int port;
    QTcpSocket * socket;
    Message_Handler * MSG_parser;

public slots:
    void readyRead();

};

#endif // TCP_COM_H
