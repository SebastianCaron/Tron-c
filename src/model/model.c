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

    // AJOUTER MUR SUR LA GRILLE

    switch (dir)
    {
        case UP:
            m->players[player].y--;
            break;
        case DOWN:
            m->players[player].y++;
            break;
        case LEFT:
            m->players[player].x--;
            break;
        case RIGHT:
            m->players[player].x++;
            break;
        default:
            break;
    }

    return 1;
}

int collision_player(model *m, int player){
    if(!m){
        perror("[MODEL] collision joueur sur model NULL");
        exit(EXIT_FAILURE);
    }  
    if(player > m->n_player){
        perror("[MODEL] collision joueur d'un joueur inexistant.");
        exit(EXIT_FAILURE);
    }

    // A ECRIRE
}

int est_fini(model *m){
    if(!m){
        perror("[MODEL] est_fini sur model NULL");
        exit(EXIT_FAILURE);
    }  

    // A ECRIRE
    // IE : si sum(dead) <= 1, Il ne reste qu'un ou aucun joueur en vie.
}