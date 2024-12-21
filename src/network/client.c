#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "client.h"
#include "network.h"

client *init_client(char *ip, int port){
    client *c = calloc(1, sizeof(client));
    if(!c){
        perror("ERREUR INITIALISATION CLIENT");
        exit(EXIT_FAILURE);
    }

    c->ip = ip;
    c->port = port;
    struct sockaddr_in serv_addr;

    if ((c->serveur_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la creation du socket\n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("Adresse non valide\n");
        return NULL;
    }

    if (connect(c->serveur_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur de connexion\n");
        return NULL;
    }

    return c;
}

void destroy_client(client *c){
    close(c->serveur_fd);
    free(c);
}

void retrieve_data(){
    
}

grid *client_get_grid(client *c);
int client_is_over(client *c);
int *client_get_scores(client *c);
position *client_get_positions(client *c);
int client_get_nb_player(client *c);
int client_get_start_signal(client *c);
char **client_get_names(client *c);
int client_get_winner(client *c);


void client_send_movement(client *s, direction d){

}
void client_ask_for_grid(client *s);
