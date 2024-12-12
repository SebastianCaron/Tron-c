#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"
#include "./controller.h"


void controller_play_solo_j_vs_random(controller *c){
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    // HANDLE ERROR
    init_directions(c->m, dirs);
    clock_t start, end;
    double duration;

    while(!est_fini(c->m)){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            // RECUPERE LES INPUTS VIA LES VIEWS
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);
        move_player(c->m, 1, dirs[1]);

        collision_player(c->m, 0);
        collision_player(c->m, 1);


        for(i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VIEWS
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        usleep(SPEED_FRM - duration);
    }
    free(dirs);
    // display_grid_i(c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
}
void controller_play_multi(controller *c){
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    // HANDLE ERROR
    init_directions(c->m, dirs);
    clock_t start, end;
    double duration;

    while(!est_fini(c->m)){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            // RECUPERE LES INPUTS VIA LES VIEWS
            c->views[i]->get_direction(c->views[i],2, dirs);
        }
        move_player(c->m, 0, dirs[0]);
        move_player(c->m, 1, dirs[1]);

        collision_player(c->m, 0);
        collision_player(c->m, 1);


        for(i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VIEWS
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        usleep(SPEED_FRM - duration);
    }
    free(dirs);
    // display_grid_i(c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
}

void controller_play_online(controller *c){

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
            grid *g = load_map("./maps/map1.txt", c->views[i]->height, c->views[i]->width);
            c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
            free(g);
            return;
        }else{
            best = c->views[i];
        }
    }

    grid *g = load_map("./maps/map1.txt", best->height, best->width);
    c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
    free(g);
}

void destroy_controller(controller *c) {
    if(c == NULL) return;
    for(int i = 0; i < c->nb_view; i++){
        c->views[i]->destroy_self(c->views[i]);
    }
    free(c->views);
    destroy_model(c->m);
    free(c);
}


void go_to_menu(controller *c){
    // ??????
    int nbMenu = c->views[0]->nbMenu || c->views[0]->nbMenu;
    
    actions act = NO_ACTION;
    int selected_option = 0;
    while(1){  
        for(int i = 0; i < c->nb_view; i++){
            c->views[i]->affiche_menu(c->views[i], &selected_option, nbMenu);
        }

        for(int i = 0; i < c->nb_view; i++){
            c->views[i]->get_action(c->views[i], &act, &selected_option, nbMenu);
        }

        switch (act) {
            case MENU_SOLO:
                nbMenu = 1; 
                break;
            case MENU_MULTI:
                nbMenu = 2; 
                break;
            case PLAY_BOT_ALGO:
                controller_play_solo_j_vs_random(c);
                nbMenu = 0;
                act = NO_ACTION;
                break;
            case PLAY_MULTI:
                controller_play_multi(c);
                // A VOIR
                // afficheGagnant();
                // act = MENU_PRINCIPAL;

                act= NO_ACTION;
                nbMenu = 0; 
                break;
            case RETOUR:
                nbMenu = 0; 
                break;
            case QUITTER:
                return; 
            default:    
                break;
        }

        // POUR EVITER DE RAFFRAICHIR TROP SOUVENT
        usleep(10000);
    }
}

