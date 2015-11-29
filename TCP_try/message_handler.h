#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <QObject>
#include "tcp_com_thread.h"
class Message_Handler : public QThread
{
public:
    Message_Handler(TCP_com_thread * com_thread);
    void send(QString msg_to_send);
    void run();
private:
    TCP_com_thread * com_thread;
    QByteArray msg;
signals:
public slots:
    void listen();
};

#endif // MESSAGE_HANDLER_H
