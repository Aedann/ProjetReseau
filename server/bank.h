#ifndef BANK_H
#define BANK_H

#include "structBank.h"


void initialize_client(Client *client, int client_id, const char *password);
void add_account(Client *client, int account_id);
void deposit(Client *client, int account_id, double amount);


#endif /* guard */