#include <stdio.h>
#include <stdlib.h>

#include "view_sdl.h"

view *init_view_sdl(){
    view *v = (view *)malloc(sizeof(view));
    if(!v){
        perror("[VIEW SDL] erreur allocation de la structure view sdl.");
        exit(EXIT_FAILURE);
    }
    // v->destroy_self = ;
    // v->get_direction = ;
    // v->update_change_screen = ;
    // v->update_screen = ;
}



void destroy_view_sdl(view *v){
}