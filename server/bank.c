#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank.h"


int initialize_client(Client *client, int client_id, const char *password) {
    if (client == NULL || password == NULL) {
        return -1; // Erreur d'argument
    }

    // Initialiser les champs du client
    client->client_id = client_id;
    strncpy(client->password, password, MAX_PASSWORD_LENGTH - 1);
    client->password[MAX_PASSWORD_LENGTH - 1] = '\0';
    client->num_accounts = 0;

    return 0; // Initialisation réussie
}

int add_account(Client *client, int account_id, const char *password) {
    if (client == NULL || password == NULL) {
        return -1; // Erreur d'argument
    }

    // Vérifier si le client a atteint le nombre maximum de comptes
    if (client->num_accounts >= MAX_ACCOUNTS) {
        return -2; // Nombre maximum de comptes atteint
    }

    // Vérifier si le compte existe déjà
    for (int i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            return -3; // Le compte existe déjà
        }
    }

    // Ajouter le compte
    client->accounts[client->num_accounts].account_id = account_id;
    client->accounts[client->num_accounts].balance = 0.0;
    client->num_accounts++;

    return 0; // Ajout de compte réussi
}

int ajout(Client *client, int account_id,  const char *password, double amount) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        return -1; // Authentification échouée
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Ajouter la somme au solde du compte
            client->accounts[i].balance += amount;
            return 0; // Opération réussie
        }
    }

    return -2; // Compte non trouvé
}

int retrait(Client *client, int account_id, const char *password, double amount) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        return -1; // Authentification échouée
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Vérifier si le solde est suffisant
            if (client->accounts[i].balance >= amount) {
                // Retirer la somme du solde du compte
                client->accounts[i].balance -= amount;
                return 0; // Opération réussie
            } else {
                return -3; // Solde insuffisant
            }
        }
    }

    return -2; // Compte non trouvé
}

int solde(Client *client, int account_id, const char *password) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        return -1; // Authentification échouée
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Formater la réponse
            printf("RES_SOLDE %.2f date_dernière_opération\n", client->accounts[i].balance);
            return 0; // Opération réussie
        }
    }

    return -2; // Compte non trouvé
}

int operations(Client *client, int account_id, const char *password) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        return -1; // Authentification échouée
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Formater la réponse (vous devez remplacer cela par votre logique pour récupérer les 10 dernières opérations)
            printf("RES_OPERATIONS type_opération date_opération montant_operation\n");
            return 0; // Opération réussie
        }
    }

    return -2; // Compte non trouvé
}

int process_command(Client *client, char *buffer) {
    printf("Received from %s: %s\n", client->name, buffer);

    char command[20];
    int id_client, id_compte;
    char password[MAX_PASSWORD_LENGTH];
    double somme = 0.0; // Initialiser à zéro

    // Utiliser sscanf pour extraire les arguments du buffer
    int result = sscanf(buffer, "%s %d %d %s %lf", command, &id_client, &id_compte, password, &somme);

    if (result < 3) {
        // Le format de la commande est invalide
        return -1;
    }

    // Traiter la commande en fonction du type
    if (strcmp(command, "AJOUT") == 0 || strcmp(command, "RETRAIT") == 0) {
        if (result < 5) {
            // Pas suffisamment d'arguments pour AJOUT ou RETRAIT
            return -1;
        }
    }

    // Traiter la commande en fonction du type
    if (strcmp(command, "AJOUT") == 0) {
        return ajout(client, id_compte, password, somme);
    } else if (strcmp(command, "RETRAIT") == 0) {
        return retrait(client, id_compte, password, somme);
    } else if (strcmp(command, "SOLDE") == 0) {
        return solde(client, id_compte, password);
    } else if (strcmp(command, "OPERATIONS") == 0) {
        return operations(client, id_compte, password);
    } else {
        // Commande inconnue
        return -1;
    }
}
