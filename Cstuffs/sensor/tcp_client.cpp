#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}


class tcp_client {

 public:
    char *tcp_server;
    int tcp_port;
    tcp_client (char*,int);
    int send_package(char*);

};

tcp_client::tcp_client(char* tcp_server_in, int tcp_port_in)
{
    tcp_server=tcp_server_in;
    tcp_port=tcp_port_in;

}

int tcp_client::send_package(char* message_str) {
    int sockfd, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char buf[BUFSIZE];
    char *tcp_server="192.168.0.115";
    int tcp_port=10000;

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(tcp_server);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", tcp_server);
        exit(0);
    }
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(tcp_port);

    /* connect: create a connection with the server */
    if (connect(sockfd,(struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
      error("ERROR connecting");



    /* send the message line to the server */
    n = write(sockfd, message_str, strlen(message_str));
    if (n < 0)
      error("ERROR writing to socket");


    close(sockfd);
    return 0;
}
