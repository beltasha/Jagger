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

void autentification()
{
	    char buf[400];
		char challenge[100];
		char decode[100];
		int n, p;
		char* pass = "iloveyasp";
		char* md5pass;

		//запрос авторизации у сервера
		sprintf_s(message_out, 800, "<auth xmlns='urn:ietf:params:xml:ns:xmpp-sasl' mechanism='DIGEST-MD5'/>");
		sendout(message_out);
		//парсим challenge-ответ
		recv(sock, buf, 800, 0);
		n = strstr(buf, "'>") - buf;
		p = strstr(buf, "</c") - buf;
		n += 2;

		int i = 0;

		for (i = 0; n < p ; i++)
		{
			challenge[i] = buf[n];
			n++;
		}
		challenge[i] = '\0';
		b64_decode(challenge, decode);
		//выдираем nonce
		p = strstr(decode, "\",q") - decode-7;
		for (i = 0; i < p; i++)
		{
			nonce[i] = decode[i+7];
		}
		nonce[i] = '\0';

		printf("%s\n", buf);

		md5pass = str2md5(pass, strlen(pass));

		sprintf_s(buf, 400, "username=\"delphi - test\",realm = \"%s\", nonce = \"%s\", cnonce = \"2313e064449daa0ca2b76363525059ebd\", nc = 00000001, qop = auth, digest - uri = \"xmpp/jabber.ru\", charset = utf - 8,response = %s", "jabber.ru", nonce, md5pass);

		printf("%s\n", challenge);
		printf("%s\n", decode);
		printf("%s\n", nonce);
		printf("%s\n", pass);
}


void exitclient()
{
	printf("\n\n\tThanks for try!\n\n");
	closesocket(sock);
	WSACleanup();
	exit(0);
}


































int sendout(char *message_out)
{
	printf("%s\n",message_out);
	if (strstr(message_out,"</stream:") != 0)
	{
		exitclient();
	}

	if ((strstr(message_out,"<") != NULL))
	if(send(sock,message_out,strlen(message_out),0) == -1) {
	perror("send");
	//exit(1);
	}
//		printf(" fin send out - buflen = %i (%i)\n",strlen(message_out), flap_out.data_length);	/* Resultat buffer lecture */
return(0);
}

int main(int argc, char *argv[])
{
	printf("Hey!");



/* encodage du pass */
strcpy(client,argv[0]);
strcpy(user,argv[1]);
if (strstr(user,"@") != NULL){
	strcpy(nick,user);
	if (strstr(user,"/") != NULL)
		sscanf(nick,"%[^@]@%[^/]/%s",user,domain,ressource);
	else
		{sscanf(nick,"%[^@]@%s",user,domain);
		//strcpy(ressource,CLIENT_NAME);}
	}
strcpy(pass,argv[2]);
}
if (argv[3] != NULL)
	strcpy(remotehost,argv[3]);
else
	strcpy(remotehost,domain);
if (domain==NULL)
	strcpy(domain,remotehost);

//if (argv[4] != NULL)
if ((argv[4] != NULL) && (argv[3] != NULL))
	strcpy(remoteport,argv[4]);
else
	strcpy(remoteport,"5222");



printf("\n\tCONNECTED as %s@%s on %s:%s!\n\n",user,domain,remotehost,remoteport);


	socketConnect();
	login();
    flapon(); printf("get stream...\n");
    toc_signon(); printf("server signon...\n");
	toc_init_done();

	while(1) 
	{
	gets(message_out);
	 
	choose_command();
	sendout(message_out);
	}
//printf("kbhit:'%s'\n",message_out);


	

//system("pause");
	return(0);
} // end of main()
