#pragma once

#include <ncurses.h>
#include <SDL2/SDL.h>

#include "../model/model.h"
#include "../utils/utils.h"


typedef struct{
    WINDOW *grid_w;
} view_ncurse;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect *buttons[20];
    unsigned nb_buttons;
    unsigned is_init_menu[3];
} view_sdl;

typedef struct v{
    char type;
    view_ncurse *ncurse;
    view_sdl *sdl;

    int nbMenu;
    

    unsigned width;
    unsigned height;

    void (*update_screen)(struct v *, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);
    void (*update_change_screen)(struct v *, int **grid, int nb_lignes, int nb_colonnes);

    direction (*get_direction)(struct v *, int nb_player_on_keyboard, direction *dirs);
    void (*destroy_self)(struct v *);

    void (*affiche_menu)(struct v *, int *, int );
    void (*get_action)(struct v *, actions *, int *, int);
} view;


