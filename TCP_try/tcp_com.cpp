#include "tcp_com.h"

TCP_com::TCP_com(QObject *parent) : QThread(parent)
{
    socket = new QTcpSocket(this);
    MSG_parser = new Message_Handler();

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

}

bool TCP_com::connect_to(){
    socket->connectToHost(Server,port);
    socket->waitForConnected();
    //exec();
    return true;
}

bool TCP_com::disconnect(){

    if (socket->state() == QAbstractSocket::ConnectedState){

        socket->close();

        socket->waitForDisconnected();
    }
    return true;
}

void TCP_com::send_msg(QString msg){
    socket->write(msg.toLocal8Bit());
    socket->flush();
    socket->waitForBytesWritten(3000);
    emit send(msg);
}



void TCP_com::setPort(int port){
    this->port = port;
}
void TCP_com::setServer(QString server){

    this->Server = server;

}


void TCP_com::readyRead(){
    QTextStream(stdout) << "MSG! >>>>>>>>>"<<endl;
    QByteArray data = socket->readAll();
    MSG_parser->parser(data);
    MSG_parser->handler();

}
