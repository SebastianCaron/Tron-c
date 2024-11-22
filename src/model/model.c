#include <stdio.h>
#include <stdlib.h>

#include <model/model.h>

model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid){
    model *game = (model *) malloc(sizeof(model));
    if(!game){
        perror("[MODEL] erreur allocation de la structure model.");
        exit(EXIT_FAILURE);
    }

    position *players = (position *) malloc(sizeof(position) * nb_player);
    if(!players){
        perror("[MODEL] erreur allocation des positions pour les joueurs.");
        exit(EXIT_FAILURE);
    }
    game->players = players;

    char *dead = (char *) calloc(nb_player, sizeof(char));
    if(!dead){
        perror("[MODEL] erreur allocation des etats de vie des joueurs.");
        exit(EXIT_FAILURE);
    }
    game->dead = dead;

    game->n_player= nb_player;


    game->grid = calloc(nb_lignes_grid, sizeof(int *));
    if(!game->grid){
        perror("[MODEL] erreur allocation des lignes de la grille de jeu.");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < nb_lignes_grid; i++){
        game->grid[i] = (int *) calloc(nb_colonnes_grid, sizeof(int));
        if(!game->grid[i]){
            perror("[MODEL] erreur allocation d'une colonne de la grille de jeu.");
            exit(EXIT_FAILURE);
        }
    }

    return game;
}