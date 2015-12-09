#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <QObject>
#include "quadro_msg.h"
class Message_Handler : public QObject
{
    Q_OBJECT
public:
    explicit Message_Handler(QObject * parent = 0);
    void parser(QByteArray data);
    void handler();
    Quadro_msg msg;

signals:
    void Get_PID_received(Quadro_msg msg);
    void Get_State_received(Quadro_msg msg);
    void Set_PID_received(Quadro_msg msg);
    void Set_Main_Power_received(Quadro_msg msg);
public slots:
    void tester(Quadro_msg msg);


};

#endif // MESSAGE_HANDLER_H
