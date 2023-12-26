#ifndef STRUCTBANK_H
#define STRUCTBANK_H

#include <sys/socket.h>
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

#define MAX_ACCOUNTS 10
#define MAX_PASSWORD_LENGTH 50
#define BUF_SIZE 1024
#define MAX_OPERATIONS 64
#define MAX_RES_SIZE 4096

typedef enum Operation_type{
    AJOUT,
    RETRAIT
}Operation_t;

typedef struct Operation
{
    char date[64];
    Operation_t type;
    float somme;
} Operation;

typedef struct {
    int account_id;
    float balance;
    Operation operations[MAX_OPERATIONS];
} Account;

typedef struct {
   SOCKET sock;
   char name[BUF_SIZE];
} Client;

typedef struct {
   int user_id;
   char password[MAX_PASSWORD_LENGTH];
   Account accounts[MAX_ACCOUNTS];
   int num_accounts;
} User;

#endif