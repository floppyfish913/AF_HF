#include "json_server.h"

json_server::json_server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer();

    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));



    if(!server->listen(QHostAddress::Any,8000))
        qDebug() << "Something went wong!!" << endl;
    else

        qDebug() << "Server is listening!!!!" <<endl;
}

void json_server:: newConnection(){
    QTcpSocket * worker = server->nextPendingConnection();

    worker->write("It works well!");
    worker->flush();

    worker->waitForBytesWritten(3000);

    worker->close();


}
