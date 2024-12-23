#pragma once

#define SCORE_INCREMENT 10

typedef enum {UP = 1, DOWN = 3, LEFT = 2, RIGHT = 4, NODIRECTION = 0} direction;

typedef enum { WALL = 100, EMPTY = 0 } cases;


typedef struct {
    int x;
    int y;
} position;

typedef struct {
    position **players;
    direction *directions;
    char *dead;

    int n_player;
    int n_player_alive;

    int **grid;
    unsigned nb_lignes_grid;
    unsigned nb_colonnes_grid;

    int *scores;
} model;

// Initialize the model with the correct amount of player
model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid, int **grid);

// Free the model
void destroy_model(model *);

// Move the player to dir only if player is not dead
int move_player(model *m, int player, direction dir);

// Check if player has collided with something and update
// dead[player]
int collision_player(model *m, int player);

int get_winner(model *);

// Return 1 if the game is over
int est_fini(model *m);

// Initialize positions of players
void init_positions(model *m);

// Initialize direction of players
void init_directions(model *m);



