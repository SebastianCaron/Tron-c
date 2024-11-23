#include <stdio.h>
#include <stdlib.h>

#include "model/model.h"
#include "views/view.h"
#include "views/view_ncurse.h"
#include "views/view_sdl.h"
#include "controler/controler.h"


int main(int argc, char **argv){
    if(argc == 1){
        // INIT LE JEU AVEC SDL

        return EXIT_SUCCESS;
    }

    char with_sdl = 0;
    char with_ncurse = 0;

    for(int i = 1; i < argc+1; i++){
        if(argv[i] == "-sdl"){
            with_sdl = 1;
        }else if(argv[i] == "-ncurse"){
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

    }
    if(with_ncurse){

    }


    return EXIT_SUCCESS;
}