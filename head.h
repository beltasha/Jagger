#include <stdlib.h>
#include <stdio.h>
//#include "strophe.h"
#include <string.h> // strlen



#include <sys/types.h>
#include <conio.h>	// kbhit
#include <winsock.h>
#define REMOTEPORT 5222



/* socket variables */
struct sockaddr_in client_addr;
struct sockaddr_in server_addr;
struct hostent *hostname;

struct sockaddr_in si_client_addr;
struct sockaddr_in si_server_addr;
struct hostent *si_hostname;
int sock;
int sin_size;
int si_sock;

void socketConnect(void);



char remotehost[128];
char remoteport[6];
