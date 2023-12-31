#ifndef CLIENT_H
#define CLIENT_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF     "\r\n"

#define BUF_SIZE 1024

void init(void);
void end(void);
void app(const char *address, char * argvPort, const char *name);
int init_connection(const char *address, SOCKADDR_IN *sin, int PORT);
void end_connection(int sock);
int read_server(SOCKET sock, SOCKADDR_IN *sin, char *buffer);
void write_server(SOCKET sock, SOCKADDR_IN *sin, const char *buffer);
void command_input(SOCKET sock, SOCKADDR_IN *sin);

#endif /* guard */