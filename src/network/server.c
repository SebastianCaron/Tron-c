#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/select.h>


#include "server.h"
#include "network.h"

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
    s->addrlen = sizeof(struct sockaddr_in);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("ERREUR CREATION SOCKET");
        exit(EXIT_FAILURE);
    }
    // Mettre la socket en mode non bloquant
    if (fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK) < 0) {
        perror("ERREUR MODE NON BLOQUANT");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("ERREUR SETSOCKOPT");
        close(server_fd);
        free(s->address);
        free(s);
        exit(EXIT_FAILURE);
    }

    s->port = port;
    s->serveur_fd = server_fd;
    s->act_connect = 0;
    s->nb_connect = nb_connect;

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
    free(s->address);

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
    // free(s->clients_fd);
    for(int i = 0; i < s->nb_connect; i++){
        free(s->names[i]);
    }
    // free(s->address);
    free(s);
}


direction get_direction_from(server *s, int connect){
    return s->directions[connect];
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
        }else{
            printf("CLIENT CONNECTE !\n");
        }
        s->clients_fd[s->act_connect] = new_socket;
        int buffer[3] = {IDSERV, s->act_connect, ENDPACKET};
        write(new_socket, (char *) buffer, 3 * sizeof(int));
        s->act_connect++;
        return 1;
    }
    return 0;
}


int wait_for_connections_timeout(server *s, void (*on_connect)(char *)) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(s->serveur_fd, &readfds);

    int activity = select(s->serveur_fd + 1, &readfds, NULL, NULL, &timeout);
    if (activity < 0) {
        perror("Erreur avec select");
        return -1;
    }

    if (activity == 0) {
        return 0;
    }

    if (FD_ISSET(s->serveur_fd, &readfds)) {
        int new_socket = accept(s->serveur_fd, (struct sockaddr *)&(s->address), (socklen_t *)&(s->addrlen));
        if (new_socket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return 0;
            }
            perror("Erreur lors de l'acceptation de la connexion");
            return -1;
        }

        if (on_connect != NULL) {
            on_connect("CLIENT CONNECTE !");
        } else {
            printf("CLIENT CONNECTE !\n");
        }

        s->clients_fd[s->act_connect] = new_socket;
        int buffer[3] = {IDSERV, s->act_connect, ENDPACKET};
        if (write(new_socket, (char *)buffer, 3 * sizeof(int)) < 0) {
            perror("Erreur lors de l'écriture sur le socket client");
            close(new_socket);
            return -1;
        }
        s->act_connect++;

        return 1;
    }

    return 0;
}



char *grid_to_buffer(int nb_lignes, int nb_colonnes, int **grid){
    int *res = calloc(4 + nb_lignes * nb_colonnes, sizeof(int));
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

    res[nb_lignes * nb_colonnes + 3] = ENDPACKET;

    return (char *) res;
}

void send_grid_to(server *s, int connect, int nb_lignes, int nb_colonnes, int **grid){
    int size = (4 + nb_lignes * nb_colonnes) * sizeof(int);
    char *buffer = grid_to_buffer(nb_lignes, nb_colonnes, grid);
    if(buffer == NULL) return;
    write(s->clients_fd[connect], buffer, size);
    free(buffer);
}

void send_nb_player_to(server *s, int connect, char nb_player){
    int size = 3 * sizeof(int);
    int buffer[3] = {NBJOUEUR, nb_player, ENDPACKET};
    write(s->clients_fd[connect], buffer, size);
}

char *positions_to_buffer(int nb_position, position **positions){
    int *res = calloc(3 + nb_position*2, sizeof(int));
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

    res[nb_position*2 + 2] = ENDPACKET;

    return (char *) res;
}

void send_positions_to(server *s, int connect, int nb_position, position **positions){
    int size = (3 + nb_position*2) * sizeof(int);
    char *buffer = positions_to_buffer(nb_position, positions);
    if(buffer == NULL) return;
    write(s->clients_fd[connect], buffer, size);
    free(buffer);
}

