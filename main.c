#include <stdlib.h>
#include <stdio.h>
//#include "strophe.h"
#include <string.h> // strlen

#include "head.h"

#include <sys/types.h>
#include <conio.h>	// kbhit
#include <winsock.h>



void socketConnect()
{
/* Start Winsock */

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 */
	if (WSAStartup(version, &wsadata) != 0)
	{	perror("initializing winsock");
		exit(1);
	}

/*  Create socket  */
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	perror("opening steam socket in");
		exit(1);
	}

/*  Name socket*/
	hostname=gethostbyname(remotehost);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr( inet_ntoa(*((struct in_addr *)hostname->h_addr)) );
	server_addr.sin_port = htons((unsigned short int) atoi(remoteport));  /* Use specified port */
	memset(server_addr.sin_zero, 0, 8);

/* resolve connecting socket */
	printf("connecting to: %s port %s\n", inet_ntoa(server_addr.sin_addr),remoteport);
	system("pause");
	sin_size=sizeof(struct sockaddr_in);
	if(connect(sock,(struct sockaddr*)&server_addr, sin_size) == -1) {
		perror("connect");
		exit(1);}
}

int main()

{
	printf("it will be a jabber client (I hope)");
	
	socketConnect();
	
	return 0;
}
