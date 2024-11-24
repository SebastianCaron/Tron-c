#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "view_ncurse.h"
#include "view.h"

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
    v->ncurse = vn;
    initscr();
    clear();
    timeout(-1);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    vn->grid_w = subwin(stdscr, LINES-2, COLS-40, 1, 20); 
    box(vn->grid_w, ACS_VLINE, ACS_HLINE);



    v->destroy_self = destroy_view_ncurse;
    v->get_direction = get_direction_ncurse;
    // v->update_change_screen = ;
    v->update_screen = update_screen_ncurse;
    refresh();
    // endwin();

    return v;
}



void destroy_view_ncurse(view *v){
    timeout(5000);
    endwin();
    clear();
    echo();
    free(v->ncurse);
    free(v);
}

direction get_direction_ncurse(){
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

void update_screen_ncurse(int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes){
    
}


// -lncurses