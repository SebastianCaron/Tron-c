#pragma once

/**
 * @file rectiligne.h
 * @date 2024-12-27
 * @brief Header file for bot implementation in the game.
 * 
 * This file contains the declarations of functions related to the bot 
 * implementation for the game, including functions to determine the 
 * direction of movement for the bot.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include "../model/model.h"

/**
 * @brief Determines the direction for the bot (no direction if no wall in front of it).
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Bot index in players and directions
 * @return The direction for the bot to move
 */
direction rectiligne_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);

/**
 * @brief Determines the direction for the bot
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Bot index in players and directions
 * @return The direction for the bot to move
 */
direction random_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);