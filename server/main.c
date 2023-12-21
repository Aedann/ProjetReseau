#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "bank.h"

int main(int argc, char **argv)
{
   init();

   Client client1;
   initialize_client(&client1, 1, "password123");
   add_account(&client1, 101);
   add_account(&client1, 102);

   deposit(&client1, 101, 500.0);
   deposit(&client1, 102, 1000.0);
   deposit(&client1, 103, 200.0); // Account not found

   app();

   end();

   return EXIT_SUCCESS;
};