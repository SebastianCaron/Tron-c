#pragma once

#define SCORE_INCREMENT 10

typedef enum {UP = 1, DOWN = 3, LEFT = 2, RIGHT = 4, NODIRECTION = 0} direction;

typedef enum { WALL = 100, EMPTY = 0 } cases;

/** 
∗ @struct position
∗ @brief easy acces to a player position
*/
typedef struct {
    int x;
    int y;
} position;


/** 
∗ @struct model
∗ @brief represents the game
*/
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

/**
 * @brief Initialize the model with the correct amount of player
 * @param nb_player the number of player
 * @param nb_lignes_grid the number of lines for the grid
 * @param nb_colonnes_grid the number of columns for the grid
 * @param grid the grid
 * @return the model
 */
model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid, int **grid);

/**
 * @brief Free the model
 */
void destroy_model(model *);

/**
 * @brief Move the player at index player to the direction dir
 * @param m the model
 * @param player index of the player
 * @param dir the direction for the player
 * @return 0 if the player has not moved
 */
int move_player(model *m, int player, direction dir);

/**
 * @brief Change positions of player and return 1 if distance > 1
 * @param m the model
 * @param p positions for the players in the model
 * @return 0 If the players have only moved one square
 */
int set_positions(model *m, position *p);

/**
 * @brief Check if player has collided with something and update the model
 * @param m the model
 * @param player player index
 * @return 1 if a collision has occurred
 */
int collision_player(model *m, int player);

/**
 * @brief Check if all players has collided with something and update the model
 * @param m the model
 * @return 1 if a collision has occurred
 */
void collision_all(model *m);

/**
 * @brief Return the index of the winner (-1 if no player is alive)
 * @param m the model
 * @return 1the index of the winner
 */
int get_winner(model *m);

/**
 * @brief Kills all players and revives the player with the player index
 * @param m the model
 * @param player player index
 */
void set_winner(model *m, int player);

/**
 * @brief Check if the game is over
 * @param m the model
 * @return 1 if the game is over
 */
int est_fini(model *m);

/**
 * @brief Initialize positions of players
 * @param m the model
 */
void init_positions(model *m);

/**
 * @brief Initialize directions of players
 * @param m the model
 */
void init_directions(model *m);



