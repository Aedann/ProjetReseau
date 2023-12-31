#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "bank.h"

int main(int argc, char **argv)
{

   if(argc < 2)
   {
      printf("Usage : %s [port]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   User users[MAX_USERS];
   for(int i = 0; i< MAX_USERS ;i++){
      memset(&users[i],0,sizeof users[i]);
   }
   initialize_user(users, 1, "chaton123");
   add_account(users, 1 ,101, "chaton123");
   add_account(users, 1 ,102, "chaton123");
   printf("Comptes 101 et 102 créés pour user_id = 1 \n");
   app(argv[2],users);

   end();

   return EXIT_SUCCESS;
};