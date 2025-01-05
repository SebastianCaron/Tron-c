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

/**
 * @brief Test if DATA is available
 * @param c Pointer to the client
 * @param DATA Type of Data
 * @return 1 if available, 0 otherwise
 */
int is_in_data_available(client *c, int DATA);


/**
 * @brief Retrieve the available data in the fd and set it into the structure
 * @param c Pointer to the client
 */
void retrieve_data_client(client *c);