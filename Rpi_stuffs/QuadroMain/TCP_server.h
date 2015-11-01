#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>


class MainTCPserver{

	private:
	
	//variables
	int listen_fd, comm_fd;
	struct sockaddr_in servaddr;
	pthread_t tcp_listener;

	public:
	//functions
	char TX_buffer[300];
	char RX_buffer[300];
	MainTCPserver(int port);
	void* read_rx(void);
	void* proc_rx(void);
	static void* call_member_function(void *arg);
	int loop_listening(void);
	int stop_listening(void);
};

