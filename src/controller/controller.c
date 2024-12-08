#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"
#include "./controller.h"


void controllerPlay(model *m, view *v){
    while(!est_fini(m)){
        direction dir = v->get_direction(v);
        move_player(m, m->n_player, dir);

        v->update_change_screen(v, m->grid, m->nb_lignes_grid, m->nb_colonnes_grid);
    }
}

controller *init_controller(view *v, ...){
    controller *c = calloc(1, sizeof(controller));
    if(c == NULL){
        perror("[CONTROLLER] erreur allocation de la structure controller.");
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, v);

    c->nb_view = 1;
    c->views = realloc(c->views, sizeof(view *) * c->nb_view);
    c->views[c->nb_view-1] = v;

    while (1) {
        view *nv = va_arg(args, view *);

        if (nv == NULL) {
            break;
        }

        c->nb_view += 1;

        c->views = realloc(c->views, sizeof(view *) * c->nb_view);
        if(c->views == NULL){
            perror("[CONTROLLER] erreur allocation liste views.");
            exit(EXIT_FAILURE);
        }

        c->views[c->nb_view-1] = nv;
    }

    return c;
}

void create_model(controller *c){
    // TODO
    // RECUPERER TAILLE NCURSES SI EXISTE SINON SDL
    // CREER LE MODELE ET L'AFFECTER AU CONTROLLER
}

void controller_destroy(controller *c) {
    if(c == NULL) return;
    for(int i = 0; i < c->nb_view; i++){
        c->views[i]->destroy_self(c->views[i]);
    }
    destroy_model(c->m);
    free(c);
}

// TODO
void go_to_menu_principal(controller *c, actions *act){


    // AFFICHER MENU
    // RECUPERER INPUT
    // SLEEP

    // EFFECTUER ACTIONS
}
void go_to_menu_solo(controller *c, actions *act){

}
void go_to_menu_multijoueur(controller *c, actions *act){

}


void launch(controller *c){
    // TODO : VERIFIER C != NULL
    actions act = MENU_PRINCIPAL;
    while(1){
        switch (act)
        {
        case QUITTER:
            break;
        case MENU_PRINCIPAL:
            go_to_menu_principal(c, &act);
            break;
        case MENU_MULTI:
            go_to_menu_multijoueur(c, &act);
            break;
        case MENU_SOLO:
            go_to_menu_solo(c, &act);
            break;
        default:
            break;
        }
    }


}