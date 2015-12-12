#include "tcp_com.h"


/**
*Konstruktor
*Message handler és socket létrehozása
*Socket és a kommunikációs csatorna összekötése
*/
TCP_com::TCP_com(QObject *parent) : QThread(parent)
{
    socket = new QTcpSocket(this);
    MSG_parser = new Message_Handler();

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

}
/**
* Kapcsolódás a szerverhez
*/
bool TCP_com::connect_to(){
    socket->connectToHost(Server,port);
    return socket->waitForConnected(3000);
}

/**
* Kapcsolat lezárása
*/
bool TCP_com::disconnect(){

    if (socket->state() == QAbstractSocket::ConnectedState){

        socket->close();

        socket->waitForDisconnected();
    }
    return true;
}

/**
* Adatok küldése a socketen keresztül
*/

void TCP_com::send(QString msg){
    socket->write(msg.toLocal8Bit());
    socket->flush();
    socket->waitForBytesWritten(3000);
}
/**
* TCP port beállítása
*/
void TCP_com::setPort(int port){
    this->port = port;
}
/**
* TCP Server IP címének beállítása
*/
void TCP_com::setServer(QString server){

    this->Server = server;

}
/**
* Beérkezett üzenetek feldolgozása
*/
void TCP_com::handler(Quadro_msg &msg){

    //caling GUI functions

   // msg.out();
    switch(msg.MessageType){
        case 1: //GetState MSG

            emit Get_State_received(msg);
            break;
        case 2: //GetPID

            emit Get_PID_received(msg);
            break;
        case 3: //SetPID

            emit Set_PID_received(msg);
            break;
        case 4: //SetMainPower

            emit Set_Main_Power_received(msg);
        break;
    }
    msg.clear();
}

/**
* Beérkezett üzenetek feldolgozása
*/
void TCP_com::readyRead(){
    //QTextStream(stdout) << "MSG! >>>>>>>>>"<<endl;
    QByteArray data = socket->readAll();
    MSG_parser->parser(data);
    handler(MSG_parser->msg);

}
/**
* Szétkapcsolás event slot
*/
void TCP_com::disconnect_from(){
   socket->disconnectFromHost();
   socket->waitForDisconnected();
}

/**
* Kapcsolódás eventet figyelő slot
*/
void TCP_com::connect_to_device(QString Server, int port)
{
    setServer(Server);
    setPort(port);
    if (connect_to())
        emit connected();
    else
        emit connection_failed();
}
