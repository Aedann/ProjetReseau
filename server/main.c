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

   Client client1;
   initialize_client(&client1, 1, "password123");
   add_account(&client1, 101, "password123");
   add_account(&client1, 102, "password123");

   app(argv[2]);

   end();

   return EXIT_SUCCESS;
};