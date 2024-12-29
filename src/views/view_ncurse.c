#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>

#include "view_ncurse.h"
#include "view.h"
#include "../controller/controller.h"

int ncurses_tabColors[10][4] = { {0,0,0,255}, {255,255,0,255}, {255,0,255,255}, //Sol, J1, J2
                            {0,255,255,255}, {255,0,0,255}, {0,255,0,255}, //J3, J4, J5
                            {0,0,255,255}, {0,140,140,255}, {140,0,0,255}, //J6, J7, J8
                            {255,255,255,255}}; // Mur de la map

void init_colors() {
    start_color();

    for (int i = 0; i < 10; i++) {
        int r = ncurses_tabColors[i][0] * 1000 / 255;
        int g = ncurses_tabColors[i][1] * 1000 / 255;
        int b = ncurses_tabColors[i][2] * 1000 / 255;

        init_color(COLOR_WHITE + i + 1, r, g, b);
    }

    for (int i = 0; i < 10; i++) {
        init_pair(i + 1, COLOR_WHITE + i + 1, COLOR_BLACK);
    }
}


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
    getmaxyx(stdscr, v->height, v->width);
    v->width = (v->width > 100) ? 100 : v->width;
    v->width -= 40;
    v->height -= 2;
    // v->width = (v->width > 100) ? 100 : v->width-40;
    // v->height = (v->height > 100) ? 100 : v->height-2;
    v->ncurse = vn;

    clear();

    v->ncurse->grid_w = subwin(stdscr, v->height, v->width, 1, 25);
    v->ncurse->scores = subwin(stdscr, v->height, 20, 1, 0);


    // vn->grid_w = subwin(stdscr, LINES-2, COLS-40, 1, 20); 
    // box(vn->grid_w, ACS_VLINE, ACS_HLINE);

    v->destroy_self = destroy_view_ncurses;
    v->get_direction = get_direction_ncurses;
    v->update_change_screen = update_change_screen_ncurses;
    v->update_screen = update_screen_ncurses;
    v->get_event = get_event_ncurses;


    v->affiche_menu = affiche_menu_ncurses;
    v->affiche_winner = affiche_win_ncurses;

    v->nbMenu = 0;

    refresh();

    return v;
}

void destroy_view_ncurses(view *v){
    if(v == NULL) return;
    delwin(v->ncurse->grid_w);
    delwin(v->ncurse->scores);
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
            dirs[0] = RIGHT;
            return RIGHT;
        case KEY_LEFT:
            dirs[0] = LEFT;
            return LEFT;
        case KEY_UP:
            dirs[0] = UP;
            return UP;
        case KEY_DOWN:
            dirs[0] = DOWN;
            return DOWN;
        case 'z':
            dirs[nb_player_on_keyboard-1] = UP;
            return UP;
        case 'q':
            dirs[nb_player_on_keyboard-1] = LEFT;
            return LEFT;
        case 's':
            dirs[nb_player_on_keyboard-1] = DOWN;
            return DOWN;
        case 'd':
            dirs[nb_player_on_keyboard-1] = RIGHT;
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
            int color_pair = COLOR_WHITE;
            char *s = " ";
            if(grid[i][j] == WALL){
                s = "█";
            }else if(grid[i][j] < 0){
                color_pair = -(grid[i][j]) + 1;
                s = "▒";
            }else if(grid[i][j] > 0){
                color_pair = grid[i][j] + 1;
                s = "-";
            }


            wattron(v->ncurse->grid_w, COLOR_PAIR(color_pair));

            mvwprintw(v->ncurse->grid_w, i, j, "%s", s);

            wattroff(v->ncurse->grid_w, COLOR_PAIR(color_pair));
        }
    }
    wnoutrefresh(v->ncurse->grid_w);
    // wrefresh(v->ncurse->grid_w);

    box(v->ncurse->scores, ACS_VLINE, ACS_HLINE);
    for (int i = 0; i < nb_player; i++) {
        wattron(v->ncurse->scores, COLOR_PAIR(i + 2));
        mvwprintw(v->ncurse->scores, i + 1, 1, "Player %d: %d", i + 1, scores[i]);
        wattroff(v->ncurse->scores, COLOR_PAIR(i + 2));
    }
    // wrefresh(v->ncurse->scores);
    wnoutrefresh(v->ncurse->scores);

    doupdate(); 
}

