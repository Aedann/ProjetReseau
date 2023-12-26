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
    // printf("users[6].user_id = %d\n",users[6].user_id);
    // printf("&users = %d\n",users);
    // printf("Entering initialize_user : \n sizeof(users) : %d\n",sizeof(users));
    int i = 0;
    for(i = 0; i < MAX_USERS; i++){
        printf("i = %d\n",i);
        if(users[i].user_id == 0){
            printf("BREAK Initialize_user\n");
            fflush(stdout);
            break;
        }
    }
    // Initialiser les champs du user
    users[i].user_id = user_id;
    printf("\n users[%d].user_id = %d , user_id = %d\n",i,users[i].user_id,user_id);
    strncpy(users[i].password, password, MAX_PASSWORD_LENGTH - 1);
    users[i].password[MAX_PASSWORD_LENGTH - 1] = '\0';
    users[i].num_accounts = 0;
    printf("Client créé avec succès : users[%d].user_id = %d \n",i,users[i].user_id);
    return; // Initialisation réussie
}

void add_account(User *users, int user_id, int account_id, const char *password) {
    printf("Entering add_account\n");
    int i = 0;
    printf("users[0].user_id = %d\n",users[0].user_id);
    for(i = 0; i < MAX_USERS; i++){
        printf("\n FOR users[%d].user_id = %d , user_id = %d, i = %d\n",i,users[i].user_id,user_id,i);
        if(users[i].user_id == user_id){
            break;
        }
    }
    printf("i = %d \n",i);
    if (users == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }
    printf("i = %d \n",i);
    // Vérifier si le user a atteint le nombre maximum de comptes
    if (users[i].num_accounts >= MAX_ACCOUNTS) {
        printf("Nombre maximum de comptes atteint\n ");
        return; // Nombre maximum de comptes atteint
    }

    printf("account check \n");
    printf("i = %d \n",i);
    fflush(stdout);
    // Vérifier si le compte existe déjà
    for (int j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            printf("Le compte existe déjà\n ");
            return; // Le compte existe déjà
        }
    }

    // Ajouter le compte
    printf("i = %d \n",i);
    fflush(stdout);
    printf("users[%d].user_id = %d, NUM Account avant : %d, users[i].accounts[0] = %d\n",i, users[i].user_id, users[i].num_accounts);    
    fflush(stdout);
    users[i].accounts[users[i].num_accounts].account_id = account_id;
    printf("0 \n",i);
    fflush(stdout);
    users[i].accounts[users[i].num_accounts].balance = 0.0;
    printf("1 \n",i);    
    fflush(stdout);
    printf("users[%d].user_id = %d, NUM Account avant : %d\n",i, users[i].user_id, users[i].num_accounts);    
    users[i].num_accounts++;
    printf("users[%d].user_id = %d, NUM Account après : %d\n",i, users[i].user_id, users[i].num_accounts);
    printf("Compte créé avec succès\n");
    return; // Ajout de compte réussi
}

void add_operation(User *users, int index, int account_index, Operation_t type, float somme ){
    printf("ADD_OPEration de %.6f\n",somme);
    time_t t = time(NULL);
    int o = 0;
    while((users[index].accounts[account_index].operations[o].type =! AJOUT)&&(users[index].accounts[account_index].operations[o].type =! RETRAIT)&&(users[index].accounts[account_index].operations[o].somme == 0.0)){
        o++;
    } //Dernière Opération
    users[index].accounts[account_index].operations[o].type = type;
    users[index].accounts[account_index].operations[o].somme = somme;
    struct tm *tm = localtime(&t);
    size_t ret = strftime(users[index].accounts[account_index].operations[o].date, sizeof(char)*64, "%c", tm);
    assert(ret);
    return;
}


