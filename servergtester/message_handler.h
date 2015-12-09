#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <QObject>

class Message_Handler : public QObject
{
    Q_OBJECT
public:
    explicit Message_Handler(QObject *parent = 0);

signals:

public slots:
};

#endif // MESSAGE_HANDLER_H
