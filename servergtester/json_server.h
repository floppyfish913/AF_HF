#ifndef JSON_SERVER_H
#define JSON_SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QObject>

class json_server : public QObject
{
    Q_OBJECT
public:
    explicit json_server(QObject *parent = 0);

signals:

public slots:
    void newConnection();
private:
    QTcpServer * server;


};

#endif // JSON_SERVER_H
