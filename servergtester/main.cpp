#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "json_server.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTcpServer server;
    QTcpSocket * worker;
    QByteArray data;
    int i=0;
    if(!server.listen(QHostAddress::Any,1234))
        qDebug() << "Something went wong!!" << endl;
    else

        qDebug() << "Server is listening!!!!" <<endl;

    while(1){
        while(!server.waitForNewConnection(10000));

        worker = server.nextPendingConnection();
        qDebug()<< "Someone is conencted!" << endl;


                while(worker->state()==QAbstractSocket::ConnectedState){

                    data = "{\"MessageType\":\"GetState\",\"KalmanXangle\":-16.286,\"KalmanYangle\":3.336,\"Altitude\":0,\"MainPowerStatus\":\"true\"'\"MotorState0\":4001,\"MotorState1\":4002,\"MotorState2\":4003,\"MotorState3\":4004}";
                    worker->write(data);
                    QTextStream(stdout) << "MSG Sent!"<<endl;
                    worker->flush();
                    worker->waitForBytesWritten(3000);
QThread::msleep(150);
                    data = "{\"MessageType\":\"SetMainPower\",\"MainPowerStatus\":\"true\"}";
                    worker->write(data);
                    QTextStream(stdout) << "MSG Sent!"<<endl;
                    worker->flush();
                    worker->waitForBytesWritten(3000);
QThread::msleep(150);
                    data = "{\"MessageType\":\"SetPID\",\"SetPIDSuccess\":\"true\"}";
                    worker->write(data);
                    QTextStream(stdout) << "MSG Sent!"<<endl;
                    worker->flush();
                    worker->waitForBytesWritten(3000);
QThread::msleep(150);
                    data = "{\"MessageType\":\"GetPID\",\"Kp\":4,\"Ki\":12,\"Kd\":44}";
                    worker->write(data);
                    QTextStream(stdout) << "MSG Sent!"<<endl;
                    worker->flush();
                    worker->waitForBytesWritten(3000);


                    QThread::msleep(150);
                    worker->waitForReadyRead(3000);
                    QTextStream(stdout) << worker->readAll()<<endl;

                }

        qDebug() << "Disconnected"<<endl;
        worker->close();
    }
    qDebug() << "Everything closed";
    QThread::msleep(10000);
    return a.exec();
}

