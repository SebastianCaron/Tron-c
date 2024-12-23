#pragma once

#include "network.h"
#include "../utils/utils.h"
#include "../model/model.h"

typedef struct {
    char *ip;
    int port;

    int serveur_fd;

    int nb_connect;
    int *clients_fd;
    char *names[MAX_CLIENT + 1];
    direction directions[MAX_CLIENT + 1];
    model *m;
    int addrlen;
    struct sockaddr_in *address;
} server;



server *init_serveur(int port, int nb_connect);
int wait_for_connections(server *s, void (*on_connect)(char *message));
void close_connections(server *s);
void destroy_server(server *s);


direction get_direction_from(server *s, int connect);


void send_grid_to(server *s, int connect, int nb_lignes, int nb_colonnes, int **grid);
void send_nb_player_to(server *s, int connect, char nb_player);
void send_positions_to(server *s, int connect,int nb_position, position **positions);
void send_is_over_to(server *s, int connect, int est_fini);
void send_start_signal_to(server *s, int connect);
void send_names_to(server *s, int connect);
void send_winner_to(server *s, int connect, int winners);
void send_scores_to(server *s, int connect, int nb_players, int *scores);


direction *get_directions_all(server *s);
void send_grid_all(server *s, int nb_lignes, int nb_colonnes, int **grid);
void send_nb_player_all(server *s, char nb_player);
void send_positions_all(server *s, int nb_position, position **positions);
void send_is_over_all(server *s, int est_fini);
void send_start_signal_all(server *s);
void send_names_all(server *s);
void send_winner_all(server *s, int winner);
void send_scores_all(server *s, int nb_players, int *scores);




