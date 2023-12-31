#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "bank.h"



void initialize_user(User *users, int user_id, const char *password) {
    if (users == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        if(users[i].user_id == 0){
            break;
        }
    }
    // Initialiser les champs du user
    users[i].user_id = user_id;
    strncpy(users[i].password, password, MAX_PASSWORD_LENGTH - 1);
    users[i].password[MAX_PASSWORD_LENGTH - 1] = '\0';
    users[i].num_accounts = 0;
    printf("Client créé avec succès : users[%d].user_id = %d \n",i,users[i].user_id);
    return; // Initialisation réussie
}

void add_account(User *users, int user_id, int account_id, const char *password) {
    printf("Entering add_account\n");
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    if (users == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }
    // Vérifier si le user a atteint le nombre maximum de comptes
    if (users[i].num_accounts >= MAX_ACCOUNTS) {
        printf("Nombre maximum de comptes atteint\n ");
        return; // Nombre maximum de comptes atteint
    }

    // Vérifier si le compte existe déjà
    for (int j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            printf("Le compte existe déjà\n ");
            return; // Le compte existe déjà
        }
    }

    // Ajouter le compte
    users[i].accounts[users[i].num_accounts].account_id = account_id;
    users[i].accounts[users[i].num_accounts].balance = 0.0;
    users[i].accounts[users[i].num_accounts].num_operations = 0;
    users[i].num_accounts++;
    printf("Compte créé avec succès\n");
    return; // Ajout de compte réussi
}

void add_operation(User *users, int index, int account_index, Operation_t type, float somme ){
    printf("ADD_OPEREATION de %.2f\n",somme);
    time_t t = time(NULL);
    users[index].accounts[account_index].operations[users[index].accounts[account_index].num_operations].type = type;
    users[index].accounts[account_index].operations[users[index].accounts[account_index].num_operations].somme = somme;

    struct tm *tm = localtime(&t);
    size_t ret = strftime(users[index].accounts[account_index].operations[users[index].accounts[account_index].num_operations].date, sizeof(char)*64, "%c", tm);
    assert(ret);
    users[index].accounts[account_index].num_operations += 1;
    return;
}


void ajout(User *users, int user_id, int account_id,  const char *password, char *res, double amount) {
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    printf("Translated : AJOUT %d %d %s %f\n",users[i].user_id, account_id, password, amount);

    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
        return;
    }
    // Rechercher le compte
    int j;  
    for (j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            users[i].accounts[j].balance += amount;
            strncpy(res,"OK : Opération réussie",255);
            add_operation(users,i,j,AJOUT,amount);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void retrait(User *users, int user_id, int account_id,  const char *password, char *res, double amount) {
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    printf("Translated : RETRAIT %d %d %s %f\n",users[i].user_id, account_id, password, amount);
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
        return;
    }
    // Rechercher le compte
    int j;  
    for (j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            users[i].accounts[j].balance -= amount;
            strncpy(res,"OK : Opération réussie",255);
            add_operation(users,i,j,RETRAIT,amount);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void solde(User *users, int user_id, int account_id, const char *password, char *res) {
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); 
        return;
    }

    // Rechercher le compte
    int j;  
    for (j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            snprintf(res,MAX_RES_SIZE,"RES_SOLDE %.2f %s\n", users[i].accounts[j].balance,users[i].accounts[j].operations[users[i].accounts[j].num_operations-1].date);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255);
    return;
}

void operations(User *users, int user_id, int account_id, const char *password, char *res) {
    int i = 0;
    for(i = 0; (size_t)i < sizeof(users); i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
        return;
    }
    
    // Rechercher le compte
    int j;  
    for (j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            break;
        }
        if(j > users[i].num_accounts){
            strncpy(res,"KO : Compte non trouvé",255); 
            return;
        }
    }

    printf("\nRES_OPERATIONS type_opération date_opération montant_operation\n");
    int start_index = users[i].accounts[j].num_operations - 10;
    if(start_index<0){start_index=0;}
    sprintf(res, "\nRES_OPERATIONS\n");
    for(int k = start_index; k < users[i].accounts[j].num_operations; k++){
        char res_begin[32];
        if((users[i].accounts[j].operations[k].type == AJOUT) ){
            snprintf(res_begin, sizeof(res_begin), "AJOUT ");
        }else if((users[i].accounts[j].operations[k].type == RETRAIT)){
            snprintf(res_begin, sizeof(res_begin), "RETRAIT ");
        }else{
            break;
        }
        strncat(res, res_begin, sizeof(res) - strlen(res) - 1);
        char res_end[512];
        snprintf(res_end, sizeof(res_end), "%.2f € %s\n", users[i].accounts[j].operations[k].somme, users[i].accounts[j].operations[k].date);
        strncat(res, res_end, sizeof(res) - strlen(res) - 1);
    }
    return;



}

void process_command(User *users,  char *buffer, char *res) {
    char command[20];
    int user_id, id_compte;
    char password[MAX_PASSWORD_LENGTH];
    double somme = 0.0; 
    int result = sscanf(buffer, "%s %d %d %s %lf", command, &user_id, &id_compte, password, &somme);
    printf("Scanned : %s %d %d %s %f\n",command, user_id, id_compte, password, somme);
    printf("Command : %s\n",command);
    if (result < 3) {
        strncpy(res,"KO : Le format de la commande est invalide",255);
    } 
    if (strcmp(command, "AJOUT") == 0 || strcmp(command, "RETRAIT") == 0) {
        if (result < 5) {
            strncpy(res,"KO : Pas suffisamment d'arguments pour AJOUT ou RETRAIT",255);
        }
    }
    // Traiter la commande en fonction du type
    if (strcmp(command, "AJOUT") == 0) {
        ajout(users, user_id, id_compte, password, res, somme);
    } else if (strcmp(command, "RETRAIT") == 0) {
        retrait(users, user_id, id_compte, password, res, somme);
    } else if (strcmp(command, "SOLDE") == 0) {
        solde(users, user_id, id_compte, password, res);
    } else if (strcmp(command, "OPERATIONS") == 0) {
        operations(users, user_id, id_compte, password, res);
    } else {
        strncpy(res,"KO : Commande inconnue",255);
    }
    printf("Response : %s\n",res);
    
    return;
}
