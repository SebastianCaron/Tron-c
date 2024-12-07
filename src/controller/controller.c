#include <stdio.h>
#include <stdlib.h>

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

    // DES TRUCS

    return c;
}

void create_model(controller *c){
    // TODO
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

// TODO
void go_to_menu_principal(controller *c, actions *act){
    affiche_menu_principal(c->ncurses, act);
    affiche_menu_principal(c->sdl, act);
}
void go_to_menu_solo(controller *c, actions *act){
    affiche_menu_solo(c->ncurses, act);
    affiche_menu_solo(c->sdl, act);
}
void go_to_menu_multijoueur(controller *c, actions *act){
    affiche_menu_multijoueur(c->ncurses, act);
    affiche_menu_multijoueur(c->sdl, act);
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