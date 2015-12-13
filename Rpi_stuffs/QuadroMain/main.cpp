#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "TCP_server.h"
#include "PCA9685.h"


#include <cstdlib>

//#include "PID_v1.h"
#define BCM2835_NO_DELAY_COMPATIBILITY

//global variable

IMU MySensor;
PCA9685 mypwm;


int main (int argc, char** argv)
{
	if (argc != 2) { //checking the validity of the input parameters
    	printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }
   
	int count_dir=0;
	Timer T; //time
	int i=0;
	mypwm.init(1,0x40);
	mypwm.StartMotors();
	//mypwm.setPWM(0,2000);

	MainTCPserver TCPserver(atoi(argv[1])); 
	TCPserver.start_listening();
	
	printf("the main loop has started\n");
    while(1)
    {
		T.StartCycle(); //start counting the time
		MySensor.KalmanFiltering(); //reading IMU datas, and Kalman filtering
		if(i==10)
		{
			TCPserver.report_state(); //send state report
			i=0;
		}	
		
		T.CountElapsedTime(); //stop counting time
		//printf("Time: %f ms motor: %d ",T.elapsedTime,mypwm.MotorStateArray[0]);
		T.WaitMs(dt*1000);//wait 10ms, dt is defined in the IMU library
		
		if(mypwm.GetMainPower())
		{
			if(mypwm.MotorStateArray[0] == 2481)
			{
				count_dir=1;
			}
			if(mypwm.MotorStateArray[0] == 1300)
			{
				count_dir=0;
			}
			

			if(count_dir==0)
			{
				mypwm.setPWM(0,mypwm.MotorStateArray[0]+1);
			}

			if(count_dir==1)
			{
				mypwm.setPWM(0,mypwm.MotorStateArray[0]-1);
			}
		}
	i++;
    } 




	

	
	TCPserver.stop_listening();
}



