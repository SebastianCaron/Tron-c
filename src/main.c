#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.h"
#include "model/model.h"
#include "views/view.h"
#include "views/view_ncurse.h"
#include "views/view_sdl.h"
#include "controller/controller.h"

#define TEST 0

int test(){

    return EXIT_SUCCESS;
}


int main(int argc, char **argv){

    if(TEST) return test();

    view *vncr = NULL;
    view *vsdl = NULL;
    char *ip = NULL;
    if(argc == 1){
        // INIT JEU AVEC SDL OU NCURSE ?
        return EXIT_SUCCESS;
    }

    char with_sdl = 0;
    char with_ncurse = 0;

    for(int i = 1; i < argc; i++){
        if(string_equal(argv[i],"-sdl") == 1){
            with_sdl = 1;
        }else if(string_equal(argv[i],"-ncurse") == 1){
            with_ncurse = 1;
        }else if(start_with(argv[i],"-ip") == 1){
            ip = argv[i] + 3;
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
        vsdl = init_view_sdl();
    }
    if(with_ncurse){
        vncr = init_view_ncurse();
    }

    if(with_sdl && with_ncurse){
        c = init_controller(vsdl, vncr);
    }else if(with_sdl){
        c = init_controller(vsdl);
    }else{
        c = init_controller(vncr);
    }


    go_to_menu(c);
    destroy_controller(c);
    // c->views[0]->nbMenu = 0;
    // c->views[1]->nbMenu = 0;

    return EXIT_SUCCESS;
}