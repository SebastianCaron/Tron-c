#pragma once

#include "../model/model.h"

/**
 * @file big.h
 * @date 2024-12-27
 * @brief Header file for various bot strategies in the game.
 * 
 * This file contains the declarations of functions related to different bot 
 * strategies for the game. Each function determines the direction of movement 
 * for a bot based on a specific strategy.
 * 
 * Authors:
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

/**
 * @brief Determines the direction for the "espace vital" bot.
 * 
 * The "espace vital" bot tries to maximize its available space and move
 * towards the center in prirority
 * 
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Index of the bot in the players array
 * @return The direction for the bot to move
 */
direction espace_vital_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);

/**
 * @brief Determines the direction for the "immitateur" bot.
 * 
 * The "immitateur" bot tries to maximize its available space and move
 * towards a player that is alive. When playing against it, it feels like it mimics us.
 * 
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Index of the bot in the players array
 * @return The direction for the bot to move
 */
direction immitateur_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);