#include <stdio.h>
#include <stdlib.h>
#include "../model/model.h"
#include "../views/view_sdl.h"
#include "../views/view_ncurse.h"
#include "../views/view.h"
#include "./controller.h"


void controllerPlay(model *m, view *v){
    while(!est_fini(m)){
        direction dir = v->get_direction(v);
        move_player(m, m->n_player, dir);

        v->update_change_screen(v, m->grid, m->nb_lignes_grid, m->nb_colonnes_grid);
    }
}

controller *init_controller(view *sdl, view *ncurses){
    // Demander tous les paramètres à mettre dans init_game
    // model *game = init_game();
    //controllerPlay(game, v);

    controller *c = calloc(1, sizeof(controller));
    if(c == NULL){
        perror("[CONTROLLER] erreur allocation de la structure controller.");
        exit(EXIT_FAILURE);
    }

    c->sdl = sdl;
    c->ncurses = ncurses;
}

void create_model(controller *c){
    // RECUPERER TAILLE NCURSES SI EXISTE SINON SDL
    // CREER LE MODELE ET L'AFFECTER AU CONTROLLER
}

void controller_destroy(controller *c) {
    if(c == NULL) return;
    if(c->sdl != NULL) c->sdl->destroy_self(c->sdl);
    if(c->ncurses != NULL) c->ncurses->destroy_self(c->ncurses);
    destroy_model(c->m);

    free(c);
}