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
    if(c == NULL){
        perror("ERREUR INITIALISATION CLIENT");
        exit(EXIT_FAILURE);
    }

    c->need_to_read_type = -1;

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

grid *buffer_to_grid(int nb_lignes, int nb_colonnes, char *c){

}

// TODO CHECK ERRORS & TIMEOUT
// SI TROP LENT PAS DE CHECK READY
void retrieve_data(client *c){
    // SI PAS DANS LES DATAS AVAILABLES, ON READ ET ON AJOUTE AUX DATA AV
    // TANT QUE PAS IN AVAILABLE ON RETRIEVE tant pis
    char f_type = 0;
    int rd_size = read(c->serveur_fd, &f_type, sizeof(char));
    if(rd_size == 0) return;

    switch (f_type)
    {
    case IDSERV:
        int size = 2 * sizeof(char);
        char buffer[2] = {0};
        rd_size = read(c->serveur_fd, buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, buffer+rd_size, size);
        }
        c->id_on_serv = buffer[0];
        break;
    case NBJOUEUR:
        break;
    case START:
        int size = 1 * sizeof(char);
        char buffer = 0;
        rd_size = read(c->serveur_fd, &buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, buffer+rd_size, size);
        }
        c->has_started = 1;
        break;
    case POSITIONS:
        int size = (c->nb_player * 2 + 2) * sizeof(int);
        char *buffer = calloc((c->nb_player * 2 + 2), sizeof(int));
        rd_size = read(c->serveur_fd, &buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, buffer+rd_size, size);
        }
        int *c_buf = (int *) buffer;
        int nb_pos = c_buf[0];
        c_buf++;
        for(int i = 0; i < nb_pos; i++){
            c->pos[i].x = c_buf[2 * i];
            c->pos[i].y = c_buf[2 * i + 1];
        }
        c->data_available[c->size_available++] = POSITIONS;
        free(buffer);
        break;
    default:
        break;
    }
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
