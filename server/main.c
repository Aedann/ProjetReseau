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

   User users[256];
   initialize_user(users, 1, "password123");
   add_account(users, 1 ,101, "password123");
   add_account(users, 1 ,102, "password123");

   app(argv[2]);

   end();

   return EXIT_SUCCESS;
};