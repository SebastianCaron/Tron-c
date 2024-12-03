#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.h"
#include "model/model.h"
#include "views/view.h"
#include "views/view_ncurse.h"
#include "views/view_sdl.h"
#include "controler/controler.h"


int main(int argc, char **argv){
    view *vncr;
    view *vsdl;
    if(argc == 1){
        // INIT LE JEU AVEC NCURSE
        // system("printf '\\e[8;40;100t'"); // CHANGE SIZE TO 40L, 100C
        vncr = init_view_ncurse();
        vncr->destroy_self(vncr);
        return EXIT_SUCCESS;
    }

    char with_sdl = 0;
    char with_ncurse = 0;

    for(int i = 1; i < argc+1; i++){
        if(string_equal(argv[i],"-sdl")){
            with_sdl = 1;
        }else if(string_equal(argv[i],"-ncurse")){
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



    if(with_sdl){
        vsdl = init_view_sdl();
        init_controller(vsdl);
    }
    if(with_ncurse){
        vncr = init_view_ncurse();
        init_controller(vncr);
    }




    return EXIT_SUCCESS;
}