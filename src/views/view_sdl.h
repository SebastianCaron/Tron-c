#pragma once

#include "view.h"
#include <SDL2/SDL.h>

#define HAUTEUR 1400
#define LARGEUR 1200

view *init_view_sdl();

void destroy_view_sdl(view *, SDL_Renderer *, SDL_Window *);

direction get_direction_ncurse();

void update_screen_sdl(int, int *, int **, int , int , SDL_Renderer *);