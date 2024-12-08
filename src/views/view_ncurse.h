#pragma once

#include "view.h"
#include "../controller/controller.h"


view *init_view_ncurse();

void destroy_view_ncurses(view *v);

direction get_direction_ncurses(view *v);

void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);
void afficheMenuPrincipalNC(view *v, int *);
void afficheMenuSoloNC(view *v, int *);
void afficheMenuMultiplayerNC(view *v, int *);

void get_action_menu_principal_ncurses(view *v, actions *act, int *selected_option);
void get_action_menu_solo_ncurses(view *v, actions *act, int *selected_option);
void get_action_menu_multi_ncurses(view *v, actions *act, int *selected_option);