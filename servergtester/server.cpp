#include "server.h"

server::server(QObject * parent) : QObject(parent)
{
    lisener = new QTcpServer(this);

    connect(lisener,SIGNAL(newConnection()),this,SLOT( newConnection()));

    lisener->listen(QHostAddress::Any,1234);

    if(lisener->isListening()){
        qDebug () << "Server is listening!!" << endl;

    }
    else {

        qDebug () << "Something went wrong!!" << endl;

    }

}

void server:: newConnection(){
    socket = lisener->nextPendingConnection();

    socket->write("It works well!");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();


}
