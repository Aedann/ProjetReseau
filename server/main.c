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
   users[6].user_id = 69;
   initialize_user(users, 1, "123");
   add_account(users, 1 ,101, "123");
   add_account(users, 1 ,102, "123");
   printf("Comptes 101 et 102 créés pour user_id = 1 \n");
   printf("users[0].user_id = %d\n",users[0].user_id);
   app(argv[2],users);

   end();

   return EXIT_SUCCESS;
};