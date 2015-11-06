#include "TCP_server.h"

using namespace rapidjson;

extern IMU MySensor;
extern PCA9685 PWM;

MainTCPserver::MainTCPserver(char* address,int port)
{
	if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
	{
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

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
	printf("TCP server config was successful on port: %d\n",port);

	//////setting reporter socket 	
	if ( (reporter=socket(AF_INET,SOCK_STREAM,0)) < 0 ) 
	{
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}
	//reporter=socket(AF_INET,SOCK_STREAM,0);
    	bzero(&servaddr,sizeof servaddr);
 
    	servaddr.sin_family=AF_INET;
    	servaddr.sin_port=htons(port);
     	inet_pton(AF_INET,address,&(servaddr.sin_addr));
	
    if ( connect(reporter, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
		printf("ECHOCLNT: Error calling connect()\n");
		exit(EXIT_FAILURE);
    }

    	
	
}

void* MainTCPserver::call_member_function(void *arg)
{ 

	while(1)
	{
		((MainTCPserver*)arg)->read_rx();
		((MainTCPserver*)arg)->proc_rx();
	}
}


//receiving the input string from socket
void* MainTCPserver::read_rx()
{
	
	bzero(RX_buffer,300);
	if ( (comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL))< 0 )
	{
		fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}
	read(comm_fd,RX_buffer,299);
	//printf("Recived: %s - over\n",RX_buffer);
	
	
}

// creating a new thread and, starting linstening in the background
int MainTCPserver::start_listening(void) 
{
	int i=0;
	if( pthread_create(&this->tcp_listener , NULL ,MainTCPserver::call_member_function ,this) < 0)
    {
            printf("could not create thread");
            return 1;
    }
	return 0;

}

//stop listening, and closeing the thread
int MainTCPserver::stop_listening(void)
{
	pthread_cancel(this->tcp_listener);
	pthread_detach(this->tcp_listener);

}


//converting input str to JSON obj, and processing the input
void* MainTCPserver::proc_rx(void)
{
	Document InputJSON;
	int len;
	//sprintf(RX_buffer,"{\"MessageType\":\"SetPID\"}");
	if(InputJSON.Parse<0>(RX_buffer).HasParseError()) //convert string to JSON obj
	{
		//printf("ERROR in parsing JSON str\n");
	}
	
	if(InputJSON["MessageType"] == "GetState")
	{
		//printf("GetState message detected\n");
		create_getstate_json();
	}
	else if(InputJSON["MessageType"] == "GetPID")
	{
		//printf("GetPID message detected\n");
		create_getpid_json();
	}
	else if(InputJSON["MessageType"] == "SetPID")
	{
		//printf("SetPID message detected\n");
		//SET PID 
		create_setpid_json(true);
	}
	else if(InputJSON["MessageType"] == "SetMainPower")
	{
		//printf("SetMainPower message detected\n");
		//SET PID 
		create_setpid_json(true);
	}
	else
	{
		sprintf(TX_buffer,"{\"MessageType\":\"ERROR\"}");
	}

	write(comm_fd, TX_buffer, strlen(TX_buffer));

}



void MainTCPserver::create_getstate_json() //NOT READY
{
	
	int MotorState[]={4001,4002,4003,4004};
	char MainPowerState[6];
	/*if(PWM.GetMainPower())
	{
		MainPowerState = "true";
	}
	else
	{
		MainPowerState = "false";
	}*/
	sprintf(MainPowerState,"true");
	sprintf(TX_buffer,"{\"MessageType\":\"GetState\",\"GyroX\":%7.3f,\"GyroY\":%7.3f,\"GyroZ\":%7.3f,\"AccelX\":%7.3f,\"AccelY\":%7.3f,\"AccelZ\":%7.3f,\"KalmanXangle\":%7.3f,\"Kalmanyangle\": %7.3f,\"Altitude\": %d,\"MainPowerState\": \"%s\",\"MotorState0\": 4001,\"MotorState1\":4002,\"MotorState2\":4003,\"MotorState3\":4004}",MySensor.GData.x,MySensor.GData.y,MySensor.GData.z,MySensor.AData.x,MySensor.AData.y,MySensor.AData.z,MySensor.KFData.x,MySensor.KFData.y,MySensor.Altitude,MainPowerState);

}

void MainTCPserver::report_state()
{
	create_getstate_json();
	//printf("sent - %s\n",TX_buffer);
	write(reporter, TX_buffer, strlen(TX_buffer));


}



void MainTCPserver::create_getpid_json() //NOT READY
{
	sprintf(TX_buffer,"{\"MessageType\":\"GetPID\",\"Kp\":4,\"Ki\":12,\"Kd\":44}");
}

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

void MainTCPserver::create_setmainpower_json(bool SetMainPower) //NOT READY
{
	if(SetMainPower)
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetMainPower\",\"MainPowerState\":\"true\"}");
	}
	else
	{
		sprintf(TX_buffer,"{\"MessageType\":\"SetMainPower\",\"MainPowerState\":\"false\"}");
	}

}












































