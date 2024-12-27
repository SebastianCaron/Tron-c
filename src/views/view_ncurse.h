#pragma once

/**
 * @file view_ncurses.h
 * @date 2024-12-27
 * @brief Header file for ncurses-based view functions.
 * 
 * This file contains the declarations of structures and functions 
 * related to the ncurses-based view, including initialization, 
 * screen updates, menu handling, and user input processing.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include "view.h"
#include "../controller/controller.h"

/**
 * @brief Initializes a ncurses-based view
 * @return A pointer to the initialized view
 */
view *init_view_ncurse();

/**
 * @brief Initialize colors for ncurses
 */
void init_colors();

/**
 * @brief Frees the memory allocated for the ncurses view
 * @param v Pointer to the view to be destroyed
 */
void destroy_view_ncurses(view *v);

/**
 * @brief Gets the direction input from the keyboard for ncurses view
 * @param v Pointer to the view
 * @param nb_player_on_keyboard Number of players using the keyboard
 * @param dirs Array of directions for each player
 * @return The direction input
 */
direction get_direction_ncurses(view *v, int nb_player_on_keyboard, direction *dirs);

/**
 * @brief Updates the screen with the current game state for ncurses view
 * @param v Pointer to the view
 * @param nb_player Number of players
 * @param scores Array of player scores
 * @param grid The game grid
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 */
void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);

/**
 * @brief Updates only the changes in the game for ncurses view
 * @param v Pointer to the view
 * @param nb_player Number of players
 * @param scores Array of player scores
 * @param grid The game grid
 * @param nb_lignes Number of lines in the grid
 * @param nb_colonnes Number of columns in the grid
 * @param players Array of player positions
 */
void update_change_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players);

/**
 * @brief Displays the menu in ncurses view
 * @param v Pointer to the view
 * @param selected_option Pointer to the selected option
 * @param nbMenu Number of the selected menu
 */
void affiche_menu_ncurses(view *v, int *selected_option, int nbMenu);

/**
 * @brief Displays the winner screen in ncurses view
 * @param v Pointer to the view
 * @param indexPlayer Index of the winning player
 */
void affiche_win_ncurses(view *v, int indexPlayer);

/**
 * @brief Gets the action from the menu in ncurses view
 * @param v Pointer to the view
 * @param act Pointer to the actions structure
 * @param selected_option Pointer to the selected option
 * @param nbMenu Number of the selected menu
 */
void get_action_menu_ncurses(view *v, actions *act, int *selected_option, int nbMenu);

/**
 * @brief Gets the event input in ncurses view
 * @param v Pointer to the view
 * @param act Pointer to the actions structure
 */
void get_event_ncurses(view *v, actions *act);