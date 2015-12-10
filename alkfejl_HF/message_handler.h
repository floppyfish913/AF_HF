#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <QObject>
#include "quadro_msg.h"
class Message_Handler : public QObject
{
    Q_OBJECT
public:
    explicit Message_Handler(QObject * parent = 0);
	/** JSON parser
     * @param data bejövő JSON adatcsomag.
     */
    void parser(QByteArray data);
    Quadro_msg msg;
};

#endif // MESSAGE_HANDLER_H
