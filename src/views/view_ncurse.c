#include <stdio.h>
#include <stdio.h>

#include <view_ncurse.h>

view *init_view_ncurse(){
    view *v = (view *)malloc(sizeof(view));
    if(!v){
        perror("[VIEW NCURSE] erreur allocation de la structure view ncurse.");
        exit(EXIT_FAILURE);
    }
    // v->destroy_self = ;
    // v->get_direction = ;
    // v->update_change_screen = ;
    // v->update_screen = ;
}



void destroy_view_ncurse(view *v){
}