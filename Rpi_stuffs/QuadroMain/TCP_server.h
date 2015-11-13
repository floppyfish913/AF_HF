#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "MPU6050.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "PCA9685.h"


class MainTCPserver{

	private:
	
	//variables
	int listen_fd, comm_fd,reporter;
	struct sockaddr_in servaddr;
	pthread_t tcp_listener;
	char TX_buffer[300];
	char RX_buffer[300];

	//fuctions
	static void* call_member_function(void *arg);	
	void* read_rx(void);
	void* proc_rx(void);
	void create_getstate_json(); //it needs an available IMU class
	void create_getpid_json();
	void create_setpid_json(bool SetPid);
	void create_setmainpower_json(bool SetMainPower);

	public:

	//variables

	//functions
	MainTCPserver(char* address,int port);
	int start_listening(void);
	int stop_listening(void);
	void report_state();
};

