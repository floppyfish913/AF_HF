#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "MPU6050.h"
#include "TCP_server.h"

#define TCP_PORT 10000

int main (int atgc, char** argv)
{
    
    IMU MySensor;
	Timer T;
	int i=0;
	MainTCPserver TCPserver(TCP_PORT);
	//pthread_t tcp_listener;

    while(1)
    {
		T.StartCycle();
		TCPserver.loop_listening();
		MySensor.KalmanFiltering();
		T.CountElapsedTime();
		printf("Time: %f ms ",T.elapsedTime);
		T.WaitMs(dt*1000);//wait 10ms
		TCPserver.stop_listening();
		

    } 

}

