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
	strcpy(remoteport,"5222");
	//strcpy(remotehost,"12jabber.com");
	 
/*  Name socket*/
	hostname=gethostbyname(remotehost);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr( inet_ntoa(*((struct in_addr *)hostname->h_addr)) );
	server_addr.sin_port = htons((unsigned short int) atoi(remoteport));  /* Use specified port */


	printf("connecting to: %s port %s\n", inet_ntoa(server_addr.sin_addr),remoteport);
	
	sin_size=sizeof(struct sockaddr_in);
	if(connect(sock,(struct sockaddr*)&server_addr, sin_size) == -1) {
		perror("connect");
		exit(1);}
	
}


void hello_message()
{
	char buf[800];
	sprintf_s(message_out, 3000,
		"<?xml version='1.0' encoding='%s'?>"
		"<stream:stream to='%s' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams' xml:l='ru' version='1.0'>", remotehost);
	sendout(message_out);
	sequence_number_client = 3615;
	recv(sock, buf, 800, 0);
	//printf("%s\n", buf);
	recv(sock, buf, 800, 0);
	//printf("%s\n", buf);

}

int sendout(char *message_out)
{
	if (send(sock, message_out, strlen(message_out), 0) == -1) {
		perror("send");
		exit(1);
	}
	return(0);
}




void exitclient()
{
	printf("\n\n\tThanks for try!\n\n");
	closesocket(sock);
	WSACleanup();
	exit(0);
}



































