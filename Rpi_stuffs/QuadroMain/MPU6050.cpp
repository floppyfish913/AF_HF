#include "MPU6050.h"

#define RESTRICT_PITCH // Comment out to restrict KFData.roll to ±90deg instead



/**

constructor function of the IMU class

this library is designed to an MPU9250 or MPU6050 or any compatible IMU

1,identify the module
2,it sets the detailed registers
3,reads the raw gyroscope, and acceleromter data from registers - saves to the structures
4,kalman filters the raw data, and saves the data

*/
IMU::IMU(void)
{	 

	char buf[1];
	char regaddr[2];	

	bcm2835_init(); //setting the memory address
	bcm2835_i2c_begin(); //enabling the I2C ports
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS); //setting the device addresss
	
	
	
	


	// validating IMU, if success, having the first measure
	regaddr[0]=WHO_AM_I;
	bcm2835_i2c_write(regaddr, 1);
	buf[0]=0;
	bcm2835_i2c_read(buf, 1);
	if(buf[0]==0x71)
	{
		WriteIMURegister(SMPRT_DIV,0x07);	// Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
		WriteIMURegister(CONFIG,0x00); // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
		WriteIMURegister(GYRO_CONFIG,0x00); //250dpi
		WriteIMURegister(ACCEL_CONFIG,0x00); //2g resolution
		WriteIMURegister(PWR_MGMT_1,0x00); //sleep mode disabled
		//for further informatin please check the data sheet of the IMU
		printf("sensor config was successful WHO_AM_I: %x\n",buf[0]);
		
		
		ReadGyr();
		ReadAccel();
		#ifdef RESTRICT_PITCH // Eq. 25 and 26
	  	KFData.roll  = atan2(AData.y, AData.z) * RAD_TO_DEG;
	 	KFData.pitch = atan(-AData.x / sqrt(AData.y * AData.y + AData.z * AData.z)) * RAD_TO_DEG;
		#else // Eq. 28 and 29
	 	 KFData.roll  = atan(AData.y / sqrt(AData.x * AData.x + AData.z * AData.z)) * RAD_TO_DEG;
		  KFData.pitch = atan2(-AData.x, AData.z) * RAD_TO_DEG;
		#endif

	kalmanX.setAngle(KFData.roll); // Set starting angle
  	kalmanY.setAngle(KFData.pitch);
	}
	else
	{
		printf("sensor config was unsuccessful, %x\n",buf[0]);
	}
	}

/**
reads the relevant register pairs of the gyroscope, and loads them into the structure variable in the class

*/

void IMU::ReadGyr()
{
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);
	//printf("G:");
	GData.x=ReadRegisterPair(GYRO_XOUT_H);
	GData.y=ReadRegisterPair(GYRO_YOUT_H);
	GData.z=ReadRegisterPair(GYRO_ZOUT_H);
	//printf("Gyr: %f %f %f\n",GData.x,GData.y,GData.z);
	return;

}

/**
reads the relevant register pairs of the accelerometer, and loads them into the structure variable in the class

*/

void IMU::ReadAccel()
{
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);
	//printf("A:");
	AData.x=ReadRegisterPair(ACCEL_XOUT_H);
	AData.y=ReadRegisterPair(ACCEL_YOUT_H);
	AData.z=ReadRegisterPair(ACCEL_ZOUT_H);	
	//printf("Acc: %f %f %f\n",AData.x,AData.y,AData.z);
	return;

}

/**
it is just a debug function

*/

void IMU::PrintDatas()
{

	printf("Gyr: %f %f %f Acc: %f %f %f\n",GData.x,GData.y,GData.z,AData.x,AData.y,AData.z);

}


/**
This function calls the register reads to get the gyroscope, and accelerometer datas, and analyzes the readed datas
The method to minimalize the drifft, of the data, and get a better orientation data is called to Kalman filtering

To process the readed gyroscope data you need a known sampling time, it in this program it is set to 100 Hz, to set the time
check the Timer class


As a template i used this example, for further information, please check this link:
https://github.com/TKJElectronics/KalmanFilter/blob/master/examples/MPU6050/MPU6050.ino
*/

