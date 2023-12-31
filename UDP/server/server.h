#ifndef SERVER_H
#define SERVER_H

#include "structBank.h"
#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#else

#error not defined for this platform

#endif

#define CRLF        "\r\n"
#define MAX_CLIENTS     100



void init(void);
void end(void);
void app(char * argvPort, User *users);
int init_connection(int PORT);
void end_connection(int sock);
int read_client(SOCKET sock, SOCKADDR_IN *csin, char *buffer);
void write_client(SOCKET sock, SOCKADDR_IN *csin, const char *buffer);
void send_message_to_all_clients(int sock, Client *clients, Client *client, int actual, const char *buffer, char from_server);
void remove_client(Client *clients, int to_remove, int *actual);
int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual);
Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual);
void send_welcome_message(SOCKET client_sock, SOCKADDR_IN *csin);

#endif /* guard */