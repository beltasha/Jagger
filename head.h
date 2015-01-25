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
//int sin_size;
int si_sock;

//void socketConnect(void);
char domain[128];
char ressource[128];
char user[127];
char userjid[384];
char remotehost[128];
char remoteport[6];
char message_out[3000];
char tmp[384];
char client[32];
char nick[127];
char pass[32];
char *sp;
char jabberservername[255];
char option[255];
char option2[255];
int buflen;
char text_old[15000];
int value;
char prior[4];
char roomname[255];
int sin_size;
unsigned short sequence_number_client;