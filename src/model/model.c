#include <stdio.h>
#include <stdlib.h>

#include "model.h"

model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid, int **grid){
    model *game = (model *) malloc(sizeof(model));
    if(!game){
        perror("[MODEL] erreur allocation de la structure model.");
        exit(EXIT_FAILURE);
    }

    game->players = (position**) malloc(sizeof(position*) * nb_player);
    if(!game->players){
        perror("[MODEL] erreur allocation des positions pour les joueurs.");
        exit(EXIT_FAILURE);
    }

    game->dead= (char *) calloc(nb_player, sizeof(char));
    if(!game->dead){
        perror("[MODEL] erreur allocation des etats de vie des joueurs.");
        exit(EXIT_FAILURE);
    }

    game->n_player= nb_player;
    game->n_player_alive = nb_player;


    // game->grid = calloc(nb_lignes_grid, sizeof(int *));
    // if(!game->grid){
    //     perror("[MODEL] erreur allocation des lignes de la grille de jeu.");
    //     exit(EXIT_FAILURE);
    // }
    // for(int i = 0; i < nb_lignes_grid; i++){
    //     game->grid[i] = (int *) calloc(nb_colonnes_grid, sizeof(int));
    //     if(!game->grid[i]){
    //         perror("[MODEL] erreur allocation d'une colonne de la grille de jeu.");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    game->grid = grid;

    game->scores = (int *) calloc(nb_player, sizeof(int));
    if(!game->scores){
        perror("[MODEL] erreur allocation d'une colonne de la grille de jeu.");
        exit(EXIT_FAILURE);
    }

    return game;
}

void destroy(model *m){
    if(!m) return;

    free(m->dead);
    free(m->players);

    for(int i = 0; i < m->nb_lignes_grid; i++){
        free(m->grid[i]);
    }
    free(m->grid);

    free(m);
}

int move_player(model *m, int player, direction dir){
    if(!m){
        perror("[MODEL] deplacement joueur sur model NULL");
        exit(EXIT_FAILURE);
    }   
    if(player > m->n_player){
        perror("[MODEL] deplacement joueur d'un joueur inexistant.");
        exit(EXIT_FAILURE);
    }

    if(m->dead[player] != 0) return 0;  // Le joueur est mort, on ne le déplace pas.

    position *player_position = m->players[player];
    m->grid[player_position->y][player_position->x] = -(player+1); // Mur representé par -(joueur+1) 

    switch (dir)
    {
        case UP:
            m->players[player]->y--;
            break;
        case DOWN:
            m->players[player]->y++;
            break;
        case LEFT:
            m->players[player]->x--;
            break;
        case RIGHT:
            m->players[player]->x++;
            break;
        default:
            break;
    }

    if(player_position->y >= 0 && player_position->y < m->nb_lignes_grid
        && player_position->x >= 0 && player_position->x < m->nb_colonnes_grid
        && m->grid[player_position->y][player_position->x] == EMPTY){

        m->grid[player_position->y][player_position->x] = (player+1); // Deplace le joueur si la case ou il doit être déplacé est vide
        m->scores[player] += SCORE_INCREMENT;
    }


    return 1;
}

// AJOUTER UN TEST AVEC LA COLLISION SUR LES BORDS DE LA FENETRE IE : y >= 0 && y < nb_lignes && x >= 0 && x < nb_colonnes
int collision_player(model *m, int indexPlayer){
    if(!m){
        perror("[MODEL] collision joueur sur model NULL");
        exit(EXIT_FAILURE);
    }  
    if(indexPlayer > m->n_player){
        perror("[MODEL] collision joueur d'un joueur inexistant.");
        exit(EXIT_FAILURE);
    }

    if(!m->dead[indexPlayer]){
        position *player = m->players[indexPlayer];
        if(m->grid[player->x][player->y] != (indexPlayer+1)){
            m->dead[indexPlayer] = 1;
            m->n_player_alive--;
            return 1;
        }
    }
    return 0;
}

int est_fini(model *m){
    if(!m){
        perror("[MODEL] est_fini sur model NULL");
        exit(EXIT_FAILURE);
    }  
    return (m->n_player_alive <= 1) ? 1 : 0;
}
