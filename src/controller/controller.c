#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"
#include "./controller.h"

// AGENTS
#include "../agents/random.h"
#include "../agents/rectiligne.h"

void controller_play_solo_j_vs_random(controller *c){
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    
    clock_t start, end;
    double duration;

    while(!est_fini(c->m)){
        start = clock();
        
        for(i = 0; i < c->nb_view; i++){
            // RECUPERE LES INPUTS VIA LES VIEWS
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);
        move_player(c->m, 1, rectiligne_get_direction(c->m->nb_lignes_grid, c->m->nb_colonnes_grid, c->m->grid, c->m->players, c->m->directions));
            
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


void controller_play_solo_j_vs_hara_kiri(controller *c){

    // Faire une liste des directions possible du bot
    // Regarder position suivante du joueur en fonction de sa direction
    // Prendre la direction du bot qui se rapproche le plus du joueur 

    // direction directionsBot[4] = {UP, DOWN, RIGHT, LEFT};
    int directionBotTest[4][2] = {{0,1},{0,-1}, {1,0},{-1,0}};
    
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    clock_t start, end;
    double duration;

    while(!est_fini(c->m)){
        // direction directionPossibles[3]={0,0,0};
        // int cptPossible=0;

        start = clock();
        // A garder
        for(i = 0; i < c->nb_view; i++){
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);

        // A changer
        // for(int i=0; i<4;i++){
        //     int x = c->m->players[1]->x+ directionBotTest[i][0] ;
        //     int y = c->m->players[1]->y+ directionBotTest[i][1];
        //     if(c->m->grid[x][y] == EMPTY ){
        //         directionPossibles[cptPossible++] = directionsBot[i];
        //         printf("%d good\n", i);   
        //     }else{
        //         printf("%d pasgood\n", i);
        //     }
            
        // }
        // if(cptPossible>=1){
        //     move_player(c->m, 1, directionsBot[aleaEntreBornes(0, cptPossible-1)]);
        // }else{
        //     move_player(c->m, 1, UP);
        // }

        int x = c->m->players[1]->x+ directionBotTest[3][0];
        int y = c->m->players[1]->y+directionBotTest[3][1];
        printf("%d test\n", c->m->grid[x][y]); 
        if(c->m->grid[x][y] == 0 ){
                move_player(c->m, 1, RIGHT);
                printf("%d good\n", i);   
        }else{
                printf("%d pasgood\n", i);
        }

        // A garder 
        collision_player(c->m, 0);
        collision_player(c->m, 1);

        for(i = 0; i < c->nb_view; i++){
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }

        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        usleep(SPEED_FRM - duration);
    }
    free(dirs);
    
}  


void controller_play_multi(controller *c){
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
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

controller *init_controller(int nb_views, ...){
    controller *c = calloc(1, sizeof(controller));
    if(c == NULL){
        perror("[CONTROLLER] erreur allocation de la structure controller.");
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, nb_views);

    c->nb_view = nb_views;
    c->views = calloc(nb_views, sizeof(view *));
    if (c->views == NULL) {
        perror("[CONTROLLER] erreur allocation liste views.");
        free(c);
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (i < nb_views) {
        view *nv = va_arg(args, view *);

        if (nv == NULL) {
            break;
        }

        c->views[i] = nv;
        i++;
    }
    va_end(args);
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
    // printf("NB VIEW : %d\n", c->nb_view);
    for(int i = 0; i < c->nb_view; i++){
        c->views[i]->destroy_self(c->views[i]);
    }
    
    free(c->views);
    destroy_model(c->m);
    free(c);
}


void go_to_menu(controller *c){
    int nbMenu = c->views[0]->nbMenu;
    actions act = NO_ACTION;
    int selected_option = 1;
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
                display_winner(c);
                
                act = RETOUR;
                break;
            case PLAY_BOT_Q:
                controller_play_solo_j_vs_hara_kiri(c);
                display_winner(c);
                
                act = RETOUR;
                break;
            case PLAY_MULTI:
                controller_play_multi(c);
                display_winner(c);
                act = RETOUR;
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

void display_winner(controller *c){
    int winner = get_winner(c->m);    

    for(int i = 0; i < c->nb_view;i++){
        c->views[i]->nbMenu = MENU_PRINCIPAL; //JSP SI CEST UTIL
        c->views[i]->affiche_winner(c->views[i], winner);
    }
    int nbMenu = c->views[0]->nbMenu;
    actions act = NO_ACTION;
    int selected_option = 1;

    while(1){
        for(int i = 0; i < c->nb_view; i++){
            c->views[i]->get_action(c->views[i], &act, &selected_option, nbMenu);
        }
        if(act != NO_ACTION){
            return;
        }
    }
}