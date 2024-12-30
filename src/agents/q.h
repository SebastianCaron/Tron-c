#pragma once
/**
 * @file q.h
 * @date 2024-12-28
 * @brief Header file for Q-Learning bot implementation in the game.
 * 
 * This file contains the declarations of functions related to the Q-Learning bot 
 * implementation for the game. This doesn't work really well...
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor 
 */

#include "../model/model.h"

// Define the penalty for collision
#define PENALTY_COLLISION -100

// Define the Q-learning algorithm parameters
#define ALPHA 0.1 /**< Learning rate */
#define GAMMA 0.9 /**< Discount factor */
#define EPSILON 0.1 /**< Exploration rate */
#define MAX_STATES 10000000 /**< Maximum number of states */

/**
 * @brief Initializes the Q-table.
 * 
 * This function initializes the Q-table, which is used to store the expected 
 * rewards for each state-action pair.
 */
void init_Q();

/**
 * @brief Saves the Q-table to a file.
 * 
 * This function saves the current state of the Q-table to a file specified by 
 * the filename parameter.
 * 
 * @param filename Path to the file where the Q-table will be saved
 */
void save_Q_table(const char *filename);

/**
 * @brief Loads the Q-table from a file.
 * 
 * This function loads the Q-table from a file specified by the filename parameter.
 * 
 * @param filename Path to the file from which the Q-table will be loaded
 * @return 1 if the Q-table was successfully loaded, 0 otherwise
 */
int load_Q_table(const char *filename);

/**
 * @brief Determines the direction for the Q-learning bot.
 * 
 * This function uses the Q-learning algorithm to determine the optimal direction 
 * for the bot to move based on the current state of the game.
 * 
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Index of the bot in the players array
 * @return The direction for the bot to move
 */
direction q_learning_bot(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);

/**
 * @brief Reset the number of episode and the epsilon value if has changed
 */
void reset_bot_q();