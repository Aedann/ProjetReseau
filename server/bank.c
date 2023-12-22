#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank.h"



void initialize_user(User *users, int user_id, const char *password) {
    if (users == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }
    int i = 0;
    while(users[i].user_id != NULL){
        i++;
        break;
    }
    // Initialiser les champs du user
    users[i].user_id = user_id;
    strncpy(users[i].password, password, MAX_PASSWORD_LENGTH - 1);
    users[i].password[MAX_PASSWORD_LENGTH - 1] = '\0';
    users[i].num_accounts = 0;
    printf("Client créé avec succès\n");
    return; // Initialisation réussie
}

void add_account(User *users, int account_id, const char *password) {
    if (users == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }
    int i = 0;
    while(users[i].user_id != NULL){
        i++;
        break;
    }

    // Vérifier si le user a atteint le nombre maximum de comptes
    if (users[i].num_accounts >= MAX_ACCOUNTS) {
        printf("Nombre maximum de comptes atteint\n ");
        return; // Nombre maximum de comptes atteint
    }

    // Vérifier si le compte existe déjà
    for (int j = 0; i < users[j].num_accounts; j++) {
        if (users[j].accounts[j].account_id == account_id) {
            printf("Le compte existe déjà\n ");
            return; // Le compte existe déjà
        }
    }

    // Ajouter le compte
    users[i].accounts[users[i].num_accounts].account_id = account_id;
    users[i].accounts[users[i].num_accounts].balance = 0.0;
    users[i].num_accounts++;
    printf("Compte créé avec succès\n");
    return; // Ajout de compte réussi
}

void ajout(User *users, int user_id, int account_id,  const char *password, char *res, double amount) {
    int i = 0;
    while(users[i].user_id == user_id){
        i++;
        break;
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    printf("Translated : AJOUT %d %d %s %f\n",users[i].user_id, account_id, password, amount);
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO",255); // Authentification échouée
    }

    // Rechercher le compte
    int j;  
    for (j = 0; j < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Ajouter la somme au solde du compte
            users[i].accounts[j].balance += amount;
            strncpy(res,"OK : Opération réussie",255);
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void retrait(User *users, int user_id, int account_id, const char *password, char *res, double amount) {
    int i = 0;
    while(users[i].user_id == user_id){
        i++;
        break;
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255);
    }

    // Rechercher le compte
    int j;
    for (j = 0; i < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Vérifier si le solde est suffisant
            if (users[i].accounts[j].balance >= amount) {
                // Retirer la somme du solde du compte
                users[i].accounts[j].balance -= amount;
                strncpy(res,"OK : Opération réussie",255);
            } else {
                strncpy(res,"KO : Solde insuffisant",255);
            }
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void solde(User *users, int user_id, int account_id, const char *password, char *res) {
    int i = 0;
    while(users[i].user_id == user_id){
        i++;
        break;
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); 
    }

    // Rechercher le compte
    int j;
    for (j = 0; i < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Formater la réponse
            sprintf(res,"RES_SOLDE %.2f date_dernière_opération\n", users[i].accounts[j].balance);
        }
    }

    strncpy(res,"KO : Compte non trouvé",255);
    return;
}

void operations(User *users, int user_id, int account_id, const char *password, char *res) {
    int i = 0;
    while(users[i].user_id == user_id){
        i++;
        break;
    }
    // Vérifier l'authentification du user (remplacez cela par votre logique d'authentification)
    if (strcmp(users[i].password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
    }

    // Rechercher le compte
    int j;
    for (j = 0; i < users[i].num_accounts; j++) {
        if (users[i].accounts[j].account_id == account_id) {
            // Formater la réponse (vous devez remplacer cela par votre logique pour récupérer les 10 dernières opérations)
            printf("RES_OPERATIONS type_opération date_opération montant_operation\n");
            strncpy(res,"OK : Opération réussie",255); 
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void process_command(User *users,  char *buffer, char *res) {
    // int i = 0;
    // while(users[i].user_id == user_id){
    //     i++;
    //     break;
    // }
    //printf("Received from %s: %s\n", client[i].name, buffer);
    fflush(stdout);
    char command[20];
    int user_id, id_compte;
    char password[MAX_PASSWORD_LENGTH];
    double somme = 0.0; // Initialiser à zéro
    // Utiliser sscanf pour extraire les arguments du buffer
    int result = sscanf(buffer, "%s %d %d %s %lf", command, &user_id, &id_compte, password, &somme);
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
