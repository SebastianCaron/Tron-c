#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "server.h"

server *init_serveur(int port, int nb_connect){
    server *s = calloc(1, sizeof(server));
    if(!s){
        perror("ERREUR INITIALISATION SERVEUR");
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket;
    s->address = calloc(1, sizeof(struct sockaddr_in));
    if(!s->address){
        perror("ERREUR INITIALISATION STRUCT ADDRESS");
        exit(EXIT_FAILURE);
    }
    s->addrlen = sizeof(s->address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("ERREUR CREATION SOCKET");
        exit(EXIT_FAILURE);
    }

    s->port = port;
    s->serveur_fd = server_fd;

    (*s->address).sin_family = AF_INET;
    (*s->address).sin_addr.s_addr = INADDR_ANY;
    (*s->address).sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &(*s->address), s->addrlen) < 0) {
        perror("ERREUR BIND");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENT) < 0) {
        perror("ERREUR LISTEN");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("PRET POUR CONNEXIONS...\n");

    return s;
}

void close_connections(server *s){
    for(int i = 0; i < s->nb_connect; i++){
        close(s->clients_fd[i]);
    }
    close(s->serveur_fd);
}

void destroy_server(server *s){
    free(s->ip);
    free(s->clients_fd);
    for(int i = 0; i < s->nb_connect; i++){
        free(s->names[i]);
    }
    free(s->address);
    free(s);
}


direction get_direction_from(server *s, int connect){
    direction dir;
    ssize_t bytes_read = read(s->clients_fd[connect], &dir, sizeof(dir));
    if (bytes_read > 0) {
        return dir;
    } else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        // printf("NO DATA\n");
        return NODIRECTION;
    } else if (bytes_read == 0) {
        // printf("CONNEXION CLIENT %d TERMINE\n", connect);
        return NODIRECTION;
    } else {
        perror("ERREUR LECTURE");
        return NODIRECTION;
    }
}


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

