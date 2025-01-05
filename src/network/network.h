#pragma once

#include "../utils/utils.h"
#include "../model/model.h"

#define PORT 16385
#define MAX_CLIENT 7
#define TIMEOUT_RECEIVE 1000000

#define BLOC_BUFFER_SIZE 2048

typedef enum {NBJOUEUR = 1, GRID = 2, POSITIONS = 3, SCORES = 4, MOUVEMENT = 5, ISOVER = 6, START = 7, NEEDGRID = 8, WINNER = 9, IDSERV = 10, READY, ENDPACKET, SIZEGRID, NAME} f_packet;

/**
 * @struct server
 * @brief Represents the game server managing connections and game state
 */
typedef struct {
    char *ip;                         /**< IP address of the server */
    int port;                         /**< Port number for the server */

    int serveur_fd;                   /**< File descriptor for the server socket */

    int act_connect;                  /**< Number of active connections */
    int nb_connect;                   /**< Maximum number of connections allowed */
    int clients_fd[MAX_CLIENT + 1];   /**< Array of file descriptors for client sockets */
    char *names[MAX_CLIENT + 1];      /**< Array of client names */
    direction directions[MAX_CLIENT + 1]; /**< Array of directions for each client */
    model *m;                         /**< Pointer to the game model */
    int addrlen;                      /**< Length of the address structure */
    struct sockaddr_in *address;      /**< Pointer to the server address structure */
} server;

/**
 * @struct client
 * @brief Represents a game client that connects to the server and manages game state
 */
typedef struct {
    char *ip;                         /**< IP address of the server */
    int port;                         /**< Port number for the server */

    int serveur_fd;                   /**< File descriptor for the server socket */

    int id_on_serv;                   /**< Client ID on the server */

    grid *g;                          /**< Pointer to the game grid */
    int is_over;                      /**< Game over status */
    int scores[MAX_CLIENT + 1];       /**< Array of scores for each player */
    position pos[MAX_CLIENT + 1];     /**< Array of positions for each player */
    int nb_player;                    /**< Number of players */
    int has_started;                  /**< Game start status */
    char *names[MAX_CLIENT + 1];      /**< Array of player names */
    int winner;                       /**< Index of the winner */

    char data_available[15];          /**< Data available for reading */
    unsigned size_available;          /**< Size of available data */

    f_packet need_to_read_type;       /**< Type of packet to read */
    int *need_to_read_nbytes;         /**< Number of bytes to read */
} client;

void uni_start_server();
void uni_start_client();
void uni_stop_server();
void uni_stop_client();


int uni_socket(int, int, int);

int uni_nblock(int server_fd);

int uni_set_sockopt(int server_fd);

int uni_bind(server *);

int uni_listen(int server_fd, int mx_client);

void uni_close(int);

int uni_get_error();

int uni_accept(server *s);

int uni_select(server *s, fd_set *readfds, struct timeval *timeout);

ssize_t uni_write(int socket_fd, const void *buffer, size_t length);

ssize_t uni_read(int socket_fd, void *buffer, size_t length);