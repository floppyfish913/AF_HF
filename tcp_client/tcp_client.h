#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int send_package();
class tcp_client {

 public:
    char *tcp_server;
    int tcp_port;
    tcp_client (char*,int);
    int send_package(char*);

};
