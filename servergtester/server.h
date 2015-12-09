#ifndef SERVER_H
#define SERVER_H
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class server : public QObject
{
    Q_OBJECT

public:
    explicit server(QObject * parent = 0);
private:
    QTcpServer * lisener;
    QTcpSocket * socket;
public slots:
    void newConnection();
};

#endif // SERVER_H
