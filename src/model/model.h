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

typedef enum {MUR = -1, VIDE = 0, PLAYER = 1} cases;


// Initialize the model with the correct amount of player
model *init_game(int nb_player);

// Free the model
void destroy(model *);

// Move the player to dir only if player is not dead
int move_player(int player,direction dir);

// Check if player has collided with something and update
// dead[player]
int collision_player(int player);

// Return 1 if the game is over
int est_fini();





