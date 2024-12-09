#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"
#include "./controller.h"


void controller_play_solo_j_vs_random(controller *c){
    create_model(c, 2);
    int i = 0;
    int *scores = calloc(2, sizeof(int));
    direction dir = NODIRECTION;
    while(!est_fini(c->m)){
        for(i = 0; i < c->nb_view; i++){
            c->views[i]->get_direction(c->views[i], &dir);
        }
        move_player(c->m, 0, dir);
        move_player(c->m, 1, UP);


        for(i = 0; i < c->nb_view; i++){
            c->views[i]->update_screen(c->views[i],2, scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        usleep(100000);
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

void create_model(controller *c, int nb_player){
    view *best = NULL;
    
    for(unsigned i = 0; i < c->nb_view; i++){
        if(c->views[i]->type == 'n'){
            grid *g = load_map("./maps/map1.txt", c->views[i]->width, c->views[i]->height);
            c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
            return;
        }else{
            best = c->views[i];
        }
    }
    grid *g = load_map("./maps/map1.txt", best->width, best->height);
    printf("G : %d %d\n", best->width, best->height);
    display_grid(g);
    c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
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
            case PLAY_BOT_ALGO:
                controller_play_solo_j_vs_random(c);
                return;
            case QUITTER:
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
                return;
            default:
                break;
        }

        // POUR EVITER DE RAFFRAICHIR TROP SOUVENT
        usleep(10000);
    }
}
