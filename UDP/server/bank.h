#ifndef BANK_H
#define BANK_H

#include "structBank.h"

#define MAX_USERS 16 


void initialize_user(User *users, int user_id, const char *password) ;
void add_account(User *users,int user_id, int account_id, const char *password) ;
void ajout(User *users, int user_id, int account_id,  const char *password, char *res, double amount) ;
void retrait(User *users, int user_id, int account_id, const char *password, char *res, double amount) ;
void solde(User *users, int user_id, int account_id, const char *password, char *res) ;
void operations(User *users, int user_id, int account_id, const char *password, char *res);
void process_command(User *user, char *buffer, char *res);
void add_operation(User *users, int index, int account_index, Operation_t type, float somme);

#endif /* guard */
