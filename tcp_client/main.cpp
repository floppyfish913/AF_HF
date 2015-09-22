#include "tcp_client.h"



int main()
{

    tcp_client asd("192.168.0.115",10000);
	asd.send_package("kaki");

}
