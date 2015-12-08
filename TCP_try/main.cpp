#include <QCoreApplication>
//#include <QDebug>
#include "tcp_com.h"
#include "message_handler.h"
int main(int argc, char *argv[])
{
    //qDebug() << "App Started";

    QCoreApplication a(argc, argv);
    TCP_com  Connection;
    Connection.setPort(1234);
   // Connection.setServer("192.168.0.5");
    Connection.setServer("localhost");

    Connection.connect_to();


    int i=0;
    while(1){
       if(i++ < 50)
           //Connection.send_msg("{\"MessageType\": \"GetPID\",\"Kp\": 4,\"Ki\": 12,\"Kd\": 44}\r\n");
            Connection.send_msg("Alive!");
       // qDebug() << Connection.receive()<<endl;
    }

    return a.exec();
}

