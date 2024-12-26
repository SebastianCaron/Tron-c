#pragma once

#include "view.h"
#include "../controller/controller.h"


view *init_view_ncurse();

void destroy_view_ncurses(view *v);

direction get_direction_ncurses(view *v, int nb_player_on_keyboard, direction *dirs);

void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes);
void update_change_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players);

void afficheMenuNC(view *v, int *, int nbMenu);
void affiche_win_ncurses(view *v, int indexPlayer);
void get_action_menu_ncurses(view *v, actions *act, int *selected_option, int nbMenu);

void get_event_ncurses(view *v, actions *act);