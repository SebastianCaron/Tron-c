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

    view *vncr = init_view_sdl();
    controller *c = init_controller(vncr);
    go_to_menu(c);

    destroy_controller(c);
    return EXIT_SUCCESS;
}


int main(int argc, char **argv){

    if(TEST) return test();

    view *vncr = NULL;
    view *vsdl = NULL;
    if(argc == 1){
        // INIT LE JEU AVEC NCURSE
        // system("printf '\\e[8;40;100t'"); // CHANGE SIZE TO 40L, 100C
        vncr = init_view_ncurse();
        vncr->destroy_self(vncr);
        return EXIT_SUCCESS;
    }

    char with_sdl = 0;
    char with_ncurse = 0;

    for(int i = 1; i < argc; i++){
        if(string_equal(argv[i],"-sdl") == 1){
            with_sdl = 1;
        }else if(string_equal(argv[i],"-ncurse") == 1){
            with_ncurse = 1;
        }
        else{
            printf("INCORRECT PARAMETER %s\n", argv[i]);
            printf("[USAGE] %s\n WITH PARAMETERS\n", argv[0]);
            printf("\t -sdl : TO USE SDL LIB\n");
            printf("\t -ncurse : TO USE NCURSE LIB\n");

            printf("EXAMPLE : %s -sdl -ncurse\n", argv[0]);
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