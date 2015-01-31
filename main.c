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
	{
		perror("initializing winsock");
		exit(1);
	}

	/*  Create socket  */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("opening steam socket in");
		exit(1);
	}
	strcpy(remoteport, "5222");
	//strcpy(remotehost,"12jabber.com");

	/*  Name socket*/
	hostname = gethostbyname(remotehost);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)hostname->h_addr)));
	server_addr.sin_port = htons((unsigned short int) atoi(remoteport));  /* Use specified port */


	printf("connecting to: %s port %s\n", inet_ntoa(server_addr.sin_addr), remoteport);

	sin_size = sizeof(struct sockaddr_in);
	if (connect(sock, (struct sockaddr*)&server_addr, sin_size) == -1) {
		perror("connect");
		exit(1);
	}

}


void hello_message()
{
	char buf[800];
	sprintf_s(message_out, 3000,
		"<?xml version='1.0' encoding='%s'?>"
		"<stream:stream to='%s' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams' xml:l='ru' version='1.0'>", ENCODING, remotehost);
	sendout(message_out);
	sequence_number_client = 3615;
	recv(sock, buf, 800, 0);
	//printf("%s\n", buf);
	recv(sock, buf, 800, 0);
	//printf("%s\n", buf);

}

int create_binstr(char *dstbuf, char *u, char *p) {

	int len;

	memset(dstbuf, 0, 600);
	memcpy(dstbuf + 1, u, strlen(u));
	memcpy(dstbuf + 2 + strlen(u), p, strlen(p));
	len = strlen(u) + strlen(p) + 2;

	return len;
}





void autentification()
{
	/*аутентификация типом PLAIN*/

	char *msg_bind_src = ""
		"<iq id=\"dummyid\" type=\"set\">"
		"<bind xmlns=\"urn:ietf:params:xml:ns:xmpp-bind\">"
		"<resource>%s</resource>"
		"</bind>"
		"</iq>";

	char *msg_presence = "<presence><show>chat</show><status></status><priority>10</priority></presence>";
	char *msg_session = ""
		"<iq id=\"seses\" type=\"set\">"
		"<session xmlns=\"urn:ietf:params:xml:ns:xmpp-session\" />"
		"</iq>";
	char *msg_stst = "<?xml version=\"1.0\"?>\n"
		"<stream:stream to=\"jabber.ru\" "
		"version=\"1.0\" "
		"xmlns=\"jabber:client\" "
		"xmlns:stream=\"http://etherx.jabber.org/streams\">";

	char buf[600];
	char jstr[600];
	char* decode;
	int len, n, p = 0;

	//преобразуем логин и пароль в вид:
	// \x00username\x00pass
	len = create_binstr(jstr, user, pass);
	//переодим в base64
	decode = base64_encode(jstr, len, &len);
	decode[len] = '\0';
	//подготавливаем пакет и отправляем на сервер
	sprintf_s(message_out, 500, "<auth xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\" mechanism=\"PLAIN\">%s</auth>", decode);
	sendout(message_out);

	recv(sock, buf, 500, 0);
	//recv(sock, buf, 500, 0);
	if (strstr(buf, "success") == NULL) {
		perror("Error: not authentificate\n");
		return;
	}

	printf("Authentificate!\n");

	//делаем запрос на JID
	sprintf_s(message_out, 500, "<?xml version='1.0' encoding='UTF-8'?><stream:stream to='%s' xmlns='jabber:client' xmlns:stream= 'http://etherx.jabber.org/streams' xml:l='ru' version='1.0'>", remotehost);
	sendout(message_out);
	recv(sock, buf, 500, 0);

	//Отправляем серверу JID-ressource
	sprintf_s(message_out, 500, msg_bind_src, ressource);
	sendout(message_out);
	recvall(message_out);

	//parse(message_out);
	userjid = return_jid(parse(strstr(message_out, check_server_message(message_out))));
	printf("%s\n", userjid);

	
	sprintf_s(message_out, 500, msg_session);
	sendout(message_out);
	recvall(message_out);
	//recv(sock, buf, 500, 0);

	//Отправляем статус "Онлайн"
	sprintf(message_out, "<presence><show></show></presence>");
	sendout(message_out);
	recvall(message_out);
}

char* check_server_message(char* buf)
{
	boolean f1 = 0; 
	boolean f2 = 0;
	boolean f3 = 0; 
	char* tag1 = "<presence";
	char* tag2 = "<message";
	char* tag3 = "<iq";

	if (strstr(buf, tag1) != NULL)
	{
		//parse(strstr(buf, tag1));
		f1 = 1;
	}
		

	if (strstr(buf, tag2) != NULL)
	{
		//parse(strstr(buf, tag2));
		f2 = 1;
	}

	if (strstr(buf, tag3) != NULL)
	{
		//parse(strstr(buf, tag3));
		f3 = 1;
	}
	//return fl;
	//parse(strstr(buf, tag3));
	if (f1 == 0 && f2 == 0 && f3 == 0)
		exit(1);
	if (f1)
		return tag1;
	if (f2)
		return tag2;
	//if (f3)
		return(tag3);
	}

