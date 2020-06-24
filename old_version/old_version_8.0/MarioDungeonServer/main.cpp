#include "MarioDungeonServer.h"
#include <stdio.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	if(argc>1)
	{
		EventLoop loop;
		unsigned int port=atoi(argv[1]);
		InetAddress serverAddr(port);
		MarioDungeonServer server(&loop,serverAddr);
		server.start();
		loop.loop();
	}
	else
	{
		printf("Usage:%s port\n",argv[0]);
	}

	return 0;
}
