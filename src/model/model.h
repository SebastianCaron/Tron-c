#pragma once

typedef struct {
    int x;
    int y;
} position;

typedef struct {
    position **players;
    char *dead;
    int n_player;

    int n_player_alive;


    int **grid;
    unsigned nb_lignes_grid;
    unsigned nb_colonnes_grid;
} model;

typedef enum {UP, DOWN, LEFT, RIGHT} direction;

typedef enum { WALL = 100, EMPTY = 0 } cases;


// Initialize the model with the correct amount of player
model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid);

// Free the model
void destroy(model *);

// Move the player to dir only if player is not dead
int move_player(model *m, int player, direction dir);

// Check if player has collided with something and update
// dead[player]
int collision_player(model *m, int player);

// Return 1 if the game is over
int est_fini(model *m);





