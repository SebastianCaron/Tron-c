#pragma once

#include "view.h"
#include "../controller/controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HAUTEUR 600
#define LARGEUR 800

#define HPIXEL HAUTEUR/20
#define WPIXEL LARGEUR/44

view *init_view_sdl();

void destroy_view_sdl(view *);

direction get_direction_sdl();

void update_screen_sdl(view *, int, int *, int **, int , int);

void quitter(SDL_Window *window, SDL_Renderer *renderer);
SDL_Rect *createRect(int h, int w, int x, int y);
void afficheTexte(SDL_Renderer *renderer,char *texte, int x, int y);
void afficheMenuPrincipalSDL(view *v, actions *act);
void afficheMenuSoloSDL(view *v, actions *act);
void afficheMenuMultiplayerSDL(view *v, actions *act);