#pragma once

#include <ncurses.h>
#include "../model/model.h"

typedef struct{
    WINDOW *grid_w;
} view_ncurse;

typedef struct{

} view_sdl;

typedef struct v{
    char type;
    view_ncurse *ncurse;
    view_sdl *sdl;

    void (*update_screen)(int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);
    void (*update_change_screen)(int **grid, int nb_lignes, int nb_colonnes);

    direction (*get_direction)();
    void (*destroy_self)(struct v *);

} view;

