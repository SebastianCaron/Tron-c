#pragma once

#include "view.h"
#include "../controller/controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

void update_screen_sdl(view *, int, int *, int **, int , int);

void quitter(SDL_Window *window, SDL_Renderer *renderer);
SDL_Rect *createRect(int h, int w, int x, int y);
SDL_Rect *afficheButton(SDL_Renderer *renderer,char *texte, int y, int titre, TTF_Font *);
void afficheScore(SDL_Renderer *, int, int *, TTF_Font *);


void affiche_win_sdl(view *v, int indexPlayer);

void affiche_menu_sdl(view *v, int *act, int nbMenu);
void free_buttons(view_sdl *sdl);

void get_action_menu_sdl(view *v, actions *act, int *selected_option, int nbMenu);
void get_event_sdl(view *v, actions *act);