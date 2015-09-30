#include "i2c-dev.h"
#include "LSM303.h"
#include "L3G.h"
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "sensor.cpp"
#include "tcp_client.cpp"





#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846

void  INThandler(int sig);

int main()
{
	


	tcp_client log("192.168.0.115",10000);
	char* buf;
	int  acc_raw[3];
	int  mag_raw[3];
	int  gyr_raw[3];
	float rate_gyr_y = 0.0;   // [deg/s]
	float rate_gyr_x = 0.0;    // [deg/s]
	float rate_gyr_z = 0.0;     // [deg/s]

	float gyroXangle = 0.0;
	float gyroYangle = 0.0;
	float gyroZangle = 0.0;
	float AccYangle = 0.0;
	float AccXangle = 0.0;
	float CFangleX = 0.0;
	float CFangleY = 0.0;

	struct  timeval tvBegin, tvEnd,tvDiff;
	
	signal(SIGINT, INThandler);
	enableIMU();

while(1)
	{

		
		
		readACC(acc_raw);
		readGYR(gyr_raw);

		//Convert Gyro raw to degrees per second
		rate_gyr_x = (float) gyr_raw[0] * G_GAIN;
		rate_gyr_y = (float) gyr_raw[1]  * G_GAIN;
		rate_gyr_z = (float) gyr_raw[2]  * G_GAIN;

		//Calculate the angles from the gyro
		gyroXangle+=rate_gyr_x*DT;
		gyroYangle+=rate_gyr_y*DT;
		gyroZangle+=rate_gyr_z*DT;

		//Convert Accelerometer values to degrees
		AccXangle = (float) (atan2(acc_raw[1],acc_raw[2])+M_PI)*RAD_TO_DEG;
		AccYangle = (float) (atan2(acc_raw[2],acc_raw[0])+M_PI)*RAD_TO_DEG;


		//Change the rotation value of the accelerometer to -/+ 180
		if (AccXangle >180)
		{
			AccXangle -= (float)360.0;
		}
		if (AccYangle >180)
			AccYangle -= (float)360.0;

		//Complementary filter used to combine the accelerometer and gyro values.
		CFangleX=AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
		CFangleY=AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;

		char message[]="";
		char buff[100];

		sprintf(buff,"{\"accelerometer\":");
		strncat (message, buff,999);

		sprintf(buff,"{\"AccXangle\":\"%f\",",AccXangle);
		strncat (message, buff,999);
		
		sprintf(buff,"\"AccYangle\":\"%f\" },",AccYangle);
		strncat (message, buff,999);

		sprintf(buff,"\"gyroscope\":");
		strncat (message, buff,999);

		sprintf(buff,"{\"gyroXangle\":\"%7.3f\",",gyroXangle);
		strncat (message, buff,999);

		sprintf(buff,"\"gyroYangle\":\"%7.3f\",",gyroYangle);
		strncat (message, buff,999);

		sprintf(buff,"\"gyroZangle\":\"%7.3f\" },",gyroZangle);
		strncat (message, buff,999);

		sprintf(buff,"\"Compl_filt_angl\":");
		strncat (message, buff,999);

		sprintf(buff,"{\"CFangleX\":\"%7.3f\",",CFangleX);
		strncat (message, buff,999);
		
		sprintf(buff,"\"CFangleY\":\"%7.3f\" }}",CFangleY);
		strncat (message, buff,999);
		

		printf("\n%s\n",message);
		log.send_package(message);
		//Each loop should be at least 20ms.
 	       	for(int i=0;i!=200;i++)
		{
	            	usleep(100);
		}

	}

}


void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
        exit(0);
}



































