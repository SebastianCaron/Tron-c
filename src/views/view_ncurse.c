#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "view_ncurse.h"
#include "view.h"
#include "../controller/controller.h"



view *init_view_ncurse(){
    view *v = (view *)malloc(sizeof(view));
    if(!v){
        perror("[VIEW NCURSE] erreur allocation de la structure view.");
        exit(EXIT_FAILURE);
    }
    view_ncurse *vn = (view_ncurse *) calloc(1, sizeof(view_ncurse));
    if(!vn){
        perror("[VIEW NCURSE] erreur allocation de la structure view ncurse.");
        exit(EXIT_FAILURE);
    }


    v->type = 'n';
    v->width = COLS;
    v->height = LINES;

    v->ncurse = vn;
    initscr();
    clear();
    timeout(-1);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // vn->grid_w = subwin(stdscr, LINES-2, COLS-40, 1, 20); 
    // box(vn->grid_w, ACS_VLINE, ACS_HLINE);



    v->destroy_self = destroy_view_ncurses;
    v->get_direction = get_direction_ncurses;
    // v->update_change_screen = ;
    v->update_screen = update_screen_ncurses;
    v->affiche_menu_principal = afficheMenuPrincipalNC;
    v->affiche_menu_multijoueur = afficheMenuMultiplayerNC;
    v->affiche_menu_solo = afficheMenuSoloNC;

    refresh();

    return v;
}

void destroy_view_ncurses(view *v){
    if(v == NULL) return;
    timeout(5000);
    getch();
    endwin();
    clear();
    echo();
    free(v->ncurse);
    free(v);
}

direction get_direction_ncurses(view *v){
    int ch = getch();
    switch (ch) {
        case KEY_RIGHT:
            return RIGHT;
        case KEY_LEFT:
            return LEFT;
        case KEY_UP:
            return UP;
        case KEY_DOWN:
            return DOWN;
        default:
            // printw("%c (code : %d)\n", ch, ch);
            break;
    }
    return NODIRECTION;
}

void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes){

}

void afficheMenuPrincipalNC(view *v, int *selected_option){
    v->get_action = get_action_menu_principal_ncurses;

    const char *options[] = {"SOLO", "MULTIPLAYER"};
    int n_options = 2;
    if(*selected_option < 0) *selected_option = n_options-1;
    if(*selected_option >= n_options) *selected_option = 0;

    int choix = *selected_option;

    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);

    const char *titre = "TRON";
    int titre_x = (largeur - strlen(titre)) / 2;

    int options_y = hauteur / 2;
    int options_x = (largeur - 10) / 2;

    clear();
    mvprintw(hauteur / 4, titre_x, "%s", titre);

    for (int i = 0; i < n_options; i++) {
        if (i == choix) {
            mvprintw(options_y + i, options_x, "< %s >", options[i]);
        } else {
            mvprintw(options_y + i, options_x, "  %s  ", options[i]);
        }
    }

    refresh();
}

void get_action_menu_principal_ncurses(view *v, actions *act, int *selected_option){
    refresh();
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            (*selected_option) = (*selected_option) - 1;
            break;
        case KEY_DOWN:
            (*selected_option) = (*selected_option) + 1;
            break;
        case '\n':
            if((*selected_option) == 0){
                (*act) = MENU_SOLO;
                return;
            }
            if((*selected_option) == 1){
                (*act) = MENU_MULTI;
                return;
            }
            return;
        // default:
        //     (*act) = NO_ACTION;
        //     return;
    }
}

void afficheMenuSoloNC(view *v, int *selected_option){
    v->get_action = get_action_menu_solo_ncurses;

    const char *options[] = {"VS ALGO", "VS Q-LEARNING"};
    int n_options = 2;
    if(*selected_option < 0) *selected_option = n_options-1;
    if(*selected_option >= n_options) *selected_option = 0;

    int choix = *selected_option;

    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);

    const char *titre = "SOLO";
    int titre_x = (largeur - strlen(titre)) / 2;

    int options_y = hauteur / 2;
    int options_x = (largeur - 10) / 2;

    clear();
    mvprintw(hauteur / 4, titre_x, "%s", titre);

    for (int i = 0; i < n_options; i++) {
        if (i == choix) {
            mvprintw(options_y + i, options_x, "< %s >", options[i]);
        } else {
            mvprintw(options_y + i, options_x, "  %s  ", options[i]);
        }
    }

    refresh();
}

void get_action_menu_solo_ncurses(view *v, actions *act, int *selected_option){
    refresh();
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            (*selected_option) = (*selected_option) - 1;
            break;
        case KEY_DOWN:
            (*selected_option) = (*selected_option) + 1;
            break;
        case '\n':
            if((*selected_option) == 0){
                (*act) = PLAY_BOT_ALGO;
                return;
            }
            if((*selected_option) == 1){
                (*act) = PLAY_BOT_Q;
                return;
            }
            return;
        // default:
        //     (*act) = NO_ACTION;
        //     return;
    }
}

void afficheMenuMultiplayerNC(view *v, int *selected_option){
    v->get_action = get_action_menu_multi_ncurses;

    const char *options[] = {"ON THIS MACHINE", "ONLINE"};
    int n_options = 2;
    if(*selected_option < 0) *selected_option = n_options-1;
    if(*selected_option >= n_options) *selected_option = 0;

    int choix = *selected_option;

    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);

    const char *titre = "MULTIPLAYER";
    int titre_x = (largeur - strlen(titre)) / 2;

    int options_y = hauteur / 2;
    int options_x = (largeur - 10) / 2;

    clear();
    mvprintw(hauteur / 4, titre_x, "%s", titre);

    for (int i = 0; i < n_options; i++) {
        if (i == choix) {
            mvprintw(options_y + i, options_x, "< %s >", options[i]);
        } else {
            mvprintw(options_y + i, options_x, "  %s  ", options[i]);
        }
    }

    refresh();
}

void get_action_menu_multi_ncurses(view *v, actions *act, int *selected_option){
    refresh();
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            (*selected_option) = (*selected_option) - 1;
            break;
        case KEY_DOWN:
            (*selected_option) = (*selected_option) + 1;
            break;
        case '\n':
            if((*selected_option) == 0){
                (*act) = PLAY_MULTI;
                return;
            }
            if((*selected_option) == 1){
                (*act) = PLAY_ONLINE;
                return;
            }
            return;
        // default:
        //     (*act) = NO_ACTION;
        //     return;
    }
}


// -lncurses