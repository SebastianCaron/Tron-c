#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "server.h"
#include "utils.h"

server *init_serveur(int port, int nb_connect){
    server *s = calloc(1, sizeof(server));
    if(!s){
        perror("ERREUR INITIALISATION SERVEUR");
        exit(EXIT_FAILURE);
    }

    int server_fd;
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

int wait_for_connections(server *s, void (*on_connect)(char *)){
    int new_socket = 0;
    if ((new_socket = accept(s->serveur_fd, (struct sockaddr *)&(s->address), (socklen_t*)&(s->addrlen))) < 0) {
        perror("Erreur lors de l'acceptation de la connexion");
        close(s->serveur_fd);
        exit(EXIT_FAILURE);
    }
    if(new_socket != 0){
        if(on_connect != NULL){
            // TODO : TRANSFORM address to char *
            on_connect("CLIENT CONNECTE ! ");
        }
        return 1;
    }
    return 0;
}

char *grid_to_buffer(int nb_lignes, int nb_colonnes, int **grid){
    int *res = calloc(3 + nb_lignes * nb_colonnes, sizeof(int));
    if(res == NULL){
        perror("[SERVER] ERREUR ALLOC GRID BUFFER\n");
        return NULL;
    }

    res[0] = GRID;
    res[1] = nb_lignes;
    res[2] = nb_colonnes;

    for(int i = 0; i < nb_lignes; i++){
        for(int j = 0; j < nb_colonnes; j++){
            res[i*nb_colonnes + j +3] = grid[i][j];
        }
    }

    return (char *) res;
}

void send_grid_to(server *s, int connect, int nb_lignes, int nb_colonnes, int **grid){
    int size = (3 + nb_lignes * nb_colonnes) * sizeof(int);
    char *buffer = grid_to_buffer(nb_lignes, nb_colonnes, grid);
    if(buffer == NULL) return;
    write(s->clients_fd[connect], buffer, size);
    free(buffer);
}

char *positions_to_buffer(int nb_position, position **positions){
    int *res = calloc(2 + nb_position*2, sizeof(int));
    if(res == NULL){
        perror("[SERVER] ERREUR ALLOC GRID BUFFER\n");
        return NULL;
    }

    res[0] = POSITIONS;
    res[1] = nb_position;

    for(int i = 0; i < nb_position; i++){
        res[2 * i + 2] = positions[i]->x;
        res[2 * i + 3] = positions[i]->y;
    }
    return (char *) res;
}

void send_positions_to(server *s, int connect, int nb_position, position **positions){
    int size = (2 + nb_position*2) * sizeof(int);
    char *buffer = positions_to_buffer(nb_position, positions);
    if(buffer == NULL) return;
    write(s->clients_fd[connect], buffer, size);
    free(buffer);
}

void send_is_over_to(server *s, int connect, int est_fini){
    int size = 2 * sizeof(int);
    int *buffer = calloc(2, sizeof(int));
    if(buffer == NULL) return;

    buffer[0] = ISOVER;
    buffer[1] = est_fini;

    write(s->clients_fd[connect], (char *) buffer, size);
    free(buffer);
}

void send_start_signal_to(server *s, int connect){
    int size = sizeof(int);
    int *buffer = calloc(1, sizeof(int));
    if(buffer == NULL) return;

    buffer[0] = START;

    write(s->clients_fd[connect], (char *) buffer, size);
    free(buffer);
}

void send_names_to(server *s, int connect){
    
}

void send_winners_to(server *s, int connect, int winner){
    int size = 2 * sizeof(int);
    int *buffer = calloc(2, sizeof(int));
    if(buffer == NULL) return;

    buffer[0] = WINNER;
    buffer[1] = winner;

    write(s->clients_fd[connect], (char *) buffer, size);
    free(buffer);
}


direction *get_directions_all(server *s){
    direction *dirs = calloc(s->nb_connect, sizeof(direction));
    if(dirs == NULL) return NULL;

    for(int i = 0; i < s->nb_connect; i++){
        dirs[i] = get_direction_from(s, i);
    }

    return dirs;
}
void send_grid_all(server *s, int nb_lignes, int nb_colonnes, int **grid){
    for(int i = 0; i < s->nb_connect; i++){
        send_grid_to(s, i, nb_lignes, nb_colonnes, grid);
    }
}
void send_positions_all(server *s, int nb_position, position **positions){
    for(int i = 0; i < s->nb_connect; i++){
        send_positions_to(s, i, nb_position, positions);
    }
}
void send_is_over_all(server *s, int est_fini){
    for(int i = 0; i < s->nb_connect; i++){
        send_is_over_to(s, i, est_fini);
    }
}
void send_start_signal_all(server *s){
    for(int i = 0; i < s->nb_connect; i++){
        send_start_signal_to(s, i);
    }
}
void send_names_all(server *s){

}
void send_winner_all(server *s, int winner){
    for(int i = 0; i < s->nb_connect; i++){
        send_winners_to(s, i, winner);
    }
}