void update_change_screen_ncurses(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players){
    //box(v->ncurse->grid_w, ACS_VLINE, ACS_HLINE);
    int to_check[5][2];
    for(int i = 0; i < nb_player; i++){
        position *p = players[i];
        to_check[0][0] = p->x;
        to_check[0][1] = p->y;
        to_check[1][0] = p->x + 1;
        to_check[2][0] = p->x - 1;
        to_check[1][1] = p->y;
        to_check[2][1] = p->y;
        to_check[3][0] = p->x;
        to_check[4][0] = p->x;
        to_check[3][1] = p->y + 1;
        to_check[4][1] = p->y - 1;        
        for(int k = 0; k < 5; k++){
            int color_pair = COLOR_WHITE;
            char *s = " ";
            if(to_check[k][1] < 0 || to_check[k][1] >= nb_lignes || to_check[k][0] < 0 || to_check[k][0] >= nb_colonnes) continue;
            if(grid[to_check[k][1]][to_check[k][0]] == WALL){
                s = "█";
            }else if(grid[to_check[k][1]][to_check[k][0]] < 0){
                color_pair = -(grid[to_check[k][1]][to_check[k][0]]) + 1;
                s = "▒";
            }else if(grid[to_check[k][1]][to_check[k][0]] > 0){
                color_pair = grid[to_check[k][1]][to_check[k][0]] + 1;
                s = "-";
            }

            wattron(v->ncurse->grid_w, COLOR_PAIR(color_pair));

            mvwprintw(v->ncurse->grid_w, to_check[k][1], to_check[k][0], "%s", s);

            wattroff(v->ncurse->grid_w, COLOR_PAIR(color_pair));
        }
    }

    wnoutrefresh(v->ncurse->grid_w);

    werase(v->ncurse->scores);
    box(v->ncurse->scores, ACS_VLINE, ACS_HLINE);
    for (int i = 0; i < nb_player; i++) {
        wattron(v->ncurse->scores, COLOR_PAIR(i + 2));
        mvwprintw(v->ncurse->scores, i + 1, 1, "Player %d: %d", i + 1, scores[i]);
        wattroff(v->ncurse->scores, COLOR_PAIR(i + 2));
    }
    wnoutrefresh(v->ncurse->scores);

    doupdate(); 
}

void affiche_win_ncurses(view *v, int indexPlayer) {
    int min_win_width = 40;
    int min_win_height = 7;
    char msg[50];
    if(indexPlayer == -1){
        snprintf(msg, sizeof(msg), "Aucun Vainqueur");
    }else{
        snprintf(msg, sizeof(msg), "Le joueur %d a gagné !", indexPlayer+1);
    }

    if (v->width < min_win_width || v->height < min_win_height) {
        clear();
        mvprintw(0, 0, "Fenêtre trop petite pour afficher proprement le vainqueur !");
        mvprintw(2, 0, "%s", msg);
        refresh();
        return;
    }

    int win_height = 10;
    int win_width = v->width;

    int start_y = (v->height - win_height) / 2;
    int start_x = (v->width - win_width) / 2 + 25;

    v->ncurse->win = subwin(stdscr, win_height, win_width, start_y, start_x);
    werase(v->ncurse->win);
    wrefresh(v->ncurse->win);
    box(v->ncurse->win, ACS_VLINE, ACS_HLINE);
    

    int msg_x = (win_width - strlen(msg)) / 2;
    mvwprintw(v->ncurse->win, 3, msg_x, "%s", msg);

    const char *ok_text = "< OK >";
    int ok_x = (win_width - strlen(ok_text)) / 2;
    mvwprintw(v->ncurse->win, 5, ok_x, "%s", ok_text);

    wrefresh(v->ncurse->win);
    delwin(v->ncurse->win);
}


void affiche_menu_ncurses(view *v, int *selected_option, int nbMenu){
    v->get_action = get_action_menu_ncurses;

    #ifdef BOT_TRAINING
    const char *options[3][6] = {
        {"TRON", "solo", "multiplayer", "EXIT", NULL, NULL},       
        {"SOLO", "vs Q_LEARNING !!", "vs Algo (kami cases)", "vs Algo (m'sieur Frodon \u261D \U0001F913)", "vs Algo (mon espace)", "BACK"},     
        {"MULTIPLAYER", "on this machine (2 players)", "with others", "BACK", NULL, NULL}      
    };
    #else
    const char *options[3][6] = {
        {"TRON", "solo", "multiplayer", "EXIT", NULL, NULL},       
        {"SOLO", "vs Algo (alphonse)", "vs Algo (kami cases)", "vs Algo (m'sieur Frodon \u261D \U0001F913)", "vs Algo (mon espace)", "BACK"},     
        {"MULTIPLAYER", "on this machine (2 players)", "with others", "BACK", NULL, NULL}      
    };
    #endif

    int n_options = 4;
    if(nbMenu == 1) n_options = 6;

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

    #ifdef BOT_TRAINING
    const actions menuActions[4][5] = {
        {MENU_SOLO, MENU_MULTI, QUITTER},       
        {PLAY_BOT_Q, PLAY_BOT_KAMIKAZE, PLAY_BOT_COPY, PLAY_BOT_ESPACE, RETOUR},    
        {PLAY_MULTI, PLAY_ONLINE, RETOUR},
        {RETOUR}       
    };
    #else
    const actions menuActions[4][5] = {
        {MENU_SOLO, MENU_MULTI, QUITTER},       
        {PLAY_BOT_ALGO, PLAY_BOT_KAMIKAZE, PLAY_BOT_COPY, PLAY_BOT_ESPACE, RETOUR},    
        {PLAY_MULTI, PLAY_ONLINE, RETOUR},
        {RETOUR}       
    };
    #endif

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

void get_event_ncurses(view *v, actions *act){
    getch();
}


// -lncurses