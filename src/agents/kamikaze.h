#pragma once

/**
 * @file kamikaze.h
 * @date 2024-12-27
 * @brief Header file for Hara-Kiri bot implementation in the game.
 * 
 * This file contains the declarations of functions related to the Hara-Kiri bot 
 * implementation for the game. The Hara-Kiri bot will try to get as close as 
 * possible to the player.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include "../model/model.h"

/**
 * @brief Determines the direction to get as close as possible to the player
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param grid The game grid
 * @param players Array of player positions
 * @param directions Array of directions for each player
 * @param index_bot Bot index in players and directions
 * @return The direction for the bot to move
 */
direction kamikaze_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);