#pragma once

/**
 * @file server.h
 * @date 2024-12-27
 * @brief Header file for the game server.
 * 
 * This file contains the declarations of structures and functions 
 * related to the game server, which manages client connections, 
 * handles game state updates, and coordinates communication between 
 * the server and clients.
 * 
 * @author Sebastian Caron
 */

#include "network.h"
#include "../utils/utils.h"
#include "../model/model.h"

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
 * @brief Initializes the server with the specified port and number of connections
 * @param port The port number for the server
 * @param nb_connect The maximum number of connections allowed
 * @return A pointer to the initialized server
 */
server *init_serveur(int port, int nb_connect);

/**
 * @brief Waits for client connections
 * @param s Pointer to the server
 * @param on_connect Callback function to be called when a client connects
 * @return 1 on success, 0 no connection
 */
int wait_for_connections(server *s, void (*on_connect)(char *));

/**
 * @brief Waits for client connections with a timeout
 * @param s Pointer to the server
 * @param on_connect Callback function to be called when a client connects
 * @return 1 on success, 0 no connection
 */
int wait_for_connections_timeout(server *s, void (*on_connect)(char *));

/**
 * @brief Closes all client connections
 * @param s Pointer to the server
 */
void close_connections(server *s);

/**
 * @brief Frees the memory allocated for the server
 * @param s Pointer to the server to be destroyed
 */
void destroy_server(server *s);

/**
 * @brief Gets the direction from a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @return The direction received from the client
 */
direction get_direction_from(server *s, int connect);

/**
 * @brief Sends the game grid to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 */
void send_grid_to(server *s, int connect, int nb_lignes, int nb_colonnes, int **grid);

/**
 * @brief Sends the number of players to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param nb_player Number of players
 */
void send_nb_player_to(server *s, int connect, char nb_player);

/**
 * @brief Sends player positions to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param nb_position Number of player positions
 * @param positions Array of player positions
 */
void send_positions_to(server *s, int connect, int nb_position, position **positions);

/**
 * @brief Sends the game over status to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param est_fini Game over status
 */
void send_is_over_to(server *s, int connect, int est_fini);

/**
 * @brief Sends the start signal to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 */
void send_start_signal_to(server *s, int connect);

/**
 * @brief Sends player names to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 */
void send_names_to(server *s, int connect);

/**
 * @brief Sends the winner index to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param winner Index of the winner
 */
void send_winner_to(server *s, int connect, int winner);

/**
 * @brief Sends player scores to a client
 * @param s Pointer to the server
 * @param connect Client connection index
 * @param nb_players Number of players
 * @param scores Array of player scores
 */
void send_scores_to(server *s, int connect, int nb_players, int *scores);

/**
 * @brief Gets the directions from all clients
 * @param s Pointer to the server
 * @return Array of directions from all clients
 */
direction *get_directions_all(server *s);

/**
 * @brief Sends the game grid to all clients
 * @param s Pointer to the server
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 */
void send_grid_all(server *s, int nb_lignes, int nb_colonnes, int **grid);

/**
 * @brief Sends the number of players to all clients
 * @param s Pointer to the server
 * @param nb_player Number of players
 */
void send_nb_player_all(server *s, char nb_player);

/**
 * @brief Sends player positions to all clients
 * @param s Pointer to the server
 * @param nb_position Number of player positions
 * @param positions Array of player positions
 */
void send_positions_all(server *s, int nb_position, position **positions);

/**
 * @brief Sends the game over status to all clients
 * @param s Pointer to the server
 * @param est_fini Game over status
 */
void send_is_over_all(server *s, int est_fini);

/**
 * @brief Sends the start signal to all clients
 * @param s Pointer to the server
 */
void send_start_signal_all(server *s);

/**
 * @brief Sends player names to all clients
 * @param s Pointer to the server
 */
void send_names_all(server *s);

/**
 * @brief Sends the winner index to all clients
 * @param s Pointer to the server
 * @param winner Index of the winner
 */
void send_winner_all(server *s, int winner);

/**
 * @brief Sends player scores to all clients
 * @param s Pointer to the server
 * @param nb_players Number of players
 * @param scores Array of player scores
 */
void send_scores_all(server *s, int nb_players, int *scores);