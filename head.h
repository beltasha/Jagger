#include <stdlib.h>
#include <stdio.h>

#include <string.h> // strlen



#include "win32config.h"
#include "libxml\parser.h"
#include "libxml\tree.h"
#include "libxml\xmlIO.h"
#include <sys/types.h>
#include <conio.h>	// kbhit
#include <winsock.h>
#define REMOTEPORT 5222
#define ENCODING "UTF-8"
#pragma comment(lib, "Ws2_32.lib")



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
char remoteport[6] = "5222";
char* userjid;
char user[127];
char pass[127];
char domain[128];
char ressource[128];
char message_out[3000];
char nonce[20];
char id[20];

unsigned short sequence_number_client;

void decodeCommands();

char tmp[255];

//char logname[255];
char logname_to[255];
char name_to[255];
char text[3000];
int buflen;
fd_set onrecv_set;
struct timeval tv;
char* mess_out[3000];
char* mess1[3000];
char *room[127];


int RecvWithTimeout(int Socket, char *Buffer, int Len, long Timeout, int *bTimedOut);
int *bTimedOut;

void all_reg();
char* check_server_message(char* buf);
xmlDocPtr parse(char* buf);
char* return_jid(xmlDocPtr * doc);
void return_contacts_list(xmlDocPtr * doc);