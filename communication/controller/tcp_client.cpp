#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    int len;
    string message;
    char line[256];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream;
    int i=0;
    printf("configuration was successful\n");
    while(1)
    {
        //printf("Press Enter if you want to get the state of the sensors");
        //getchar();
        stream = connector->connect(argv[2], atoi(argv[1])); //write an IP address in the lace oif argv[2]
        if (stream) {
            message.clear();
            message = "{\"MessageType\": \"SetMainPower\",\"MainPowerOn\": \"false\"}";
            stream->send(message.c_str(), message.size());
            printf("sent - %s\n", message.c_str());
            len = stream->receive(line, sizeof(line));
            line[len] = NULL;
            printf("%d: received - %s\n",i, line);
            delete stream;
        }
        i++;
        sleep(3);
    }

    exit(0);
}
