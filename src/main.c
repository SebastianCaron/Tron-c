/**
 * @file main.c
 * @date 2024-12-27
 * @brief Main entry point for the game application.
 * 
 * This file contains the main function which initializes the game, 
 * processes command-line arguments, sets up the views (SDL and ncurses), 
 * and starts the game controller.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// UTILS
#include "utils/utils.h"
// MODEL
#include "model/model.h"
// VIEWS
#include "views/view.h"
#include "views/view_ncurse.h"
#include "views/view_sdl.h"
// CONTROLLER
#include "controller/controller.h"
// NETWORK
#include "network/network.h"
#include "network/server.h"

#include "./agents/rectiligne.h"
#include "./agents/kamikaze.h"
#include "./agents/big.h"

#ifdef BOT_TRAINING
// #define BOT_TRAINING 0
#include "./agents/q.h"
// POUR l'entrainement du bot !
int bot_training(int max_episodes, int nb_bots){

    if (TTF_Init() == -1) {
        fprintf(stderr, "ERREUR SDL_ttf - %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERREUR SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    init_Q();
    load_Q_table("./q.bot");

    controller *c = init_controller(0);
    // controller *c = init_controller(1, init_view_sdl());

    int nb_victoire = 0;
    int nb_played = 0;
    // epsilon = initial_epsilon;

    // printf("%02.2f %%\n", (float) nb_played / (float) max_episodes * 100.0f);
    for(int i = 0; i < max_episodes; i++){
        // update_epsilon(i, max_episodes);
    
        // controller_play_train_vs_bot(c, rectiligne_get_direction, q_learning_bot, rand()%((5+1)-2) + 2);
        controller_play_train_vs_bot(c, rectiligne_get_direction, q_learning_bot, nb_bots);
        if(get_winner(c->m) == nb_bots) nb_victoire++;
        destroy_model(c->m);
        c->m = NULL;
        nb_played++;

        if ((i + 1) % 1000 == 0) {
            printf("\b\b\b\b\b\b\b%02.2f %%", (float) nb_played / (float) max_episodes * 100.0f);
        }
    }
    printf("\n");

    printf("%d / %d \t\t -- %0.2f %% de victoire \t %0.2f %% du test \n", nb_victoire, nb_played, (float) nb_victoire / (float) nb_played * 100.0f, (float) nb_played / (float) max_episodes * 100.0f);

    save_Q_table("./q.bot");
    destroy_controller(c);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
#endif


int main(int argc, char **argv){

    srand(time(NULL));

    view *vncr = NULL;
    view *vsdl = NULL;
    char *ip = "127.0.0.1";
    char *port = NULL;
    char with_sdl = 0;
    char with_ncurse = 0;
    char marker = 0;
    char *map = "./maps/map1.txt";
    char *nb_bots = "1";
    #ifdef BOT_TRAINING
    char train = 0;
    char *nb_ep = "200000";
    #endif
    
    if(argc == 1){
        with_sdl = 1;
    }


    for(int i = 1; i < argc; i++){
        if(string_equal(argv[i],"-sdl") == 1){
            with_sdl = 1;
        }else if(string_equal(argv[i],"-ncurse") == 1 || string_equal(argv[i],"-nc") == 1 || string_equal(argv[i],"-ncurses") == 1){
            with_ncurse = 1;
        }else if(start_with(argv[i],"-ip") == 1){
            ip = argv[i] + 3;
        }else if(start_with(argv[i],"-port") == 1){
            port = argv[i] + 5;
        }else if(string_equal(argv[i],"-h") == 1){
            marker = 1;
        }else if(start_with(argv[i],"-bots") == 1){
            nb_bots = argv[i] + 5;
        }else if(start_with(argv[i],"-map") == 1){
            map = argv[i] + 4;
        }
        #ifdef BOT_TRAINING
        else if(string_equal(argv[i],"-train") == 1){
            train = 1;
        }else if(start_with(argv[i],"-ep") == 1){
            nb_ep = argv[i] + 3;
        }
        #endif
        else{
            printf("INCORRECT PARAMETER %s\n", argv[i]);
            printf("[USAGE] %s\n WITH PARAMETERS\n", argv[0]);
            printf("\t -sdl : TO USE SDL LIB\n");
            printf("\t -ncurses : TO USE NCURSE LIB\n");
            printf("\t -ip : TO CONFIGURE THE IP ADDRESS\n");
            printf("\t -port : TO CONFIGURE THE PORT\n");
            printf("\t -h : TO HOST A GAME\n");
            printf("\t -bots : TO SET THE NUMBER OF BOTS OR PLAYERS IN ONLINE GAMES\n");
            printf("\t -map : TO SET THE PATH TO THE MAP TO USE\n");
            #ifdef BOT_TRAINING
            printf("\t -train : TO TRAIN THE BOT\n");
            printf("\t -ep : TO CHOOSE THE NUMBER OF ROUND\n");
            printf("\nEXAMPLE : %s -train -ep10000 -bots5", argv[0]);
            #endif


            printf("\nEXAMPLE : %s -sdl -ncurses -ip192.168.0.0 -bots5 -map./maps/map3.txt\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    #ifdef BOT_TRAINING
    if(train == 1){
        return bot_training(atoi(nb_ep), atoi(nb_bots));
    }else{
        init_Q();
        load_Q_table("./q.bot");
    }
    #endif
    

    if(with_sdl == 0 && with_ncurse == 0) with_sdl = 1;

    controller *c;
    if(with_sdl){
        if (TTF_Init() == -1) {
            fprintf(stderr, "ERREUR SDL_ttf - %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
        }

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "ERREUR SDL : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        vsdl = init_view_sdl();
    }
    if(with_ncurse){
        setlocale(LC_ALL, "");
        initscr();
        timeout(-1);
        noecho();
        cbreak();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        init_colors();

        vncr = init_view_ncurse();
    }


    // if(with_sdl == 1 && with_ncurse == 1){
    //     c = init_controller(2, vncr, vsdl);
    // }else if(with_sdl){
    //     c = init_controller(1, vsdl);
    // }else{
    //     c = init_controller(1, vncr);
    // }

    c = init_controller(0);
    if(with_sdl){
        add_view(c, vsdl);
    }
    if(with_ncurse){
        add_view(c, vncr);
    }

    set_ip(c, ip);
    set_port(c, port);
    c->marker = marker;
    set_nb_bots(c, nb_bots);
    set_map(c, map);

    if(marker == 1){
        c->s = init_serveur(c->port, c->nb_bots + 1);
    }

    go_to_menu(c);
    // for(int i = 0; i < 50; i++){
    //     controller_play_solo_j_vs_bot(c, immitateur_get_direction, c->nb_bots);

    // }


    if(marker == 1){
        destroy_server(c->s);
    }
    destroy_controller(c);
    if(with_sdl){
        TTF_Quit();
        SDL_Quit();
    }
    if(with_ncurse){
        clear();
        echo();
        endwin();
    }
    

    return EXIT_SUCCESS;
}