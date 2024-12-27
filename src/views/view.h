#pragma once

/**
 * @file view.h
 * @date 2024-12-27
 * @brief Header file for the game views.
 * 
 * This file contains the declarations of structures and functions 
 * related to the game views, including ncurses and SDL implementations, 
 * for rendering the game state and handling user input.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include <ncurses.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"

#include "../model/model.h"
#include "../utils/utils.h"

/**
 * @struct view_ncurse
 * @brief Represents a view using the ncurses library
 */
typedef struct {
    WINDOW *grid_w;  /**< Window for the game grid */
    WINDOW *scores;  /**< Window for displaying scores */
    WINDOW *win;     /**< Window for displaying the game state */
} view_ncurse;

/**
 * @struct view_sdl
 * @brief Represents a view using the SDL library
 */
typedef struct {
    SDL_Window *window;          /**< SDL window */
    SDL_Renderer *renderer;      /**< SDL renderer */
    SDL_Rect *buttons[20];       /**< Array of buttons */
    unsigned nb_buttons;         /**< Number of buttons */
    unsigned menu_current;       /**< Current menu index */

    TTF_Font *font_title;        /**< Font for title */
    TTF_Font *font;              /**< General font */
    TTF_Font *font_score;        /**< Font for scores */
} view_sdl;

/**
 * @struct view
 * @brief General representation of a view, handling both SDL and ncurses special cases
 */
typedef struct v {
    char type;                   /**< Type of the view ('n' for ncurses, 's' for SDL) */
    view_ncurse *ncurse;         /**< Pointer to ncurses view structure */
    view_sdl *sdl;               /**< Pointer to SDL view structure */

    int nbMenu;                  /**< Number of menu options */

    unsigned width;              /**< Width of the view */
    unsigned height;             /**< Height of the view */

    /**
     * @brief Function pointer to update the screen
     * @param v Pointer to the view
     * @param nb_player Number of players
     * @param scores Array of player scores
     * @param grid Game grid
     * @param nb_lignes Number of lines in the grid
     * @param nb_colonnes Number of columns in the grid
     */
    void (*update_screen)(struct v *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);

    /**
     * @brief Function pointer to update the screen with changes
     * @param v Pointer to the view
     * @param nb_player Number of players
     * @param scores Array of player scores
     * @param grid Game grid
     * @param nb_lignes Number of lines in the grid
     * @param nb_colonnes Number of columns in the grid
     * @param players Array of player positions
     */
    void (*update_change_screen)(struct v *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players);

    /**
     * @brief Function pointer to get the direction input
     * @param v Pointer to the view
     * @param nb_player_on_keyboard Number of players using the keyboard
     * @param dirs Array of directions for each player
     * @return The direction input
     */
    direction (*get_direction)(struct v *v, int nb_player_on_keyboard, direction *dirs);

    /**
     * @brief Function pointer to destroy the view
     * @param v Pointer to the view
     */
    void (*destroy_self)(struct v *v);

    /**
     * @brief Function pointer to display the menu
     * @param v Pointer to the view
     * @param selected_option Pointer to the selected option
     * @param nbMenu Index of the selected menu
     */
    void (*affiche_menu)(struct v *v, int *selected_option, int nbMenu);

    /**
     * @brief Function pointer to get the action from the menu
     * @param v Pointer to the view
     * @param act Pointer to the actions structure
     * @param selected_option Pointer to the selected option
     * @param nbMenu Index of the selected menu
     */
    void (*get_action)(struct v *v, actions *act, int *selected_option, int nbMenu);

    /**
     * @brief Function pointer to display the winner screen
     * @param v Pointer to the view
     * @param indexPlayer Index of the winning player
     */
    void (*affiche_winner)(struct v *v, int indexPlayer);

    /**
     * @brief Function pointer to get the event input
     * @param v Pointer to the view
     * @param act Pointer to the actions structure
     */
    void (*get_event)(struct v *v, actions *act);
} view;