void IMU::KalmanFiltering() 
{
		
		//reading new datas
		ReadGyr();
		ReadAccel();
		
		     
        #ifdef RESTRICT_PITCH // Eq. 25 and 26
			KFData.roll  = atan2(AData.y, AData.z) * RAD_TO_DEG;
			KFData.pitch = atan(-AData.x / sqrt(AData.y * AData.y + AData.z * AData.z)) * RAD_TO_DEG;
		#else // Eq. 28 and 29
			KFData.roll  = atan(AData.y / sqrt(AData.x * AData.x + AData.z * AData.z)) * RAD_TO_DEG;
			KFData.pitch = atan2(-AData.x, AData.z) * RAD_TO_DEG;
		#endif
        
		double gyroXrate = GData.x / GYROSCOPE_SENSITIVITY; // 131- Convert to deg/s
  		double gyroYrate = GData.y / GYROSCOPE_SENSITIVITY; // 131- Convert to deg/s

		#ifdef RESTRICT_PITCH
		  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
		  if ((KFData.roll < -90 && KFData.x > 90) || (KFData.roll > 90 && KFData.x < -90)) {
			kalmanX.setAngle(KFData.roll);
			KFData.x = KFData.roll;
		  } else
			KFData.x = kalmanX.getAngle(KFData.roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

		  if (abs(KFData.x) > 90)
			gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
		  KFData.y = kalmanY.getAngle(KFData.pitch, gyroYrate, dt);
		#else
		  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
		  if ((KFData.pitch < -90 && KFData.y > 90) || (KFData.pitch > 90 && KFData.y < -90)) {
			kalmanY.setAngle(KFData.pitch);
			KFData.y = KFData.pitch;
		  } else
			KFData.y = kalmanY.getAngle(KFData.pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

		  if (abs(KFData.y) > 90)
			gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
		  KFData.x = kalmanX.getAngle(KFData.roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
		#endif
        
		//printf("G: %7.3f %7.3f %7.3f A: %7.3f %7.3f %7.3f KF: %7.3f %7.3f R-P: %7.3f %7.3f\n",GData.x,GData.y,GData.z,AData.x,AData.y,AData.z,KFData.x,KFData.y,KFData.roll,KFData.pitch);
		//printf ("KFAngleX:%7.3f \t KFAngleY: %7.3f\n",KFData.x,KFData.y);
	

}


/**
get the actual time, and it is going to be used as an origo at measuring the elapsed time

*/
void Timer::StartCycle()
{
	gettimeofday(&t1, NULL);
}

/**
waiting a given amount of time, the elapsed time is compared to time what was set in "StartCycle"

*/

int Timer::WaitMs(int ms) 
{
	elapsedTime=0;
	int i=0;
	while(elapsedTime < ms)
        {
            gettimeofday(&t2, NULL);
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
            usleep(5);
	    	i++;
            //wait(t1);
        }
	if(i==1)//it means it had just 1 round in 1 loop, so the cycle was too long
	{
		//printf("ERROR: cycle was too long, time:  %d\n, elapsedTime");
		return 1;
	}
	else
		return 0;

}


/**

	gives back the elapsed time betwen t1 and the actual moment

*/


double Timer::CountElapsedTime()
{

	gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	return elapsedTime;

}



/**
it just simply reads two registers in a row from the I2C bus
the given register address is the first
*/

int ReadRegisterPair(int REG_H)
{
	char buf[1];
	int ret;
	int value = 0;

	char regaddr[2];
	regaddr[0]=REG_H;
	ret = BCM2835_I2C_REASON_ERROR_DATA;
	while(ret != BCM2835_I2C_REASON_OK)
	{
		bcm2835_i2c_write(regaddr, 1);
		ret = bcm2835_i2c_read(buf, 1);		
	}
	value = buf[0]<<8;
	regaddr[0]=(REG_H+1);
	
    	ret = BCM2835_I2C_REASON_ERROR_DATA;
    	while(ret != BCM2835_I2C_REASON_OK)
	{
		bcm2835_i2c_write(regaddr, 1);
		ret = bcm2835_i2c_read(buf, 1);
	}
	value += buf[0];
	if (value & 1<<15)
   	{
        	value -= 1<<16;
   	}
	//printf("%d ",value);
	return value;


}


/**
it writes a given value into a given register on I2C bus

*/

int WriteIMURegister(int REG,int value)
{
	char regaddr[2];
	regaddr[0]=REG;
	regaddr[1]=value;
	return(bcm2835_i2c_write(regaddr, 2));
}








