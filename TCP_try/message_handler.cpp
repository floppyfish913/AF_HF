#include "message_handler.h"

Message_Handler::Message_Handler(TCP_com_thread *com_thread)
{
    this->com_thread = com_thread;
}

void Message_Handler::send(QString msg_to_send){
    com_thread->send_msg(msg_to_send);
}

void Message_Handler::run(){
    QByteArray data;
    while(1){
    data = com_thread->receive();

    }



}
