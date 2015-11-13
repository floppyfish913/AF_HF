#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "TCP_server.h"
#include "PCA9685.h"
#include <cstdlib>
//#include "PID_v1.h"
#define BCM2835_NO_DELAY_COMPATIBILITY
#define TCP_PORT 10000
#define CLIENT_ADDRESS "192.168.0.115"
#define CLIENT_PORT 10001

//global variable
IMU MySensor;
//PCA9685 PWM;

int main (int atgc, char** argv)
{
   
	Timer T;
	int i=0;
	//PWM.init(1,0x40); //bus=1 address=0x40
	MainTCPserver TCPserver(CLIENT_ADDRESS,TCP_PORT);
	TCPserver.start_listening();
	
    while(1)
    {
		T.StartCycle();
	
		MySensor.KalmanFiltering();
		TCPserver.report_state();
		T.CountElapsedTime();
		//printf("Time: %f ms ",T.elapsedTime);
		T.WaitMs(dt*1000);//wait 10ms
	
		

    } 
	TCPserver.stop_listening();
}



