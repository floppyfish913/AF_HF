/*Required Headers*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    if(argc != 2 )
    {
        printf("\ngive me a port!\n");
        exit(1);
    }
    printf("looking for host...\n");

    char str[300];
    int listen_fd, comm_fd;

    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
    printf("ready\n");
    while(1)
    {

        bzero( str, 300);

        read(comm_fd,str,300);
        if(str[0]!='\0')
        {
            printf("Received - %s\n",str);
        }
        else
        {
            printf("Disconnection!!!");
        }
        //write(comm_fd, str, strlen(str)+1);

    }
}
