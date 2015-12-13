#include "TCP_server.h"

using namespace rapidjson;

int Kp=1;
int Ki=2;
int Kd=3;
/**
these variables are defined in the main.cpp 
*/
extern IMU MySensor; 
extern PCA9685 mypwm;

/**
it creates a tcp lisener server, what is gonna listen to the incomming datas from the client application
for further information about it please check: http://www.paulgriffiths.net/program/c/echoserv.php

@port the listernet port where the server will listen to the connecting client
*/

MainTCPserver::MainTCPserver(int port)
{
	if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
	{
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}
	comm_fd=0;
	bzero( &servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); //any address
    servaddr.sin_port = htons(port); //given port

	//setting server socket
	if(bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		fprintf(stderr, "ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);		
	}
	if ( (listen(listen_fd, 10)) < 0 )
	{
		fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
	}
	printf("TCP server config was successful on port: %d, it is waiting for commands, and it will send the state reports to the connected client\n",port);
}



/**

this loop runs in the background and listens to connecting client

the function is called when the new thread is created, 


-the read_rx function waits to the connecting clients, and creates the connection, and puts the incoming package to the RX buffer
-the proc_rx is a state machine what processes the string from the RX buffer, executes the required operation, and sends back the answer 

@arg a pointer what points on the class whats member is this function

*/
void* MainTCPserver::call_member_function(void *arg)
{ 

	while(1)
	{
		((MainTCPserver*)arg)->read_rx(); //listening, and reading from the socket
		((MainTCPserver*)arg)->proc_rx(); //processing the incoming string
	}
}


/**
receiving the input string from socket, and read it into the RX buffer of the server


no parameter
*/
void* MainTCPserver::read_rx()
{
	
	bzero(RX_buffer,300);
	if(comm_fd==0)
	{
		if ( (comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL))< 0 )
		{
			fprintf(stderr, "ECHOSERV: Error calling accept()\n");
			exit(EXIT_FAILURE);
		}
	}
	read(comm_fd,RX_buffer,299);
	//printf("Recived: %s - over\n",RX_buffer);
	
	
}

/**
creating a new thread and, starting linstening in the background, and it is gonna what for the incomming packages


this solution maybe seems weird, for further information check the description of phtread 
*/

int MainTCPserver::start_listening(void) 
{
	int i=0;
	if( pthread_create(&this->tcp_listener , NULL ,MainTCPserver::call_member_function ,this) < 0)
    {
            printf("could not create thread");
            return 1;
    }
	else
	{
		printf("Starting new thread was successful\n");
	}
	return 0;

}

/**
stop listening, and closeing the thread

*/

int MainTCPserver::stop_listening(void)
{
	pthread_cancel(this->tcp_listener);
	pthread_detach(this->tcp_listener);

}


/**

converting input str to JSON obj, and processing the input

the passible message types are:
-GetState - the raspberry sends back the debug informations, (motor state, angle, gyroscope, accelerometer data, power switch state)
-GetPID - the raspberry sends back the PID controller's parameters
-SetPID - the raspberry refreshes the PID controller's paramters according to the incoming JSON string
-SetMainPower - the raspberry refreshes 


*/

