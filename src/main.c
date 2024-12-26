#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "utils/utils.h"
#include "model/model.h"

#include "views/view.h"
#include "views/view_ncurse.h"
#include "views/view_sdl.h"

#include "controller/controller.h"

#include "network/network.h"

#define TEST 0

int test(){

    return EXIT_SUCCESS;
}


int main(int argc, char **argv){

    if(TEST) return test();

    srand(time(NULL));

    view *vncr = NULL;
    view *vsdl = NULL;
    char *ip = "127.0.0.1";
    char *port = NULL;
    char with_sdl = 0;
    char with_ncurse = 0;
    char marker = 0;

    if(argc == 1){
        with_sdl = 1;
    }


    for(int i = 1; i < argc; i++){
        if(string_equal(argv[i],"-sdl") == 1){
            with_sdl = 1;
        }else if(string_equal(argv[i],"-ncurse") == 1){
            with_ncurse = 1;
        }else if(start_with(argv[i],"-ip") == 1){
            ip = argv[i] + 3;
        }else if(start_with(argv[i],"-port") == 1){
            port = argv[i] + 5;
        }else if(string_equal(argv[i],"-h") == 1){
            marker = 1;
        }
        else{
            printf("INCORRECT PARAMETER %s\n", argv[i]);
            printf("[USAGE] %s\n WITH PARAMETERS\n", argv[0]);
            printf("\t -sdl : TO USE SDL LIB\n");
            printf("\t -ncurse : TO USE NCURSE LIB\n");
            printf("\t -ip : TO CONFIGURE THE IP ADDRESS\n");

            printf("EXAMPLE : %s -sdl -ncurse -ip192.168.0.0\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

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
        vncr = init_view_ncurse();
    }

    if(with_sdl == 1 && with_ncurse == 1){
        c = init_controller(2, vncr, vsdl);
    }else if(with_sdl){
        c = init_controller(1, vsdl);
    }else{
        c = init_controller(1, vncr);
    }

    set_ip(c, ip);
    set_port(c, port);
    c->marker = marker;

    go_to_menu(c);
    destroy_controller(c);

    if(with_sdl){
        TTF_Quit();
        SDL_Quit();
    }

    return EXIT_SUCCESS;
}