xmlDocPtr parse(char* buf)
{
	xmlDocPtr doc = NULL;
	xmlNode *root_element = NULL;
	//XML_Parser parser = XML_ParserCreate(NULL);
	int n = strlen(buf);
	doc = xmlParseMemory(buf, n);
	if (doc == NULL)
	{
		printf("Error parsing file");
		exit(1);
	}
	return doc;
}

void return_message(xmlDocPtr doc)
{
	xmlNode *root_element = NULL;
	root_element = xmlDocGetRootElement(doc);
	xmlNode *cur_node = root_element;
	//char *tag;
	//char *value;
	
	for (; cur_node; cur_node = cur_node->next)
	{
		printf("<%s>: %s", logname_to, xmlNodeGetContent(cur_node));
		return_message(cur_node->children);
	}
}


char* return_jid(xmlDocPtr * doc)
{
	xmlNode *root_element = NULL;
	root_element = xmlDocGetRootElement(doc);
	xmlNode *cur_node = root_element;
	char *tag;
	char *value;
	//boolean fl = 0;
	for (; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			tag = cur_node->name;
			value = xmlNodeGetContent(cur_node);
			if (strstr(tag, "jid") != NULL)
			{
				value = xmlNodeGetContent(cur_node);
				//userjid = value;
				break;
			}
		}
		//break;
		return_jid(cur_node->children);
	}
	return value;
	//printf("%s\n", userjid);
}

void return_contacts_list(xmlDocPtr * doc)
{
	xmlNode *root_element = NULL;
	root_element = xmlDocGetRootElement(doc);
	xmlNode *cur_node = root_element;
	xmlAttrPtr attr; // берем атрибут
	for (; cur_node; cur_node = cur_node->next)
	{
		char *tag;
		char *atr;
		char *atr_val;
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			tag = cur_node->name;

			if (strstr(tag, "item") != NULL)
			{
				attr = cur_node->properties;
				if ((strstr(attr->name, "subscription") != NULL) && (strstr(attr->children->content, "none") == NULL))
				{
					while (attr)
					{
						atr = attr->name;
						//atr_val = attr->children->content;
						if (strstr(atr, "jid") != NULL);
						{
							atr_val = attr->children->content;
							//printf(" %s", atr);// печать имени атрибута
							
						}// печать значения атрибута	
						attr = attr->next; //на следующий атрибут
						
					}
					printf("\t%s\n", atr_val);
				//attr = attr->next;//на следующий атрибут
				}
			}
		}
		return_contacts_list(cur_node->children);
	}
}

void all_reg()
{
	hello_message();
	autentification();
}

int sendout(char *message_out)
{
	int total = 0;
	int n=0;
	int len = strlen(message_out);

	while (total < len)
	{
		n = send(sock, message_out + total, len - total, 0);
		if (n == -1) { break; }
		total += n;
	}
	if (n != 0)
		return("Error output, try again");
	return 0;
	//return (n == -1 ? -1 : total);
}
int RecvWithTimeout(int Socket, char *Buffer, int Len, long Timeout, int *bTimedOut)
{
	fd_set ReadSet;
	int n;
	struct timeval Time;

	FD_ZERO(&ReadSet);
	FD_SET(Socket, &ReadSet);
	Time.tv_sec = Timeout;
	Time.tv_usec = 0;
	*bTimedOut = FALSE;
	n = select(Socket + 1, &ReadSet, NULL, NULL, &Time);
	if (n > 0) { /* got some data */
		return recv(Socket, Buffer, Len, 0);
	}
	if (n == 0) { /* timeout */
		*bTimedOut = TRUE;
	}
	return n; /* trouble */
}

void check_mail(char *buf)
{
	recvall(buf);
	if (strstr(buf, "<message") != NULL)
		return_message(parse(strstr(message_out, check_server_message(buf))));
	if (strstr(message_out, "error") != NULL)
		printf("Error input, try again");
}

int recvall(char *message_out){


	int total = 0;
	int n;
	int len = 3000;
	int* bTimedOut = (int*)malloc(sizeof(int*));

	while (total < len)
	{
		n = RecvWithTimeout(sock, message_out + total, len - total, 1, bTimedOut);
		if (n == -1 || n == 0) { break; }
		total += n;
	}

	message_out[total] = '\0';
	if (n != 0)
		return("Error output, try again");
	return 0;
	//return (n == -1 ? -1 : total);
}