void ajout(User *users, int user_id, int account_id,  const char *password, char *res, double amount) {
    int i = 0;
    printf("users[0].user_id = %d\n",users[0].user_id);
    for(i = 0; i < MAX_USERS; i++){
        printf("\n FOR users[%d].user_id = %d , user_id = %d, i = %d\n",i,users[i].user_id,user_id,i);
        if(users[i].user_id == user_id){
            break;
        }
    }
    printf("\n users[%d].user_id = %d , user_id = %d\n",i,users[i].user_id,user_id);
    printf("Translated : AJOUT %d %d %s %f\n",users[i].user_id, account_id, password, amount);
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO",255); // Authentification échouée
    }
    // Rechercher le compte
    int j;  
    printf("SEEKING ACCOUNT --------\n");
    printf("account_id : %d, amount = %d, users[%].num_accounts = %d\n", account_id,amount,users[i].num_accounts);
    for (j = 0; j < users[i].num_accounts; j++) {
        printf("users[i].accounts[j].account_id = %d",users[i].accounts[j].account_id);
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            users[i].accounts[j].balance += amount;
            strncpy(res,"OK : Opération réussie",255);
            printf("YOOOOOOOOOO \n");
            add_operation(users,i,account_id,AJOUT,amount);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void retrait(User *users, int user_id, int account_id,  const char *password, char *res, double amount) {
    int i = 0;
    printf("users[0].user_id = %d\n",users[0].user_id);
    for(i = 0; i < MAX_USERS; i++){
        printf("\n FOR users[%d].user_id = %d , user_id = %d, i = %d\n",i,users[i].user_id,user_id,i);
        if(users[i].user_id == user_id){
            break;
        }
    }
    printf("\n users[%d].user_id = %d , user_id = %d\n",i,users[i].user_id,user_id);
    printf("Translated : AJOUT %d %d %s %f\n",users[i].user_id, account_id, password, amount);
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO",255); // Authentification échouée
    }
    // Rechercher le compte
    int j;  
    printf("SEEKING ACCOUNT --------\n");
    printf("account_id : %d, amount = %d, users[%].num_accounts = %d\n", account_id,amount,users[i].num_accounts);
    for (j = 0; j < users[i].num_accounts; j++) {
        printf("users[i].accounts[j].account_id = %d",users[i].accounts[j].account_id);
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            users[i].accounts[j].balance -= amount;
            strncpy(res,"OK : Opération réussie",255);
            printf("YOOOOOOOOOO \n");
            add_operation(users,i,account_id,RETRAIT,amount);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void solde(User *users, int user_id, int account_id, const char *password, char *res) {
    printf("SOLDEEEE : \n");
    int i = 0;
    printf("users[0].user_id = %d\n",users[0].user_id);
    for(i = 0; i < MAX_USERS; i++){
        printf("\n FOR users[%d].user_id = %d , user_id = %d, i = %d\n",i,users[i].user_id,user_id,i);
        if(users[i].user_id == user_id){
            break;
        }
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); 
    }

    // Rechercher le compte
    int j;  
    printf("SEEKING ACCOUNT --------\n");
    printf("account_id : %d,  users[%].num_accounts = %d\n", account_id,users[i].num_accounts);
    for (j = 0; j < users[i].num_accounts; j++) {
        printf("users[i].accounts[j].account_id = %d",users[i].accounts[j].account_id);
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            snprintf(res,MAX_RES_SIZE,"RES_SOLDE %f date_dernière_opération\n", users[i].accounts[j].balance);
            return;
        }
    }

    strncpy(res,"KO : Compte non trouvé",255);
    return;
}

void operations(User *users, int user_id, int account_id, const char *password, char *res) {
    int i = 0;
    for(i = 0; i < sizeof(users); i++){
        if(users[i].user_id == user_id){
            break;
        }
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
    }
    
    // Rechercher le compte
    int j;  
    printf("SEEKING ACCOUNT --------\n");
    printf("account_id : %d,  users[%].num_accounts = %d\n", account_id,users[i].num_accounts);
    for (j = 0; j < users[i].num_accounts; j++) {
        printf("users[i].accounts[j].account_id = %d",users[i].accounts[j].account_id);
        if (users[i].accounts[j].account_id == account_id) {
            break;
        }
        if(j > users[i].num_accounts){
            strncpy(res,"KO : Compte non trouvé",255); 
            return;
        }
    }

    printf("\nRES_OPERATIONS type_opération date_opération montant_operation\n");
    for(int k = 0; k < MAX_OPERATIONS-1; k++){
        if(users[i].accounts[j].operations[k].type == AJOUT){
            printf("Y A UN AJOUT LA\n");
            snprintf(res,MAX_RES_SIZE,"\nAJOUT, de : ");
        }else if(users[i].accounts[j].operations[k].type == RETRAIT){
            printf("Y A UN RERTAIT LA\n");
            snprintf(res,MAX_RES_SIZE,"\nRETRAIT, de : ");
        }else{
            break;
        }
        printf("Test k = %d\n",k);
        fflush(stdout);
        snprintf(res,MAX_RES_SIZE,"%s %s",res,(users[i].accounts[j].operations[k].somme));
        strcat(res," €, le : ");
        snprintf(res,MAX_RES_SIZE,"%s %s",res,(users[i].accounts[j].operations[k].date));
    }
    strcat(res,"\n");
    return;



}

void process_command(User *users,  char *buffer, char *res) {
    //printf("Received from %s: %s\n", client[i].name, buffer);
    fflush(stdout);
    char command[20];
    int user_id, id_compte;
    char password[MAX_PASSWORD_LENGTH];
    double somme = 0.0; // Initialiser à zéro
    // Utiliser sscanf pour extraire les arguments du buffer
    int result = sscanf(buffer, "%s %d %d %s %lf", command, &user_id, &id_compte, password, &somme);
    printf("Scanned : %s %d %d %s %f\n",command, user_id, id_compte, password, somme);
    printf("Command : %s\n",command);
    if (result < 3) {
        strncpy(res,"KO : Le format de la commande est invalide",255);
    }
    // Traiter la commande en fonction du type
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
