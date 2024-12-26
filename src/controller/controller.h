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

    char *ip;
    int port;
    char marker;
} controller;



controller *init_controller(int, ...);

void create_model(controller *c, int nb_player);

void controller_play_solo_j_vs_bot(controller *c, direction (*get_dir_bot)(int, int, int **, position **, direction *), int);

void destroy_controller(controller *);

void go_to_menu(controller *);
void display_winner(controller *c);

void set_ip(controller *c, char *ip);
void set_port(controller *c, char *port);

void controller_play_online_join(controller *c);
void controller_play_online_host(controller *c, int nb_connect);
