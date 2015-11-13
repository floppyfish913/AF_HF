#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include "Kalman.h"


#define MPU6050_ADDRESS 0x68
#define SMPRT_DIV 0x19 //Sample Rate Divider
#define CONFIG 0x1A //config external sync
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H ((int)0x3B)
#define ACCEL_XOUT_L ((int)0x3C)
#define ACCEL_YOUT_H ((int)0x3D)
#define ACCEL_YOUT_L ((int)0x3E)
#define ACCEL_ZOUT_H ((int)0x3F)
#define ACCEL_ZOUT_L ((int)0x40)
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H ((int)0x43)
#define GYRO_XOUT_L ((int)0x44)
#define GYRO_YOUT_H ((int)0x45)
#define GYRO_YOUT_L ((int)0x46)
#define GYRO_ZOUT_H ((int)0x47)
#define GYRO_ZOUT_L ((int)0x48)
#define PWR_MGMT_1 0x6B
#define WHO_AM_I 0x75


#define ACCELEROMETER_SENSITIVITY 8192.0 // +/- 2g
#define GYROSCOPE_SENSITIVITY 131 //250dpi LSB/deg/s
#define dt 0.01
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846
#define DT 0.01

int ReadRegisterPair(int REG_H);
int WriteRegister(int REG,int value);


class IMU
{
    
        typedef struct SensorData
        {
          float x;
          float y;
          float z;
        } RawData;

	    typedef struct KalmanFilter_datas
        {
		  float x;
          float y;
	 	  float roll;
	  	  float pitch;
        } KFilterData;
		
	private:
	//////////VARIABLES
	Kalman kalmanX; // Create the Kalman instances
	Kalman kalmanY;

    public:

	//variable
	KFilterData KFData;
	RawData GData;
	RawData AData;
    int Altitude;
	//constructor
    IMU(void);

    //functions
	void PrintDatas();
    void ReadGyr();
	void ReadAccel();
    void KalmanFiltering();
};


class Timer
{
	private:
		struct timeval t1;
		struct timeval t2;

	public:
		double elapsedTime;
		void StartCycle();
		double CountElapsedTime();
		int WaitMs(int ms);

};
