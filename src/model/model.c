#include <stdio.h>
#include <stdlib.h>

#include <model/model.h>

model *init_game(int nb_player){
    model *game = (model *)malloc(sizeof(model));
    if(!game){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    position *players = (position *)malloc(sizeof(position));
    if(!players){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    game->players = players;

    char *dead = (char *)calloc(nb_player, sizeof(char));
    if(!dead){
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    game->dead = dead;

    game->n_player= nb_player;

    // game->grid = ...
    
}