#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

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

    c->nb_view -= 1;
    // printf("%d\n", c->nb_view);
    // exit(EXIT_SUCCESS);

    return c;
}

void create_model(controller *c){
    // TODO
    // RECUPERER TAILLE NCURSES SI EXISTE SINON SDL
    // CREER LE MODELE ET L'AFFECTER AU CONTROLLER
}

void destroy_controller(controller *c) {
    if(c == NULL) return;
    for(int i = 0; i < c->nb_view; i++){
        c->views[i]->destroy_self(c->views[i]);
    }
    destroy_model(c->m);
    free(c);
}

void go_to_menu_principal(controller *c){
    actions act = NO_ACTION;
    int selected_option = 0;
    while(1){
        for(int i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VUES
            c->views[i]->affiche_menu_principal(c->views[i], &selected_option);
        }

        for(int i = 0; i < c->nb_view; i++){
            // RECUPERE LES ACTIONS
            c->views[i]->get_action(c->views[i], &act, &selected_option);
        }


        switch (act)
        {
            // EFFECTUE L'ACTION
            case MENU_SOLO:
                go_to_menu_solo(c);
                return;
            case MENU_MULTI:
                go_to_menu_multijoueur(c);
                return;
            case QUITTER:
                destroy_controller(c);
                return;
            default:
                break;
        }

        // POUR EVITER DE RAFFRAICHIR TROP SOUVENT
        usleep(10000);
    }
}


void go_to_menu_solo(controller *c){
    actions act = NO_ACTION;
    int selected_option = 0;
    while(1){
        for(int i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VUES
            c->views[i]->affiche_menu_solo(c->views[i], &selected_option);
        }

        for(int i = 0; i < c->nb_view; i++){
            // RECUPERE LES ACTIONS
            c->views[i]->get_action(c->views[i], &act, &selected_option);
        }


        switch (act)
        {
            case QUITTER:
                destroy_controller(c);
                return;
            default:
                break;
        }

        // POUR EVITER DE RAFFRAICHIR TROP SOUVENT
        usleep(10000);
    }
}
void go_to_menu_multijoueur(controller *c){
    actions act = NO_ACTION;
    int selected_option = 0;
    while(1){
        for(int i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VUES
            c->views[i]->affiche_menu_multijoueur(c->views[i], &selected_option);
        }

        for(int i = 0; i < c->nb_view; i++){
            // RECUPERE LES ACTIONS
            c->views[i]->get_action(c->views[i], &act, &selected_option);
        }


        switch (act)
        {
            case QUITTER:
                destroy_controller(c);
                return;
            default:
                break;
        }

        // POUR EVITER DE RAFFRAICHIR TROP SOUVENT
        usleep(10000);
    }
}
