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
#include "../agents/kamikaze.h"
#include "../agents/big.h"

// NETWORK
#include "../network/network.h"
#include "../network/server.h"
#include "../network/client.h"

void controller_play_solo_j_vs_bot(controller *c, direction (*get_dir_bot)(int, int, int **, position **, direction *, int), int nb_bots){
    create_model(c, nb_bots+1);
    int i = 0;
    direction *dirs = calloc(nb_bots+1, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    clock_t start, end;
    double duration;
    c->views[i]->update_screen(c->views[i], c->m->n_player, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);


    while(!est_fini(c->m)){

        start = clock();
        for(i = 0; i < c->nb_view; i++){
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);
        
        for(int i = 0; i < nb_bots; i++){
            dirs[i+1] = get_dir_bot(c->m->nb_lignes_grid, c->m->nb_colonnes_grid, c->m->grid, c->m->players, c->m->directions, i+1);
            move_player(c->m, i+1, dirs[i+1]);
        }

        collision_all(c->m);

        for(i = 0; i < c->nb_view; i++){
            c->views[i]->update_change_screen(c->views[i], c->m->n_player, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid, c->m->players);
        }

        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        if(SPEED_FRM - duration > 0) usleep(SPEED_FRM - duration);
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
        if(SPEED_FRM - duration > 0) usleep(SPEED_FRM - duration);
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
            grid *g = load_map(c->map, c->views[i]->height, c->views[i]->width);
            c->m = init_game(nb_player, g->nb_lignes, g->nb_colonnes, g->grid);
            free(g);
            return;
        }else{
            best = c->views[i];
        }
    }

    grid *g = load_map(c->map, best->height, best->width);
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
                controller_play_solo_j_vs_bot(c, rectiligne_get_direction, c->nb_bots);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_BOT_ESPACE:
                controller_play_solo_j_vs_bot(c, espace_vital_get_direction, c->nb_bots);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_BOT_COPY:
                controller_play_solo_j_vs_bot(c, immitateur_get_direction, c->nb_bots);
                display_winner(c);
                destroy_model(c->m);
                c->m = NULL;
                act = RETOUR;
                break;
            case PLAY_BOT_Q:
                controller_play_solo_j_vs_bot(c, kamikaze_get_direction, c->nb_bots);
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
                    controller_play_online_host(c, c->nb_bots + 1);
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

void set_map(controller *c, char *map){
    FILE *file = fopen(map, "r");
    if (file) {
        fclose(file);
        c->map = map;
    } else {
        c->map = "./maps/map1.txt";
    }
}

void set_nb_bots(controller *c, char *nb_bots){
    if(nb_bots == NULL){
        c->nb_bots = 1;
        return;
    }
    int res = 0;
    int i = 0;
    while(nb_bots[i] != '\0'){
        if(nb_bots[i] >= '0' && nb_bots[i] <= '9'){
            res = res * 10 + (nb_bots[i] - '0');
        }else{
            c->nb_bots = 1;
            return;
        }
        i++;
    }
    c->nb_bots = (res >= 8 ? 7 : res);
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
    create_model(c, nb_player_connected);
    send_nb_player_all(s, nb_player_connected);
    send_grid_all(s, c->m->nb_lignes_grid, c->m->nb_colonnes_grid, c->m->grid);

    // WAIT FOR EVERYONE TO BE READY

    send_start_signal_all(s);

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
    for(i = 0; i < c->nb_view; i++){
        c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
    }

    while(!is_over){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        move_player(c->m, 0, dirs[0]);

        dirso = get_directions_all(s);
        for(int i = 0; i < nb_player_connected-1; i++){
            move_player(c->m, i+1, dirso[i]);
        }

        collision_all(c->m);

        for(i = 0; i < c->nb_view; i++){
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }

        send_positions_all(s, nb_player_connected, c->m->players);
        
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        if(SPEED_FRM - duration > 0) usleep(SPEED_FRM - duration);

        is_over = est_fini(c->m);
        send_is_over_all(s, is_over);
        // printf("SIGNAL FIN? OK\n");


        // WAIT FOR EVERYONE TO BE READY
    }

    send_winner_all(s, get_winner(c->m));
    free(dirs);
    close_connections(s);
    destroy_server(s);
}

void controller_play_online_join(controller *c){
    client *client = init_client(c->ip, c->port);
    // actions act = NO_ACTION;
    int interrupt = 0;

    if(client == NULL){
        printf("IMPOSSIBLE DE SE CONNECTER A %s:%d\n", c->ip, c->port);
        return;
    }else{
        printf("CONNECTE OK !\n");
    }

    // NON BLOQUANT
    // while(is_in_data_available(client, NBJOUEUR) == 0){
    //     for(int i = 0; i < c->nb_view; i++){
    //         c->views[i]->get_event(c->views[i], &act);
    //     }
    //     if(act == QUITTER){
    //         interrupt = 1;
    //         break;
    //     }
    //     usleep(SPEED_FRM);
    //     retrieve_data_client(client);
    // }

    int nb_player = client_get_nb_player(client);

    grid *g = client_get_grid(client);
    create_model_with_grid(c, nb_player, g);

    direction *dirs = calloc(nb_player, sizeof(direction));
    if(dirs == NULL){
        perror("[CONTROLLER] erreur allocation directions\n");
        return;
    }
    position *pos;
    clock_t start, end;
    double duration;
    int i = 0;
    int is_over = 0;


    while(client_get_start_signal(client) == 0){
        // for(int i = 0; i < c->nb_view; i++){
        //     c->views[i]->get_event(c->views[i],&act);
        // }
        // if(act == QUITTER){
        //     is_over = 1;
        //     interrupt = 1;
        //     break;
        // }
        usleep(SPEED_FRM);
    }
    for(i = 0; i < c->nb_view; i++){
        c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
    }

    while(is_over == 0 && interrupt == 0){
        start = clock();
        for(i = 0; i < c->nb_view; i++){
            c->views[i]->get_direction(c->views[i],1, dirs);
        }
        client_send_movement(client, dirs[0]);

        pos = client_get_positions(client);
        if(set_positions(c->m, pos) != 0){
            // client_ask_for_grid(client); // PAS IMPLEMENTE PROPREMENT
        }

        // collision_all(c->m); // ON SERV

        for(i = 0; i < c->nb_view; i++){
            c->views[i]->update_screen(c->views[i],2, c->m->scores, c->m->grid, c->m->nb_lignes_grid, c->m->nb_colonnes_grid);
        }
        
        end = clock();
        duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
        if(SPEED_FRM - duration > 0) usleep(SPEED_FRM - duration);

        // WAIT FOR EVERYONE TO BE READY

        is_over = client_is_over(client);
    }

    if(interrupt == 0){
        int winner = client_get_winner(client);
        set_winner(c->m, winner);
    }else{
        set_winner(c->m, -1);
    }
    

    free(dirs);
    destroy_client(client);
}