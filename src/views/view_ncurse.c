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

    initscr();
    v->type = 'n';
    getmaxyx(stdscr, v->height, v->width);
    v->width -= 40;
    v->height -= 2;

    v->ncurse = vn;

    clear();
    timeout(-1);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    v->ncurse->grid_w = subwin(stdscr, v->height, v->width, 1, 20);

    // vn->grid_w = subwin(stdscr, LINES-2, COLS-40, 1, 20); 
    // box(vn->grid_w, ACS_VLINE, ACS_HLINE);



    v->destroy_self = destroy_view_ncurses;
    v->get_direction = get_direction_ncurses;
    // v->update_change_screen = ;
    v->update_screen = update_screen_ncurses;


    v->affiche_menu = afficheMenuNC;
    // v->affiche_menu_principal = afficheMenuPrincipalNC;
    // v->affiche_menu_multijoueur = afficheMenuMultiplayerNC;
    // v->affiche_menu_solo = afficheMenuSoloNC;

    v->nbMenu = 0;

    refresh();

    return v;
}

void destroy_view_ncurses(view *v){
    if(v == NULL) return;
    getch();
    free(v->ncurse->grid_w);
    free(v->ncurse);
    free(v);
    clear();
    echo();
    endwin();
    

}

direction get_direction_ncurses(view *v, int nb_player_on_keyboard, direction *dirs){
    int ch = getch();
    switch (ch) {
        case KEY_RIGHT:
            if((dirs[0]) != LEFT) (dirs[0]) = RIGHT;
            return RIGHT;
        case KEY_LEFT:
            if((dirs[0]) != RIGHT) (dirs[0]) = LEFT;
            return LEFT;
        case KEY_UP:
            if((dirs[0]) != DOWN) (dirs[0]) = UP;
            return UP;
        case KEY_DOWN:
            if((dirs[0]) != UP) (dirs[0]) = DOWN;
            return DOWN;
        case 'z':
            if((dirs[nb_player_on_keyboard-1]) != DOWN) (dirs[nb_player_on_keyboard-1]) = UP;
            return UP;
        case 'q':
            if((dirs[nb_player_on_keyboard-1]) != RIGHT) (dirs[nb_player_on_keyboard-1]) = LEFT;
            return LEFT;
        case 's':
            if((dirs[nb_player_on_keyboard-1]) != UP) (dirs[nb_player_on_keyboard-1]) = DOWN;
            return DOWN;
        case 'd':
            if((dirs[nb_player_on_keyboard-1]) != LEFT) (dirs[nb_player_on_keyboard-1]) = RIGHT;
            return RIGHT;
        default:
            // printw("%c (code : %d)\n", ch, ch);
            break;
    }
    return NODIRECTION;
}

void update_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes){
    clear();
    box(v->ncurse->grid_w, ACS_VLINE, ACS_HLINE);
    // fprintf(stderr, "C : %d, L : %d\n", nb_colonnes, nb_lignes);
    for(int i = 0; i < nb_lignes; i++){
        for(int j = 0; j < nb_colonnes; j++){
            char s = ' ';
            if(grid[i][j] == WALL){
                s = '#';
            }else if(grid[i][j] < 0){
                s = '#';
            }else if(grid[i][j] > 0){
                s = '-';
            }
            mvwprintw(v->ncurse->grid_w, i, j, "%c", s);
        }
    }
    wrefresh(v->ncurse->grid_w);
}

void afficheMenuNC(view *v, int *selected_option, int nbMenu){
    v->get_action = get_action_menu_ncurses;

    const char *options[3][4] = {
        {"TRON", "solo", "multiplayer", "EXIT"},       
        {"SOLO", "vs Algo", "vs Q-Learning", "BACK"},     
        {"MULTIPLAYER", "on this machine (2 players)", "with others", "BACK"}      
    };

    int n_options = 4;

    if(*selected_option < 1) *selected_option = n_options-1;
    if(*selected_option >= n_options) *selected_option = 1;

    int choix = *selected_option;

    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);

    int titre_x = (largeur - strlen(options[nbMenu][0])) / 2;

    int options_y = hauteur / 2;
    int options_x = (largeur - 10) / 2;

    clear();
    mvprintw(hauteur / 4, titre_x, "%s", options[nbMenu][0]);

    for (int i = 1; i < n_options; i++) {
        if (i == choix) {
            mvprintw(options_y + i, options_x, "< %s >", options[nbMenu][i]);
        } else {
            mvprintw(options_y + i, options_x, "  %s  ", options[nbMenu][i]);
        }
    }

    refresh();
}


void get_action_menu_ncurses(view *v, actions *act, int *selected_option, int nbMenu){
    refresh();
    int ch = getch();

    const actions menuActions[3][3] = {
        {MENU_SOLO, MENU_MULTI, QUITTER},       
        {PLAY_BOT_ALGO, PLAY_BOT_Q, RETOUR},    
        {PLAY_MULTI, PLAY_ONLINE, RETOUR}       
    };

    switch (ch) {
        case KEY_UP:
            (*selected_option) = (*selected_option) - 1;
            break;
        case KEY_DOWN:
            (*selected_option) = (*selected_option) + 1;
            break;
        case '\n':
            (*act) = menuActions[nbMenu][*selected_option-1];
            return;
    }
}


// -lncurses