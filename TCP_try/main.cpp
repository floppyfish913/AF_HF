#include <QCoreApplication>
//#include <QDebug>
#include "tcp_com_thread.h"
#include "message_handler.h"
int main(int argc, char *argv[])
{
    //qDebug() << "App Started";

    QCoreApplication a(argc, argv);
    TCP_com_thread Connection;
    Connection.setPort(1234);
    Connection.setServer("localhost");
    Connection.connect();
    Message_Handler * parser = new Message_Handler(&Connection);
    parser->run();

    int i=0;
    while(1){
       if(i++ < 20)
           Connection.send_msg("{\"MessageType\": \"GetPID\",\"Kp\": 4,\"Ki\": 12,\"Kd\": 44}\r\n");

       // qDebug() << Connection.receive()<<endl;
    }

    return a.exec();
}