void exitclient()
{
	printf("\n\n\tThanks for try!\n\n");
	closesocket(sock);
	WSACleanup();
	exit(0);
}

void decodeCommands()
{

	if (strstr(message_out, "quit:") != NULL)
	{
		/*strcpy(option, "");
		sscanf(message_out, "quit: %[^\n]", option);
		sprintf(message_out, "<presence type='unavailable'><status>%s</status></presence>", option);
		sendout(message_out);
		sprintf(message_out, "</stream:stream>");
		printf("%s\n", message_out);*/
		exitclient();
	}

	if (strstr(message_out, "status:") != NULL)
	{
		sscanf(message_out, "status: %[^\n]", tmp);
		sprintf(message_out, "<presence><status>%s</status></presence>", tmp);
		sendout(message_out);
		
		check_mail(message_out);
	}


	if (strstr(message_out, "chat: ") != NULL)
	{
		sscanf(message_out, "chat: %s", logname_to);
		printf("You can write messages to %s below\n", logname_to);
		check_mail(message_out);
		boolean fl = 1;
		while (fl)
		{
			check_mail(message_out);
			gets(text);
			if (strstr(text, "exitchat:") != NULL)
			{
				fl = 0;
				break;
			}
				
			//scanf("%s", text);
			sprintf(message_out, "<message to='%s' type='chat'>"
				"<x xmlns='jabber:x:event'><composing/></x>"
				"<body>%s</body></message>", logname_to, text);
			sendout(message_out);

			check_mail(message_out);

		}
		check_mail(message_out);

	}

	if (strstr(message_out, "im: ") != NULL)
	{
		sscanf(message_out, "im: %s %[^\n]", logname_to, text);
		sprintf(message_out, "<message to='%s' type='chat'>"
			"<x xmlns='jabber:x:event'><composing/></x>"
			"<body>%s</body></message>", logname_to, text);
		sendout(message_out);
		check_mail(message_out);

	}

	if (strstr(message_out, "list:") != NULL)
	{
		sprintf(message_out, "<iq from='%s' id='123' type='get'><query xmlns='jabber:iq:roster'/></iq>", userjid);
		sendout(message_out);
		recvall(message_out);
		printf("Your friends are:\n");
		return_contacts_list(parse(strstr(message_out, check_server_message(message_out))));
		check_mail(message_out);
		
	}

	if (strstr(message_out, "online:") != NULL)
	{
		sprintf(message_out, "<presence><show></show></presence>");
		sendout(message_out);
		check_mail(message_out);
	}
	if (strstr(message_out, "offline:") != NULL)
	{
		sprintf(message_out, "<presence type='unavailable'></presence>");
		sendout(message_out);
		check_mail(message_out);
	}
	
	check_mail(message_out);
}

int main()
{
	int n = 0;
	char a;

	//parse();
	//system("pause");
	printf("Enter server: ");
	gets_s(remotehost, 128);
	socketConnect();
	printf("Enter username: ");
	gets_s(user, 127);
	printf("Enter password: ");
	//Скрываем вводимые символы  звездочкой
	/*while ((a = getch()) != '\r')
	{
	pass[n] = a;
	printf("%c", '*');
	n = n + 1;
	}*/
	gets_s(pass, 127);

	printf("\nEnter ressource: ");
	gets_s(ressource, 128);
	hello_message();
	autentification();
	//printf("%s\n", strcat(strcat(strcpy(logname, user), "@"), remotehost));

//	sprintf(userjid, "%s/%s", logname, ressource);


	while (1)  /*Boucle permanente */
	{
		
		if (kbhit() != 0)
		{
			check_mail(message_out);
			gets(message_out);

			decodeCommands();
			//sendout(message_out);
			recvall(message_out);
			
			if (strstr(message_out, "<message") != NULL)
				return_message(parse(strstr(message_out, check_server_message(message_out))));
		}
		recvall(message_out);
		if (strstr(message_out, "<message") != NULL)
			return_message(parse(strstr(message_out, check_server_message(message_out))));
	}

	//system("pause"); 
	//return 0;
}

//const char* buf = "<?xml version=\"1.0\"?><stream:features><push xmlns='p1:push'/><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'/><session xmlns='urn:ietf:params:xml:ns:xmpp-session'/><sm xmlns='urn:xmpp:sm:2'/><sm xmlns='urn:xmpp:sm:3'/><c xmlns='http://jabber.org/protocol/caps' hash='sha-1' node='http://www.process-one.net/en/ejabberd/' ver='S4v2n+rKmTsgLFog7BtVvK2o660='/><register xmlns='http://jabber.org/features/iq-register'/></stream:features><iq id='dummyid' type='result'><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'><jid>slapoguzov_alex@jabber.ru/home</jid></bind></iq>\0";

