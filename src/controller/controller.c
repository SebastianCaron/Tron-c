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
#include "../agents/rectiligne.h"

// NETWORK
#include "../network/network.h"
#include "../network/server.h"
#include "../network/client.h"

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

}


void controller_play_solo_j_vs_hara_kiri(controller *c){

    // Faire une liste des directions possible du bot
    // Regarder position suivante du joueur en fonction de sa direction
    // Prendre la direction du bot qui se rapproche le plus du joueur 

    int directionBotTest[4][2] = {{-1,0},{0,-1},{1,0},{0,1} };
    
    create_model(c, 2);
    int i = 0;
    direction *dirs = calloc(2, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    clock_t start, end;
    double duration;
    direction currentDir;

    while(!est_fini(c->m)){

        start = clock();
        for(i = 0; i < c->nb_view; i++){
            direction dir = c->views[i]->get_direction(c->views[i],1, dirs);
            if(dir!=0){
                currentDir = dir;
            }
        }
        move_player(c->m, 0, dirs[0]);

        int xJ = c->m->players[0]->x;
        int yJ = c->m->players[0]->y;
        if(currentDir!=0){
            xJ = c->m->players[0]->x + directionBotTest[currentDir-1][1];
            yJ = c->m->players[0]->y + directionBotTest[currentDir-1][0];
        }

        int distancePlusCourte = 100000;
        int indexDistancePlusCourte = 0;

        for (int i = 0; i < 4; i++) {
            int xBot = c->m->players[1]->x + directionBotTest[i][1];
            int yBot = c->m->players[1]->y + directionBotTest[i][0];
            if (c->m->grid[yBot][xBot] == EMPTY) {
                int distance = ((xBot - xJ) * (xBot - xJ)) + ((yBot - yJ) * (yBot - yJ));
                if (distancePlusCourte > distance) {
                    indexDistancePlusCourte = i+1;
                    distancePlusCourte = distance;
                }
            }
        }

        if (distancePlusCourte == 100000) {
            move_player(c->m, 1, RIGHT);
        } else {
            move_player(c->m, 1, indexDistancePlusCourte);
        }

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
            grid *g = load_map("./maps/map3.txt", c->views[i]->height, c->views[i]->width);
            c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
            free(g);
            return;
        }else{
            best = c->views[i];
        }
    }

    grid *g = load_map("./maps/map3.txt", best->height, best->width);
    c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
    // display_grid(g);
    free(g);
}
void create_model_with_grid(controller *c, int nb_player, grid *g){
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
                selected_option = 1;
                act = NO_ACTION;
                break;
            case MENU_MULTI:
                nbMenu = 2; 
                selected_option = 1;
                act = NO_ACTION;
                break;
            case PLAY_BOT_ALGO:
                controller_play_solo_j_vs_random(c);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_BOT_Q:
                controller_play_solo_j_vs_hara_kiri(c);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_MULTI:
                controller_play_multi(c);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_ONLINE:
                if(c->marker == 1){
                    controller_play_online_host(c, 2);
                }else{
                    controller_play_online_join(c);
                }
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
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

// NETWORK

void set_ip(controller *c, char *ip){
    c->ip = ip;
}
void set_port(controller *c, char *port){
    if(port == NULL){
        c->port = PORT;
        return;
    }
    int rport = 0;
    int i = 0;
    while(port[i] != '\0'){
        rport = rport * 10 + (port[i] - '0');
        i++;
    }
    c->port = rport;
}


// TODO : CHECK ERRORS & READY

void controller_play_online_host(controller *c, int nb_connect){
    server *s = init_serveur(PORT, nb_connect);

    int nb_player_connected = 1;
    actions act = NO_ACTION;

    while(nb_player_connected < nb_connect){
        if(wait_for_connections_timeout(s, NULL) == 1){
            nb_player_connected++;
        }
        for(int i = 0; i < c->nb_view; i++){
            c->views[i]->get_event(c->views[i],&act);
        }
        if(act == QUITTER) return;
        usleep(SPEED_FRM);
    }
    // printf("OK TOUT LE MONDE CONNECTE\n");

    create_model(c, nb_player_connected);
    // printf("MODEL OK\n");
    send_nb_player_all(s, nb_player_connected);
    // printf("NB PLAYER SEND TO EVERYONE\n");
    send_grid_all(s, c->m->nb_lignes_grid, c->m->nb_colonnes_grid, c->m->grid);
    // printf("GRID SEND TO EVERYONE\n");


    // WAIT FOR EVERYONE TO BE READY

    send_start_signal_all(s);
    // printf("START SIGNAL OK\n");


    direction *dirs = calloc(nb_player_connected, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    direction *dirso;
    clock_t start, end;
    double duration;
    int i = 0;
    int is_over = 0;
    // printf("DEBUT BOUCLE JEU\n");

    for(i = 0; i < c->nb_view; i++){
        // MET A JOUR LES VIEWS
        c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
    }

    while(!is_over){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            // RECUPERE LES INPUTS VIA LES VIEWS
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);
        // printf("DEPLACE JOUEUR SOLO\n");

        dirso = get_directions_all(s);
        // printf("RECUPERATION DES DIRECTIONS\n");


        for(int i = 0; i < nb_player_connected-1; i++){
            move_player(c->m, i+1, dirso[i]);
        }
        // printf("DEPLACE JOUEURS\n");


        collision_all(c->m);
        // printf("COLLISION ALL\n");


        for(i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VIEWS
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        // printf("SCREEN OK\n");


        send_positions_all(s, nb_player_connected, c->m->players);
        // printf("POSITIONS ENVOYEES OK\n");

        
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        usleep(SPEED_FRM - duration);

        is_over = est_fini(c->m);
        send_is_over_all(s, is_over);
        // printf("SIGNAL FIN? OK\n");


        // WAIT FOR EVERYONE TO BE READY
    }
    // printf("FIN DU JEU ET SERV'\n");
    free(dirs);
    close_connections(s);
    destroy_server(s);
}

void controller_play_online_join(controller *c){
    client *client = init_client(c->ip, c->port);

    if(client == NULL){
        printf("IMPOSSIBLE DE SE CONNECTER A %s:%d\n", c->ip, c->port);
        return;
    }else{
        printf("CONNECTE OK !\n");
    }

    int nb_player = client_get_nb_player(client);
    // printf("NBPLAYER OK ! %d\n", nb_player);

    grid *g = client_get_grid(client);
    // printf("GRID OK !\n");

    create_model_with_grid(c, nb_player, g);
    // printf("MODEL OK!\n");

    direction *dirs = calloc(nb_player, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    // direction *dirso;
    position *pos;
    clock_t start, end;
    double duration;
    int i = 0;
    int is_over = 0;

    while(client_get_start_signal(client) == 0){
        usleep(SPEED_FRM);
    }
    // printf("START !!!!\n");

    for(i = 0; i < c->nb_view; i++){
        // MET A JOUR LES VIEWS
        c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
    }

    while(is_over == 0){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            // RECUPERE LES INPUTS VIA LES VIEWS
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        // printf("GET DIRECTION !\n");


        client_send_movement(client, dirs[0]);
        // printf("MOUVEMENT SENT !\n");


        pos = client_get_positions(client);
        // printf("POSITION OK!\n");

        // SET POSITIONS
        if(set_positions(c->m, pos) != 0){
            // client_ask_for_grid(client);
        }

        // collision_all(c->m);
        // printf("COLLISION OK!\n");

        for(i = 0; i < c->nb_view; i++){
            // MET A JOUR LES VIEWS
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        // printf("SCREEN OK!\n");

        
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        usleep(SPEED_FRM - duration);

        // WAIT FOR EVERYONE TO BE READY

        is_over = client_is_over(client);
        // printf("ISOVER OK!\n");
    }

    free(dirs);
    destroy_client(client);
    // exit(EXIT_FAILURE);
}