#pragma once

#include <stdarg.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"

#define SPEED_FRM 100000


typedef struct {
    model *m;
    unsigned nb_view;
    view **views;
} controller;



controller *init_controller(view *, ...);

void create_model(controller *c, int nb_player);
void controller_play_solo_j_vs_random(controller *c);

void destroy_controller(controller *);

void go_to_menu(controller *);
void display_winner(controller *c);

