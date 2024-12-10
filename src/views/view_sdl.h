#pragma once

#include "view.h"
#include "../controller/controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HAUTEUR 600
#define LARGEUR 900

view *init_view_sdl();

void destroy_view_sdl(view *);

direction get_direction_sdl(view *v, direction *dir);

void update_screen_sdl(view *, int, int *, int **, int , int);

void quitter(SDL_Window *window, SDL_Renderer *renderer);
SDL_Rect *createRect(int h, int w, int x, int y);
SDL_Rect afficheTexte(SDL_Renderer *renderer,char *texte, int y, int titre);
void afficheMenuPrincipalSDL(view *v, actions *act);
void afficheMenuSoloSDL(view *v, actions *act);
void afficheMenuMultiplayerSDL(view *v, actions *act);

void get_action_menu_principal_sdl(view *v, actions *act, int *selected_option);
void get_action_menu_solo_sdl(view *v, actions *act, int *selected_option);
void get_action_menu_multi_sdl(view *v, actions *act, int *selected_option);