#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structBank.h"
#include "bank.h"


void initialize_client(Client *client, int client_id, const char *password) {
    client->client_id = client_id;
    strncpy(client->password, password, sizeof(client->password) - 1);
    client->password[sizeof(client->password) - 1] = '\0';
    client->num_accounts = 0;
}

void add_account(Client *client, int account_id) {
    if (client->num_accounts < MAX_ACCOUNTS) {
        client->accounts[client->num_accounts].account_id = account_id;
        client->accounts[client->num_accounts].balance = 0.0;
        client->num_accounts++;
    }
}

void deposit(Client *client, int account_id, double amount) {
    for (int i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            client->accounts[i].balance += amount;
            printf("Deposit of %.2f to account %d successful. New balance: %.2f\n", amount, account_id, client->accounts[i].balance);
            return;
        }
    }

    printf("Account %d not found for client %d.\n", account_id, client->client_id);
}

