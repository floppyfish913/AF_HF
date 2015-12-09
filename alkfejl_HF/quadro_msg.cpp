#include "quadro_msg.h"

Quadro_msg::Quadro_msg(QObject *parent) : QObject(parent)
{
   this->clear();
}

Quadro_msg::Quadro_msg(const Quadro_msg& other){
    MessageType = other.MessageType;
    KalmanX = other.KalmanX;
    KalmanY=other.KalmanY;
    Altitude = other.Altitude;
    MainPowerStatus = other.MainPowerStatus;
    MotorState0 = other.MotorState0;
    MotorState1 = other.MotorState1;
    MotorState2 = other.MotorState2;
    MotorState3 = other.MotorState2;
    Kp = other.Kp;
    Ki = other.Ki;
    Kd = other.Kd;
    SetPIDSuccess = other.SetPIDSuccess;

}
void Quadro_msg::clear()
{
    MessageType = 0;
    KalmanX = 0.0;
    KalmanY=0.0;
    Altitude = 0;
    MainPowerStatus = false;
    MotorState0 = 0;
    MotorState1 = 0;
    MotorState2 = 0;
    MotorState3 = 0;
    Kp = 0;
    Ki = 0;
    Kd = 0;
    SetPIDSuccess = false;
}

void Quadro_msg::out(){
    QString full="";
    if (MessageType){
        full += "MessageType : " + QString::number(this->MessageType) + "\r\n";
        full += "KalmanX : " + QString::number(this->KalmanX) + "\r\n";
        full += "KalmanY : " + QString::number(this->KalmanY) + "\r\n";
        full += "Altitude : " + QString::number(this->Altitude) + "\r\n";
        full += "MainPowerStatus : " + QString::number(this->MainPowerStatus) + "\r\n";
        full += "MotorState0 : " + QString::number(this->MotorState0) + "\r\n";
        full += "MotorState1 : " + QString::number(this->MotorState1) + "\r\n";
        full += "MotorState2 : " + QString::number(this->MotorState2) + "\r\n";
        full += "MotorState3 : " + QString::number(this->MotorState3) + "\r\n";
        full += "Kp : " + QString::number(this->Kp) + "\r\n";
        full += "Kd : " + QString::number(this->Kd) + "\r\n";
        full += "Ki : " + QString::number(this->Ki) + "\r\n";
        full += "SetPIDSuccess : " + QString::number(this->SetPIDSuccess) + "\r\n";
        QTextStream(stdout) <<  full;

    }
}
