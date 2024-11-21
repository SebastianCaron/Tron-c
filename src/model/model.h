#pragma once

typedef struct {
    int x;
    int y;
} position;

typedef struct {
    position *players;
    char *dead;
    int n_player;

    int **grid;
} model;

typedef enum {UP, DOWN, LEFT, RIGHT} direction;




model *init_game(int nb_player);

void destroy(model *);

int move_player(int player,direction dir);

int collision_player(int player);

int est_fini();