void* MainTCPserver::proc_rx(void)
{
	if(strlen(RX_buffer)<1)
	{
		
		return 0;
	}
	bzero(TX_buffer,300);
	Document InputJSON;
	int len;
	//sprintf(RX_buffer,"{\"MessageType\":\"SetPID\"}");
	if(InputJSON.Parse<0>(RX_buffer).HasParseError()) //convert string to JSON obj
	{
		printf("ERROR in parsing JSON str\n");
	}
	
	if(InputJSON["MessageType"] == "GetState")
	{
		printf("GetState message detected\n");
		create_getstate_json();
	}
	else if(InputJSON["MessageType"] == "GetPID")
	{
		printf("GetPID message detected\n");
		create_getpid_json();
	}
	else if(InputJSON["MessageType"] == "SetPID")
	{
		printf("SetPID message detected\n");
		
		assert(InputJSON["Kp"].IsNumber());
		assert(InputJSON["Ki"].IsNumber());
		assert(InputJSON["Kd"].IsNumber());
		
		Kp=InputJSON["Kp"].GetInt();
		Ki=InputJSON["Ki"].GetInt();
		Kd=InputJSON["Kd"].GetInt();
		create_setpid_json(true);

	}
	else if(InputJSON["MessageType"] == "SetMainPower")
	{
		printf("SetMainPower message detected\n");
		if(InputJSON["MainPowerStatus"] == "true")//turn on the motors
		{
			printf("Set power to high is detected!\n");
			if(mypwm.GetMainPower() == false)
			{
				mypwm.SetMainPower(true);
				//mypwm.StartMotors();
			}
			printf("it was high already!!!\n");
		}
		else if(InputJSON["MainPowerStatus"] == "false")
		{
			printf("Set power to low is detected!\n");
			mypwm.SetMainPower(false);
			printf("1\n");
			mypwm.setPWM(0,1240);
			printf("2\n");

		}
		create_setmainpower_json(mypwm.GetMainPower());
	}
	else
	{
		sprintf(TX_buffer,"{\"MessageType\":\"ERROR\"}");
	}
	write(comm_fd,TX_buffer,strlen(TX_buffer)); //sending the content of TX_buffer to the connected client
	printf("kaka");

}
/**

this function creates the JSON string about the state of the system what includes
-raw gyroscope data
-raw accelerometer data
-Kalman filtered orientation data
-altitude
-main power switch state
-PWM signals on the motors

It loads the JSON string into the TX_buffer

*/


void MainTCPserver::create_getstate_json() //NOT READY
{
	
	//int MotorState[]={4001,4002,4003,4004};
	char MainPowerState[6];
	MySensor.Altitude=23;
	if(mypwm.GetMainPower())
	{
		sprintf(MainPowerState,"true");
	}
	else
	{
		sprintf(MainPowerState,"false");
	}
	
        sprintf(TX_buffer,"{\"MessageType\":\"GetState\",\"GyroX\":%7.3f,\"GyroY\":%7.3f,\"GyroZ\":%7.3f,\"AccelX\":%7.3f,\"AccelY\":%7.3f,\"AccelZ\":%7.3f,\"KalmanXangle\":%7.3f,\"KalmanYangle\": %7.3f,\"Altitude\": %d,\"MainPowerState\": \"%s\",\"MotorState0\": %d,\"MotorState1\":%d,\"MotorState2\":%d,\"MotorState3\":%d}",MySensor.GData.x,MySensor.GData.y,MySensor.GData.z,MySensor.AData.x,MySensor.AData.y,MySensor.AData.z,MySensor.KFData.x,MySensor.KFData.y,MySensor.Altitude,MainPowerState,mypwm.MotorStateArray[0],mypwm.MotorStateArray[1],mypwm.MotorStateArray[2],mypwm.MotorStateArray[3]);

}


/**
send the state of the system, if it has any connected peer

It calls the create_getstate_json function to create the JSON string about the system's state and load into TX_buffer
Writes the contain of TX_buffer into the connected socket

*/

void MainTCPserver::report_state()
{
	
	
	if(listen_fd!=-1)//it means it connected succesfuly
	{	
		create_getstate_json();
		write(comm_fd, TX_buffer, strlen(TX_buffer));
		//printf("msg sent, motor: %d\n",mypwm.MotorStateArray[0]);
	}
	bzero(TX_buffer,300);
	

}

/**
creates the string about the parameters of the PID contoller and loads it into the TX_buffer

*/


void MainTCPserver::create_getpid_json() //NOT READY-test mode
{
	sprintf(TX_buffer,"{\"MessageType\":\"GetPID\",\"Kp\":%d,\"Ki\":%d,\"Kd\":%d}",Kp,Ki,Kd);
}

/**
creates the feedback string about modifying the PID controller's parameters

*/

void MainTCPserver::create_setpid_json(bool SetPid)
{
	if(SetPid)
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetPID\",\"SetPIDSuccess\":\"true\"}");
	}
	else
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetPID\",\"SetPIDSuccess\":\"false\"}");
	}
}

/**
creates the feedback string about modifying the main power switch state

*/

void MainTCPserver::create_setmainpower_json(bool SetMainPower) //NOT READY
{
	if(SetMainPower)
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetMainPower\",\"MainPowerStatus\":\"true\"}");
	}
	else
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetMainPower\",\"MainPowerStatus\":\"false\"}");
	}

}












































