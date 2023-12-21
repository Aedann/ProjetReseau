#ifndef BANK_H
#define BANK_H

#include "structBank.h"


int initialize_client(Client *client, int client_id, const char *password);
int add_account(Client *client, int account_id, const char *password);
int ajout(Client *client, int account_id,  const char *password, double amount);
int retrait(Client *client, int account_id,  const char *password, double amount);
int solde(Client *client, int account_id,  const char *password);
int operations(Client *client, int account_id, const char *password);
int process_command(Client *client, char *buffer); 

#endif /* guard */



/* Utilise les HTML Status : 400 si le nom de l'opération n'existe pas, 401 si le mot de passe est mauvais, 404 si mauvais account_id, 200 si tout s'est bien passé. */