#pragma once

/**
 * @file client.h
 * @date 2024-12-27
 * @brief Header file for the game client.
 * 
 * This file contains the declarations of structures and functions 
 * related to the game client, which connects to the server, manages 
 * local game state, and handles communication with the server.
 * 
 * @author Sebastian Caron
 */

#include "network.h"
#include "../utils/utils.h"
#include "../utils/QStack.h"
#include "../model/model.h"

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

/**
 * @brief Initializes a client with the specified IP address and port
 * @param ip The IP address of the server
 * @param port The port number for the server
 * @return A pointer to the initialized client
 */
client *init_client(char *ip, int port);

/**
 * @brief Frees the memory allocated for the client
 * @param c Pointer to the client to be destroyed
 */
void destroy_client(client *c);

/**
 * @brief Gets the game grid from the server
 * @param c Pointer to the client
 * @return A pointer to the game grid
 */
grid *client_get_grid(client *c);

/**
 * @brief Checks if the game is over
 * @param c Pointer to the client
 * @return Game over status
 */
int client_is_over(client *c);

/**
 * @brief Gets the scores from the server
 * @param c Pointer to the client
 * @return Array of scores
 */
int *client_get_scores(client *c);

/**
 * @brief Gets the positions of players from the server
 * @param c Pointer to the client
 * @return Array of player positions
 */
position *client_get_positions(client *c);

/**
 * @brief Gets the number of players from the serveur
 * @param c Pointer to the client
 * @return Number of players
 */
int client_get_nb_player(client *c);

/**
 * @brief Checks if the game has started
 * @param c Pointer to the client
 * @return Game start status
 */
int client_get_start_signal(client *c);

/**
 * @brief Gets the names of players from the server
 * @param c Pointer to the client
 * @return Array of player names
 */
char **client_get_names(client *c);

/**
 * @brief Gets the winner index from the server
 * @param c Pointer to the client
 * @return Index of the winner
 */
int client_get_winner(client *c);

/**
 * @brief Sends a movement direction to the server
 * @param c Pointer to the client
 * @param d Direction to be sent
 */
void client_send_movement(client *c, direction d);

/**
 * @brief Asks the server for the game grid
 * @param c Pointer to the client
 */
void client_ask_for_grid(client *c);
