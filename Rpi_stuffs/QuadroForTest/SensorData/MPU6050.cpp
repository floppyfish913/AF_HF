#include "MPU6050.h"

#define RESTRICT_PITCH // Comment out to restrict KFData.roll to ±90deg instead
char regaddr[2];

IMU::IMU(void)
{	 

	char buf[1];
	bcm2835_init();
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);
	WriteRegister(SMPRT_DIV,0x07);	// Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
	WriteRegister(CONFIG,0x00); // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
	WriteRegister(GYRO_CONFIG,0x00); //250dpi
	WriteRegister(ACCEL_CONFIG,0x00); //2g resolution
	WriteRegister(PWR_MGMT_1,0x00); //sleep mode disabled

	
	regaddr[0]=WHO_AM_I;
	bcm2835_i2c_write(regaddr, 1);
	bcm2835_i2c_read(buf, 1);
	if(buf[0]==0x71)
	{
		printf("sensor config was successful WHO_AM_I: %x\n",buf[0]);
	}
	else
	{
		printf("sensor config was unsuccessful, %x\n",buf[0]);
	}
	
	bcm2835_i2c_end();
	///////////SETUP VARIABLES

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

void IMU::ReadGyr()
{
	//printf("G:");
	GData.x=ReadRegisterPair(GYRO_XOUT_H);
	GData.y=ReadRegisterPair(GYRO_YOUT_H);
	GData.z=ReadRegisterPair(GYRO_ZOUT_H);
	//printf("Gyr: %f %f %f\n",GData.x,GData.y,GData.z);
	return;

}

void IMU::ReadAccel()
{
	//printf("A:");
	AData.x=ReadRegisterPair(ACCEL_XOUT_H);
	AData.y=ReadRegisterPair(ACCEL_YOUT_H);
	AData.z=ReadRegisterPair(ACCEL_ZOUT_H);
	
	//printf("Acc: %f %f %f\n",AData.x,AData.y,AData.z);
	return;

}

void IMU::PrintDatas()
{

	printf("Gyr: %f %f %f Acc: %f %f %f\n",GData.x,GData.y,GData.z,AData.x,AData.y,AData.z);

}

int ReadRegisterPair(int REG_H)
{
	char buf[1];
	int ret;
	int value = 0;
	

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

	regaddr[0]=REG_H;
	ret = BCM2835_I2C_REASON_ERROR_DATA;
	while(ret != BCM2835_I2C_REASON_OK)
	{
		//This is the basic operation to read an register
		//regaddr[0] is the register address
		//buf[0] is the value
		bcm2835_i2c_write(regaddr, 1);
		ret = bcm2835_i2c_read(buf, 1);
		//printf("%d\n",ret);
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
	bcm2835_i2c_end();
	//printf("%d ",value);
	return value;


}

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
		printf ("KFAngleX:%7.3f \t KFAngleY: %7.3f\n",KFData.x,KFData.y);
	

}

int WriteRegister(int REG,int value)
{

	regaddr[0]=REG;
	regaddr[1]=value;
	return(bcm2835_i2c_write(regaddr, 2));
}


void Timer::StartCycle()
{
	gettimeofday(&t1, NULL);
}


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


double Timer::CountElapsedTime()
{

	gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	return elapsedTime;

}













