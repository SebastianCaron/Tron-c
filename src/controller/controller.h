#pragma once

#include "../model/model.h"
#include "../views/view_sdl.h"
#include "../views/view_ncurse.h"
#include "../views/view.h"

typedef struct {
    model *m;
    view *sdl;
    view *ncurses;
} controller;

controller *init_controller(view *, view *);

void controllerPlay(model *, view *);

void destroy_controller(model *, view *);


void go_to_menu_principal(controller *);
void go_to_menu_solo(controller *);
void go_to_menu_multijoueur(controller *);


