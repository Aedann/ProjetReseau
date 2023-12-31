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
int read_client(SOCKET sock, char *buffer);
void write_client(SOCKET sock, const char *buffer);
void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
void remove_client(Client *clients, int to_remove, int *actual);
void clear_clients(Client *clients, int actual);
void send_welcome_message(SOCKET client_sock);

#endif /* guard */