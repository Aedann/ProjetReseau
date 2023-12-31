//Code repris de https://broux.developpez.com/articles/c/sockets/#LV-C-2

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "bank.h"

void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

void app(char * argvPort, User *users)
{
   int PORT = atoi(argvPort);
   SOCKET sock = init_connection(PORT);
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];

   fd_set rdfs;

   while(1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };

         /* a client is talking */
         printf("\nReady to read Client\n");
         fflush(stdout);
         read_client(sock, &csin, buffer);

         if(check_if_client_exists(clients, &csin, actual) == 0)
         {
            if(actual != MAX_CLIENTS)
            {
               Client c = { csin };
               strncpy(c.name, buffer, BUF_SIZE - 1);
               clients[actual] = c;
               actual++;
               /* Send welcome message to the new client */
               send_welcome_message(sock, &csin);
            }
         }
         else
         {
            Client *client = get_client(clients, &csin, actual);
            if(client == NULL) continue;
            // Processing Client Command
            char res[MAX_RES_SIZE];
            buffer[MAX_RES_SIZE - 1] = '\0'; // Ensure the buffer is null-terminated
            process_command(users, buffer, res);
            write_client(sock, &csin, res);
         }
      }
   }
   end_connection(sock);
}

int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      if(clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
        && clients[i].sin.sin_port == csin->sin_port)
      {
     return 1;
      }
   }

   return 0;
}

Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      if(clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
        && clients[i].sin.sin_port == csin->sin_port)
      {
     return &clients[i];
      }
   }

   return NULL;
}

void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender != &clients[i])
      {
     if(from_server == 0)
     {
        strncpy(message, sender->name, BUF_SIZE - 1);
        strncat(message, " : ", sizeof message - strlen(message) - 1);
     }
     strncat(message, buffer, sizeof message - strlen(message) - 1);
     write_client(sock, &clients[i].sin, message);
      }
   }
}

int init_connection(int PORT)
{
   /* UDP so SOCK_DGRAM */
   SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   return sock;
}

void end_connection(int sock)
{
   closesocket(sock);
}

int read_client(SOCKET sock, SOCKADDR_IN *sin, char *buffer)
{
   int n = 0;
   size_t sinsize = sizeof *sin;

   if((n = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (SOCKADDR *) sin, &sinsize)) < 0)
   {
      perror("recvfrom()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

void write_client(SOCKET sock, SOCKADDR_IN *sin, const char *buffer)
{
   if(sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *) sin, sizeof *sin) < 0)
   {
      perror("send()");
      exit(errno);
   }
}


void send_welcome_message(SOCKET client_sock, SOCKADDR_IN *sin)
{
    const char *welcome_message = "Welcome to the chat server!\nUse these commands : \nAJOUT <id_client id_compte password somme>\nRETRAIT <id_client id_compte password somme>\nSOLDE <id_client id_compte password>\nOPERATIONS <id_client id_compte password>\n";
    write_client(client_sock, sin, welcome_message);
}