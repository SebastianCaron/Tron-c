#pragma once

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"


typedef struct {
    model *m;
    view *sdl;
    view *ncurses;
} controller;



controller *init_controller(view *, view *);

void controllerPlay(model *, view *);

void destroy_controller(model *, view *);


void go_to_menu_principal(controller *, actions *act);
void go_to_menu_solo(controller *, actions *act);
void go_to_menu_multijoueur(controller *, actions *act);
void launch(controller *c);


