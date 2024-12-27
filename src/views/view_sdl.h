#pragma once

/**
 * @file view_sdl.h
 * @date 2024-12-27
 * @brief Header file for SDL-based view functions.
 * 
 * This file contains the declarations of structures and functions 
 * related to the SDL-based view, including initialization, screen 
 * updates, menu handling, and user input processing.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "view.h"
#include "../controller/controller.h"


#define HAUTEUR 600
#define LARGEUR 900


/**
 * @brief Initialize the sdl view
 * @return the view
 */
view *init_view_sdl();

/**
 * @brief Initialize the sdl view
 * @param v the view we want free
 */
void destroy_view_sdl(view *v);

/**
 * @brief get the direction of player in the game depending on the key they push
 * @param v the view where the direction is retrieve
 * @param nb_player_on_keyboard the number of players to know for which player the direction corresponds
 * @return the direction retrieved
 */
direction get_direction_sdl(view *v, int nb_player_on_keyboard, direction *dirs);

/**
 * @brief update the display of sdl view
 * @param v the view that needs to be update
 * @param nb_player the number of players to display their scores
 * @param score the table of scores / each score corresponds to the player's index
 * @param grid table of the game
 * @param nb_lignes to know how many items to display with nb_colonnes
 * @param nb_colonnes to know how many items to display with nb_lignes
 */
void update_screen_sdl(view *v, int nb_players, int *scores, int **grid, int nb_lignes, int nb_colonnes);

/**
 * @brief leave the sdl window
 * @param window must exist to exit ttf and sdl
 * @param renderer must exist to exit ttf and sdl
 */
void quitter(SDL_Window *window, SDL_Renderer *renderer);

/**
 * @brief create a new rectangle item
 * @param h heigth
 * @param w width
 * @param x coordinate 
 * @param y coordinate
 * @return the rectangle created
 */
SDL_Rect *createRect(int h, int w, int x, int y);

/**
 * @brief create the menu buttons 
 * @param renderer to draw the button on
 * @param texte to display on the button
 * @param y coodinate to place the button
 * @param titre to know if the text surface should be filled or not
 * @param f the font used
 * @return the rectangle created for the button to know his coordinate
 */
SDL_Rect *afficheButton(SDL_Renderer *renderer,char *texte, int y, int titre, TTF_Font *f);

/**
 * @brief display the scores
 * @param renderer to display the scores on
 * @param nb_player the number of players to display their scores
 * @param scores the table of scores / each score corresponds to the player's index
 * @param f the font used
 */
void afficheScore(SDL_Renderer *renderer, int nb_player, int *scores,TTF_Font *f);

/**
 * @brief display the winner at the end of the game
 * @param v to retrieve informations of the view
 * @param indexPlayer the index of the winner
 */
void affiche_win_sdl(view *v, int indexPlayer);

/**
 * @brief display the correct menu
 * @param v to retrieve informations of the view
 * @param act only use for ncurse
 * @param nbMenu the index of the current menu
 */
void affiche_menu_sdl(view *v, int *act, int nbMenu);

/**
 * @brief free memory used by buttons
 * @param sdl to retrieve buttons of the view
 */
void free_buttons(view_sdl *sdl);

/**
 * @brief update act based on the button clicked
 * @param v to retrieve informations of the view buttons
 * @param act the pointer of the element to update to do what is marked on the button
 * @param selected_option only use for ncurse for the display
 * @param nbMenu the index of the current menu
 */
void get_action_menu_sdl(view *v, actions *act, int *selected_option, int nbMenu);

/**
 * @brief listen event to know if the view wants to be exited
 * @param v the view
 * @param act the pointer of the element to update
 */
void get_event_sdl(view *v, actions *act);


void update_change_screen_sdl(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players);