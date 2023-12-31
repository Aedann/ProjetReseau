#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"


int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [port] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }
   init();

   app(argv[1], argv[2], argv[3]);

   end();

   return EXIT_SUCCESS;
}