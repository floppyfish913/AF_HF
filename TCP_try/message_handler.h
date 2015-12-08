#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <QObject>
#include "quadro_msg.h"
class Message_Handler : public QObject
{
public:
    Message_Handler();
    void run();
    void parser(QByteArray data);
    void handler();
    bool SetPID(int Kp,int Kd,int Ki);
    bool GetPID();
    bool SetMainPower(bool Status);
    bool GetState();
private:
    Quadro_msg msg;
public slots:
    void received( QByteArray data);
    void HELLO();
};

#endif // MESSAGE_HANDLER_H
