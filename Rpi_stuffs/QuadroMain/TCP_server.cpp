#include "TCP_server.h"

MainTCPserver::MainTCPserver(int port)
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
}

void* MainTCPserver::call_member_function(void *arg)
{ 
	 ((MainTCPserver*)arg)->read_rx();
	return ((MainTCPserver*)arg)->proc_rx();
}

void* MainTCPserver::read_rx()
{
	
	bzero(RX_buffer,strlen(RX_buffer));
	if ( (comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL))< 0 )
	{
		fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}
	
	read(comm_fd,RX_buffer,300);
	
	//socket closing is in "proc_rx" function
	
}

int MainTCPserver::loop_listening(void)
{
	int i=0;
	if( pthread_create(&this->tcp_listener , NULL ,MainTCPserver::call_member_function ,this) < 0)
    {
            printf("could not create thread");
            return 1;
    }
	return 0;

}

int MainTCPserver::stop_listening(void)
{
	pthread_cancel(this->tcp_listener);
	pthread_detach(this->tcp_listener);

}

void* MainTCPserver::proc_rx(void)
{
	if(strcmp(RX_buffer,"StateRequest\0") == 0)
	{
		printf("state request has sent\n");
		sprintf(TX_buffer,"This is the state of the Rpi");		
		write(comm_fd, TX_buffer, strlen(TX_buffer)+1);
	}

	if ( close(comm_fd) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}

}



