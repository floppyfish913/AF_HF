#include "tcp_com_thread.h"

TCP_com_thread::TCP_com_thread()
{
    socket = new QTcpSocket();
}
bool TCP_com_thread::connect(){
    socket->connectToHost(Server,port);
    socket->waitForConnected();


    if(socket->isOpen())
    {
        //qDebug() << "Connected";
        this->start();
    }
    else
    {
     // throw -1 ; // Connection failed
       // qDebug() << "Connection failed";
    }

    return true;
}

bool TCP_com_thread::disconnect(){
    this->quit();
    if (!this->isRunning()){

        socket->close();

        socket->waitForDisconnected();
    }
    return true;
}


void TCP_com_thread::run(){

    QString data;
   // QTextStream Console = QTextStream(stdout);
    while (1){
        if(socket->isOpen()){
          if (socket->waitForReadyRead(3000)){
            In_buffer += socket->readAll();
            //QTextStream(stdout) << In_buffer.dequeue();

          }

          if (Out_buffer.count() > 0){
              data = Out_buffer.dequeue();
              socket->write(data.toLocal8Bit());
              socket->flush();
              socket->waitForBytesWritten(3000);
          }

        }
    }
    
}

void TCP_com_thread::send_msg(QString msg){
    Out_buffer.enqueue(msg);
}

QByteArray TCP_com_thread::receive(){
    if(In_buffer.count()>0)
    return In_buffer.dequeue();
    else
        return NULL;
}

int TCP_com_thread::status(){

    if(this->isRunning())
        return 1;
    else
        return 0;

    if(Out_buffer.count() > 0  && In_buffer.count() > 0)
        return 4;

    if(Out_buffer.count() > 0 )
        return 3;

    if(In_buffer.count() > 0)
        return 2;


}

void TCP_com_thread::setPort(int port){
    this->port = port;
}
void TCP_com_thread::setServer(QString server){

    this->Server = server;

}

