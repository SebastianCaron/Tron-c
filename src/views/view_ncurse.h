#pragma once

#include "view.h"
#include "../controller/controller.h"


view *init_view_ncurse();

void destroy_view_ncurses(view *v);

direction get_direction_ncurses(view *v);

void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);
void afficheMenuPrincipalNC(view *v, actions *act);
void afficheMenuSoloNC(view *v, actions *act);
void afficheMenuMultiplayerNC(view *v, actions *act);