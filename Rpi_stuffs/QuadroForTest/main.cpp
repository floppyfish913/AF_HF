#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "TCP_server.h"
#include "PCA9685.h"
#include <cstdlib>
//#include "PID_v1.h"
//#include <wiringPi.h>
#define BCM2835_NO_DELAY_COMPATIBILITY
#define TCP_PORT 10000


//global variable
IMU MySensor;
//PID MotorCtrArray[4];


int main (int atgc, char** argv)
{
   
	Timer T;
	int i=0;
	PCA9685 pwm;
	//pwm.init(1,0x40);
	MainTCPserver TCPserver(TCP_PORT);
	TCPserver.loop_listening();
	


    while(1)
    {
		T.StartCycle();
	
		MySensor.KalmanFiltering();
		T.CountElapsedTime();
		printf("Time: %f ms ",T.elapsedTime);
		T.WaitMs(dt*1000);//wait 10ms
	
		

    } 
	TCPserver.stop_listening();
}



