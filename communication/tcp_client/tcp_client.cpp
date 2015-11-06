#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
    /*if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }*/

    int len;
    string message;
    char line[256];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream;
    int i=0;
    while(1)
    {
        //printf("Press Enter if you want to get the state of the sensors");
        //getchar();
        stream = connector->connect("192.168.0.3", 22000);
        if (stream) {
            message.clear();
            message = "{\"MessageType\":\"GetPID\" } ";
            stream->send(message.c_str(), message.size());
            printf("sent - %s\n", message.c_str());
            len = stream->receive(line, sizeof(line));
            line[len] = NULL;
            printf("%d: received - %s\n",i, line);
            delete stream;
        }
        i++;
        sleep(0.5);
    }

    exit(0);
}
