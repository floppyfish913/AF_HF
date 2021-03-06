#ifndef QUADRO_MSG_H
#define QUADRO_MSG_H

#include <QObject>
#include <QString>
#include <QTextStream>

/**
 * @brief A robot szimulátor üzenet .
 * 
 * Tartalmazza az összes mezőt, ami az üzenetekben található.
 * A továbbiakban a beérkezett JSON formátumú üzeneteket a parser egy ilyen objektumba írja
 * Valamint a GUI és a kommunikációs réteg között az adatok cseréje "ezzel történik"
 */


class Quadro_msg : public QObject
{
    Q_OBJECT
public:
    explicit Quadro_msg(QObject *parent = 0);
    Quadro_msg(const Quadro_msg& other);
    int MessageType;
    double KalmanX;
    double KalmanY;
    double Altitude;
    bool MainPowerStatus;
    int MotorState0;
    int MotorState1;
    int MotorState2;
    int MotorState3;
    int Kp;
    int Ki;
    int Kd;
    bool SetPIDSuccess;
    void clear();
    void out();
signals:

public slots:
};

#endif // QUADRO_MSG_H
