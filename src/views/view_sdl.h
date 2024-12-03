#pragma once

#include "view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HAUTEUR 600
#define LARGEUR 800

#define HPIXEL HAUTEUR/20
#define WPIXEL LARGEUR/44

view *init_view_sdl();

void destroy_view_sdl(view *, SDL_Renderer *, SDL_Window *);

direction get_direction_ncurse();

void update_screen_sdl(int, int *, int **, int , int , SDL_Renderer *);