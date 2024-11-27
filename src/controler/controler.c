#include <stdio.h>
#include <stdlib.h>


#include "../model/model.h"
#include "../views/view_sdl.h"
#include "../views/view_ncurse.h"
#include "../views/view.h"


void controllerPlay(model *m, view *v){
    while(!est_fini(m)){
        direction dir = v->get_direction();
        move_player(m, m->n_player, dir);

        v->update_change_screen(m->grid, m->nb_lignes_grid, m->nb_colonnes_grid);
    }
}

void init_controller(view *v){
    // Demander tous les paramètres à mettre dans init_game
    model *game = init_game();

    controllerPlay(game, v);
}

void controller_destroy(model *m, view *v) {
    v->destroy_self(v); 
    destroy(m);
}