#ifndef TCP_COM_THREAD_H
#define TCP_COM_THREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QString>
#include <QTcpSocket>
//#include <QDebug>
#include <QByteArray>
class TCP_com_thread : public QThread
{
public:
    TCP_com_thread();
    void setPort(int port);
    void setServer(QString server);
    void run();
    bool connect();
    bool disconnect();
    int status();
    void notEmpty();
    void send_msg(QString msg);
    QByteArray receive();
signals:
    void msg_in_the_box();
private:
    QString Server;
    int port;
    QTcpSocket * socket;
    QQueue<QString> Out_buffer;
    QQueue<QByteArray> In_buffer;

public slots:

};

#endif // TCP_COM_THREAD_H
