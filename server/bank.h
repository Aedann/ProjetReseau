#ifndef BANK_H
#define BANK_H

#include "structBank.h"


void initialize_client(User *user, int user_id, const char *password);
void add_account(User *user, int account_id, const char *password);
void ajout(User *user, int account_id,  const char *password, char * res, double amount);
void retrait(User *user, int account_id,  const char *password, char * res, double amount);
void solde(User *user, int account_id,  const char *password, char * res);
void operations(User *user, int account_id, const char *password, char * res);
void process_command(User *user, char *buffer, char *res); 

#endif /* guard */



/* Utilise les HTML Status : 400 si le nom de l'opération n'existe pas, 401 si le mot de passe est mauvais, 404 si mauvais account_id, 200 si tout s'est bien passé. */