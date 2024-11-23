#pragma once

#include "utils.h"
#include "../model/model.h"

typedef struct {
    char *ip;
    int port;

    int serveur_fd;

    int nb_connect;
    int *clients_fd;
    char **names;

    int addrlen;
    struct sockaddr_in *address;
} server;

#define MAX_CLIENT 7


server *init_serveur(int port, int nb_connect);
void wait_for_connections(void (*on_connect)(char *message));
void close_connections(server *s);
void destroy_server(server *s);


direction get_direction_from(server *s, int connect);


void send_grid_to(server *s, int connect, int **grid);
void send_positions_to(server *s, int connect, position **positions);
void send_is_over_to(server *s, int connect, int est_fini);
void send_start_signal_to(server *s, int connect);
void send_names_to(server *s, int connect);
void send_winners_to(server *s, int connect, int *winners);


direction *get_directions_all(server *s);
void send_grid_all(server *s, int **grid);
void send_positions_all(server *s, position **positions);
void send_is_over_all(server *s, int est_fini);
void send_start_signal_all(server *s);
void send_names_all(server *s);
void send_winners_all(server *s, int *winners);




