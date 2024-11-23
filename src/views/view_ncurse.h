#pragma once

#include "view.h"

view *init_view_ncurse();

void destroy_view_ncurse(view *);

direction get_direction_ncurse();

void update_screen_ncurse(int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);