void send_is_over_to(server *s, int connect, int est_fini){
    int size = 3 * sizeof(int);
    int buffer[3] = {ISOVER, est_fini, ENDPACKET};

    write(s->clients_fd[connect], (char *) buffer, size);
}

void send_start_signal_to(server *s, int connect){
    int size = 2 * sizeof(int);
    int buffer[2] = {START, ENDPACKET};

    write(s->clients_fd[connect], (char *) buffer, size);
}

void send_names_to(server *s, int connect){
    
}

void send_winner_to(server *s, int connect, int winner){
    int size = 3 * sizeof(int);
    int buffer[3] = {WINNER, winner, ENDPACKET};

    write(s->clients_fd[connect], (char *) buffer, size);
}

void send_scores_to(server *s, int connect, int nb_players, int *scores){
    int size = (2 + nb_players) * sizeof(int);
    int *buffer = calloc(2 + nb_players, sizeof(int));
    if(buffer == NULL) return;
    buffer[0] = SCORES;
    for(int i = 0; i < nb_players; i++){
        buffer[i + 1] = scores[i];
    }
    buffer[nb_players + 1] = ENDPACKET;

    write(s->clients_fd[connect], (char *) buffer, size);
    free(buffer);
}

void retrieve_data_server(server *s){
    char f_type = 0;
    for(int i = 0; i < s->nb_connect-1; i++){
        int rd_size = read(s->clients_fd[i], &f_type, sizeof(char));
        // printf("READ : %d, NB :%d\n", f_type, rd_size);
        if(rd_size == 0) return;
        if(rd_size == -1){
            printf("Erreur lors de la lecture : %s\n", strerror(errno));
            return;
        }

        switch (f_type)
        {
        case MOUVEMENT:
            {
            // printf("MOVEMENT RECEIVED!\n");
            int size = 3 * sizeof(char);
            char buffer[3] = {0};
            rd_size = read(s->clients_fd[i], buffer, size);
            while(rd_size < size){
                rd_size += read(s->clients_fd[i], ((char *) buffer) + rd_size, size);
            }
            s->directions[(int)buffer[0]] = buffer[1];
            }
            break;
        case GRID:
            {int size = 2 * sizeof(char);
            char buffer[2] = {0};
            rd_size = read(s->clients_fd[i], ((char *) buffer), size);
            while(rd_size < size){
                rd_size += read(s->clients_fd[i], ((char *) buffer) + rd_size, size);
            }
            send_grid_to(s, buffer[0],s->m->nb_lignes_grid, s->m->nb_colonnes_grid, s->m->grid);}
            break;
        default:
            break;
        }
        // printf("RETRIEVE DATA!\n");
    }
    // retrieve_data_server(s);
    // printf("END\n");
    
}

direction *get_directions_all(server *s){
    for(int i = 0; i < MAX_CLIENT+1; i++){
        s->directions[i] = NODIRECTION;
    }
    retrieve_data_server(s);
    return s->directions;
}
void send_grid_all(server *s, int nb_lignes, int nb_colonnes, int **grid){
    int size = (4 + nb_lignes * nb_colonnes) * sizeof(int);
    char *buffer = grid_to_buffer(nb_lignes, nb_colonnes, grid);
    if(buffer == NULL) return;
    for(int i = 0; i < s->nb_connect; i++){
        write(s->clients_fd[i], buffer, size);
    }
    free(buffer);
}
void send_nb_player_all(server *s, char nb_player){
    for(int i = 0; i < s->nb_connect; i++){
        send_nb_player_to(s, i, nb_player);
    }
}
void send_positions_all(server *s, int nb_position, position **positions){
    int size = (3 + nb_position*2) * sizeof(int);
    char *buffer = positions_to_buffer(nb_position, positions);
    if(buffer == NULL) return;
    for(int i = 0; i < s->nb_connect; i++){
        write(s->clients_fd[i], buffer, size);
    }
    free(buffer);
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
        send_winner_to(s, i, winner);
    }
}

void send_scores_all(server *s, int nb_players, int *scores){
    for(int i = 0; i < s->nb_connect; i++){
        send_scores_to(s, i, nb_players, scores);
    }
}