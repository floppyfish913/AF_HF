#include "message_handler.h"


Message_Handler::Message_Handler(QObject * parent) : QObject(parent)
{
    connect(this,SIGNAL(Get_State_received(Quadro_msg)),this,SLOT(tester(Quadro_msg)));
    connect(this,SIGNAL(Get_PID_received(Quadro_msg)),this,SLOT(tester(Quadro_msg)));
    connect(this,SIGNAL(Set_Main_Power_received(Quadro_msg)),this,SLOT(tester(Quadro_msg)));
    connect(this,SIGNAL(Set_PID_received(Quadro_msg)),this,SLOT(tester(Quadro_msg)));
}

void Message_Handler::parser(QByteArray data){
    QByteArray tmp;
    int digit = 1;
    int i=0;
    int sign=0;

    if(data.startsWith('{')){
        while(data.count() > 0){
            data.remove(0,data.indexOf('"')+1);
            tmp = data.left(data.indexOf('"'));
            data.remove(0,data.indexOf('"')+1);
            if(QString(tmp)=="MessageType"){
                data.remove(0,data.indexOf(':')+2);
                tmp = data.left(data.indexOf('"'));
                data.remove(0,data.indexOf(',')+1);
                if(QString(tmp)=="GetState")
                    msg.MessageType = 1;
                else if(QString(tmp)=="GetPID")
                    msg.MessageType = 2;
                else if(QString(tmp)=="SetPID")
                    msg.MessageType = 3;
                else if(QString(tmp)=="SetMainPower")
                    msg.MessageType = 4;
                else
                    msg.MessageType = 0;
            }
            else if(QString(tmp)=="KalmanXangle"){
                data.remove(0,data.indexOf(':')+1);
                msg.KalmanX = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.KalmanX*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.KalmanX += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
                msg.KalmanX /= 1000;
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="KalmanYangle"){
                data.remove(0,data.indexOf(':')+1);
                msg.KalmanY = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.KalmanY*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.KalmanY += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
                msg.KalmanY /= 1000;
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="Altitude"){
                data.remove(0,data.indexOf(':')+1);
                msg.Altitude = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.Altitude*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.Altitude += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
                msg.Altitude /= 1000;
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="Ki"){
                data.remove(0,data.indexOf(':')+1);
                msg.Ki = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.Ki*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.Ki += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="Kd"){
                data.remove(0,data.indexOf(':')+1);
                msg.Kd = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.Kd*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.Kd += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="Kp"){
                data.remove(0,data.indexOf(':')+1);
                msg.Kp = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.Kp*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.Kp += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="MotorState0"){
                data.remove(0,data.indexOf(':')+1);
                msg.MotorState0 = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.MotorState0*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.MotorState0 += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="MotorState1"){
                data.remove(0,data.indexOf(':')+1);
                msg.MotorState1 = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.MotorState1*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.MotorState1 += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="MotorState2"){
                data.remove(0,data.indexOf(':')+1);
                msg.MotorState2 = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.MotorState2*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.MotorState2 += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="MotorState3"){
                data.remove(0,data.indexOf(':')+1);
                msg.MotorState3 = 0;
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                digit = 1;
                for(i= sign;i>=0;i--)
                {
                    if(data[i]=='-') msg.MotorState3*=-1;
                    if(data[i] <='9' && data[i] >= '0')
                    {
                        msg.MotorState3 += digit * (data[i]-0x30);
                        digit *= 10;
                    }

                }
              data.remove(0,sign+1);
            }
            else if(QString(tmp)=="MainPowerStatus"){
                data.remove(0,data.indexOf(':')+2);
                tmp = data.left(data.indexOf('"'));
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                data.remove(0,sign+1);
                if(QString(tmp)=="true")
                    msg.MainPowerStatus = true;
                else if(QString(tmp)=="false")
                    msg.MainPowerStatus = false;
            }
            else if(QString(tmp)=="SetPIDSuccess"){
                data.remove(0,data.indexOf(':')+2);
                tmp = data.left(data.indexOf('"'));
                sign = data.indexOf(',');
                if(sign == -1) sign = data.indexOf('}');
                data.remove(0,sign+1);
                if(QString(tmp)=="true")
                    msg.SetPIDSuccess = true;
                else if(QString(tmp)=="false")
                    msg.SetPIDSuccess = false;
            }

        }







    }


}

void Message_Handler::handler(){

    //caling GUI functions

    msg.out();
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

void Message_Handler::tester(Quadro_msg msg){

     QTextStream(stdout) << "MessageType : " << msg.MessageType << endl;

}
