#pragma once

#include "network.h"
#include "../utils/utils.h"
#include "../utils/QStack.h"
#include "../model/model.h"

typedef struct {
    char *ip;
    int port;

    int serveur_fd;

    int id_on_serv;

    grid *g;
    int is_over;
    int *scores;
    position pos[MAX_CLIENT + 1];
    int nb_player;
    int has_started;
    char **names;
    int winner;

    char data_available[15];
    unsigned size;

    f_packet need_to_read_type;
    int *need_to_read_nbytes;
    
} client;

client *init_client(char *ip, int port);
void destroy_client(client *c);

grid *client_get_grid(client *c);
int client_is_over(client *c);
int *client_get_scores(client *c);
position *client_get_positions(client *c);
int client_get_nb_player(client *c);
int client_get_start_signal(client *c);
char **client_get_names(client *c);
int client_get_winner(client *c);


void client_send_movement(client *s, direction d);
void client_ask_for_grid(client *s);

