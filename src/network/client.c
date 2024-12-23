#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "client.h"
#include "network.h"
#include "../utils/utils.h"

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
    grid *ng = init_grid(nb_lignes, nb_colonnes);
    if(ng == NULL) return NULL;
    int *buffer = (int *) c;

    for(int i = 0; i < nb_lignes; i++){
        for(int j = 0; j < nb_colonnes; j++){
            ng->grid[i][j] = buffer[ (i * nb_colonnes) + j];
        }
    }
    return ng;
}

// TODO CHECK ERRORS & TIMEOUT
// SI TROP LENT PAS DE CHECK READY
void retrieve_data_client(client *c){
    // SI PAS DANS LES DATAS AVAILABLES, ON READ ET ON AJOUTE AUX DATA AV
    // TANT QUE PAS IN AVAILABLE ON RETRIEVE tant pis
    char f_type = 0;
    int rd_size = read(c->serveur_fd, &f_type, sizeof(char));
    if(rd_size == 0) return;

    switch (f_type)
    {
    case IDSERV:
        {int size = 2 * sizeof(char);
        char buffer[2] = {0};
        rd_size = read(c->serveur_fd, buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, buffer+rd_size, size);
        }
        c->id_on_serv = buffer[0];
        c->data_available[c->size_available++] = IDSERV;}
        break;
    case NBJOUEUR:
        {int size = 2 * sizeof(char);
        char buffer[2] = {0};
        rd_size = read(c->serveur_fd, buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, buffer+rd_size, size);
        }
        c->nb_player = buffer[0];
        c->data_available[c->size_available++] = NBJOUEUR;}
        break;
    case START:
        {int size = 1 * sizeof(char);
        char buffer = 0;
        rd_size = read(c->serveur_fd, &buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, ((char *) &buffer)+rd_size, size);
        }
        c->has_started = 1;
        c->data_available[c->size_available++] = START;}
        break;
    case POSITIONS:
        {int size = (c->nb_player * 2 + 2) * sizeof(int);
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
        free(buffer);}
        break;
    case GRID:
        {int size = 2 * sizeof(int);
        int buffer[2] = {0};
        rd_size = read(c->serveur_fd, &buffer, size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, ((char *) buffer)+rd_size, size);
        }

        int nb_lignes = buffer[0];
        int nb_colonnes = buffer[1];

        int g_size = (nb_lignes * nb_colonnes + 1) * sizeof(int);
        char *grid_buff = calloc(nb_lignes * nb_colonnes + 1, sizeof(int));
        if(grid_buff != NULL){
            rd_size = read(c->serveur_fd, &grid_buff, g_size);
            while(rd_size < g_size){
                rd_size += read(c->serveur_fd, grid_buff+rd_size, g_size);
            }

            grid *g = buffer_to_grid(nb_lignes, nb_colonnes, grid_buff);
            if(g != NULL) c->g = g;
            free(grid_buff);
            c->data_available[c->size_available++] = GRID;
        }
        else{
            // READ LES DATAS POUR VIDER LE FD
            char buffer[BLOC_BUFFER_SIZE];
            int size = BLOC_BUFFER_SIZE * sizeof(char);
            rd_size = read(c->serveur_fd, &buffer, size);
            while(rd_size < g_size){
                rd_size += read(c->serveur_fd, grid_buff+rd_size, size);
            }
        }
        }
        break;
    case SCORES:
        {int size = sizeof(int);
        int nb_p = 0;
        rd_size = read(c->serveur_fd, ((char *) &nb_p), size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, ((char *) &nb_p)+rd_size, size);
        }

        int scores_buff[MAX_CLIENT + 2] = {0};
        int s_size = (nb_p + 1) * sizeof(int);
        rd_size = read(c->serveur_fd, ((char *) &scores_buff), s_size);
        while(rd_size < s_size){
            rd_size += read(c->serveur_fd, ((char *) scores_buff)+rd_size, s_size);
        }

        for(int i = 0; i < nb_p; i++){
            c->scores[i] = scores_buff[i];
        }
        c->data_available[c->size_available++] = SCORES;}
        break;
    case WINNER:
        {int size = 2 * sizeof(int);
        int buffer[2] = {0};
        rd_size = read(c->serveur_fd, ((char *) buffer), size);
        while(rd_size < size){
            rd_size += read(c->serveur_fd, ((char *) buffer)+rd_size, size);
        }
        c->winner = buffer[0];
        c->data_available[c->size_available++] = WINNER;}
        break;
    case NAME:
        break;
    default:
        break;
    }
}

int pop_is_in_data_available(client *c, int DATA){
    for(int i = 0; i < c->size_available; i++){
        if(c->data_available[i] == DATA){
            c->data_available[i] = c->data_available[c->size_available - 1];
            c->size_available--;
            return 1;
        }
    }
    return 0;
}

grid *client_get_grid(client *c){
    if(pop_is_in_data_available(c, GRID)) return c->g;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, GRID));
    return c->g;
}

int client_is_over(client *c){
    if(pop_is_in_data_available(c, ISOVER)) return c->is_over;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, ISOVER));
    return c->is_over;
}
int *client_get_scores(client *c){
    if(pop_is_in_data_available(c, SCORES)) return c->scores;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, SCORES));
    return c->scores;
}
position *client_get_positions(client *c){
    if(pop_is_in_data_available(c, POSITIONS)) return c->pos;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, POSITIONS));
    return c->pos;
}
int client_get_nb_player(client *c){
    if(pop_is_in_data_available(c, NBJOUEUR)) return c->nb_player;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, NBJOUEUR));
    return c->nb_player;
}
int client_get_start_signal(client *c){
    if(pop_is_in_data_available(c, START)) return c->has_started;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, START));
    return c->has_started;
}
char **client_get_names(client *c){
    if(pop_is_in_data_available(c, NAME)) return c->names;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, NAME));
    return c->names;
}
int client_get_winner(client *c){
    if(pop_is_in_data_available(c, WINNER)) return c->winner;

    do{
        retrieve_data_client(c);
    }while(!pop_is_in_data_available(c, WINNER));
    return c->winner;
}


void client_send_movement(client *c, direction d){
    int size = 4 * sizeof(char);
    char buffer[4] = {MOUVEMENT, c->id_on_serv, d, ENDPACKET};
    write(c->serveur_fd, (char *) buffer, size);
}

void client_ask_for_grid(client *c){
    int size = 3 * sizeof(char);
    char buffer[3] = {GRID, c->id_on_serv, ENDPACKET};
    write(c->serveur_fd, (char *) buffer, size);
}
