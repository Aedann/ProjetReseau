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
      //printf("\nWhile(1)\n");
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

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
         socklen_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;
         //printf("\nCSOCK = %d\n",csock);
         /* Send welcome message to the new client */
         send_welcome_message(csock);

      }
      else
      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int received_bytes = read_client(clients[i].sock, buffer);
               /* client disconnected */
               if(received_bytes == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else //Processing Client Command
               {
                  char res[MAX_RES_SIZE];
                  buffer[received_bytes] = '\0'; // Ensure the buffer is null-terminated
                  process_command(users, buffer, res);
                  write_client(client.sock, res);
               }
               break;
            }
         }
      }
   }
   //printf("End app\n");
   clear_clients(clients, actual);
   end_connection(sock);
}

void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}

int init_connection(int PORT)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
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

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }else{
      printf("Server listening on port %d...\n", PORT);
   }

   return sock;
}

void end_connection(int sock)
{
   closesocket(sock);
}

int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;
   return n;
}

void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

void send_welcome_message(SOCKET client_sock)
{
    const char *welcome_message = "Welcome to the chat server!\nUse these commands : \nAJOUT <id_client id_compte password somme>\nRETRAIT <id_client id_compte password somme>\nSOLDE <id_client id_compte password>\nOPERATIONS <id_client id_compte password>\n";
    write_client(client_sock, welcome_message);
}






La fonction \mintinline{c}{accept} est utilisée pour accepter une nouvelle connexion TCP. Elle prend le socket d'écoute (\mintinline{c}{sock}), crée un nouveau socket dédié à la communication avec le client (\mintinline{c}{csock}), et récupère les informations du client telles que son adresse IP et le port utilisé.

\begin{minted}{c}
int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
\end{minted}

La fonction \mintinline{c}{send} est utilisée pour envoyer des données sur un socket TCP. Elle prend en argument le descripteur de socket (\mintinline{c}{sock}), le tampon contenant les données (\mintinline{c}{buffer}), la longueur des données à envoyer (\mintinline{c}{length}), et des options supplémentaires (\mintinline{c}{flags}).

\begin{minted}{c}
send(sock, buffer, length, flags);
\end{minted}

La fonction \mintinline{c}{sendto} est utilisée pour envoyer des données sur un socket UDP avec spécification du destinataire. Elle prend en argument le descripteur de socket (\mintinline{c}{sock}), le tampon contenant les données (\mintinline{c}{buffer}), la longueur des données à envoyer (\mintinline{c}{length}), des options supplémentaires (\mintinline{c}{flags}), et les informations sur le destinataire (\mintinline{c}{to}).

\begin{minted}{c}
sendto(sock, buffer, length, flags, (SOCKADDR *)&to, sizeof to);
\end{minted}

La fonction \mintinline{c}{recv} est utilisée pour recevoir des données depuis un socket TCP. Elle prend en argument le descripteur de socket (\mintinline{c}{sock}), le tampon où stocker les données reçues (\mintinline{c}{buffer}), la longueur maximale des données à recevoir (\mintinline{c}{length}), et des options supplémentaires (\mintinline{c}{flags}).

\begin{minted}{c}
recv(sock, buffer, length, flags);
\end{minted}

La fonction \mintinline{c}{recvfrom} est utilisée pour recevoir des données depuis un socket UDP avec spécification de l'expéditeur. Elle prend en argument le descripteur de socket (\mintinline{c}{sock}), le tampon où stocker les données reçues (\mintinline{c}{buffer}), la longueur maximale des données à recevoir (\mintinline{c}{length}), des options supplémentaires (\mintinline{c}{flags}), et les informations sur l'expéditeur (\mintinline{c}{from}).

\begin{minted}{c}
recvfrom(sock, buffer, length, flags, (SOCKADDR *)&from, &fromlen);
\end{minted}




Faire "make" dans ./UDP ou ./TCP
\newline
Lancer le server avec :
\newline
./server.out localhost PORT
\newline

\newline
Lancer le client avec : 
\newline
./client.out localhost PORT username
\newline

\newline
Les comptes déjà créés s'affichent en console : 
\newline
 - Un utilisateur avec pour ID : 1 et mot de passe : chaton123
\newline
 - Deux comptes 101 et 102.
\newline
(changer dans le main.c en cas de besoins).
\newline
On peut quitter le programme avec exit.
