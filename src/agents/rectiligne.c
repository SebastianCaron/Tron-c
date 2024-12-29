#include "rectiligne.h"

#include <stdio.h>
#include <stdlib.h>

direction rectiligne_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot){
    int rectiligne_XY[4][2]={{0,-1},{0,1},{-1,0},{1,0}};
    direction rectiligne_converted[4] = {UP, DOWN, LEFT, RIGHT};
    int cptPosPossible = 0;
    position *bot_position = players[index_bot];
    int posPossible[4] = {NODIRECTION};

    direction current_direction = directions[index_bot];
    int x = 0; 
    int y = 0;
    if(current_direction != NODIRECTION){
        int i = 0;
        switch (current_direction)
        {
            case UP:
                i = 0;
                break;
            case DOWN:
                i = 1;
                break;
            case LEFT:
                i = 2;
                break;
            case RIGHT:
                i = 3;
                break;
            default:
                break;
        }
        x = (bot_position->x)+rectiligne_XY[i][0];
        y = (bot_position->y)+rectiligne_XY[i][1];
        // printf("direction : %d\n", i);
        // printf("x : %d; y : %d, value : %d\n", bot_position->x, bot_position->y, grid[y][x]);
        // printf("x : %d; y : %d, value : %d\n", x, y, grid[y][x]);
        if(x >= 0 && x <= nb_lignes-1 && y >= 0 && y <= nb_colonnes-1 && grid[y][x] == EMPTY) return NODIRECTION;
    }


    for(int k = 0; k<4; k++){
        x = (bot_position->x)+rectiligne_XY[k][0];
        y = (bot_position->y)+rectiligne_XY[k][1];
        // printf("x : %d; y : %d\n", x, y);
        if (x >= 0 && x <= nb_lignes-1 && y >= 0 && y <= nb_colonnes-1 && grid[y][x] == EMPTY) {
            if(directions[index_bot] != NODIRECTION || (rectiligne_converted[k]&1) != (directions[index_bot]&1)){ // VERIFIE DIRECTIONS OPPOSEE
                posPossible[cptPosPossible++] = k;
            } 
        }
        
    }

    return (cptPosPossible == 0) ? NODIRECTION : rectiligne_converted[posPossible[rand() % cptPosPossible]];
}

direction random_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot){
    int rectiligne_XY[4][2]={{0,-1},{0,1},{-1,0},{1,0}};
    direction rectiligne_converted[4] = {UP, DOWN, LEFT, RIGHT};
    int cptPosPossible = 0;
    position *bot_position = players[index_bot];
    int posPossible[4] = {NODIRECTION};

    direction current_direction = directions[index_bot];
    int x = 0; 
    int y = 0;
    if(current_direction != NODIRECTION){
        int i = 0;
        switch (current_direction)
        {
            case UP:
                i = 0;
                break;
            case DOWN:
                i = 1;
                break;
            case LEFT:
                i = 2;
                break;
            case RIGHT:
                i = 3;
                break;
            default:
                break;
        }
        x = (bot_position->x)+rectiligne_XY[i][0];
        y = (bot_position->y)+rectiligne_XY[i][1];
        // printf("direction : %d\n", i);
        // printf("x : %d; y : %d, value : %d\n", bot_position->x, bot_position->y, grid[y][x]);
        // printf("x : %d; y : %d, value : %d\n", x, y, grid[y][x]);
        // if(x >= 0 && x <= nb_lignes-1 && y >= 0 && y <= nb_colonnes-1 && grid[y][x] == EMPTY) return NODIRECTION;
    }


    for(int k = 0; k<4; k++){
        x = (bot_position->x)+rectiligne_XY[k][0];
        y = (bot_position->y)+rectiligne_XY[k][1];
        // printf("x : %d; y : %d\n", x, y);
        if (x >= 0 && x <= nb_lignes-1 && y >= 0 && y <= nb_colonnes-1 && grid[y][x] == EMPTY) {
            if(directions[index_bot] != NODIRECTION || (rectiligne_converted[k]&1) != (directions[index_bot]&1)){ // VERIFIE DIRECTIONS OPPOSEE
                posPossible[cptPosPossible++] = k;
            } 
        }
        
    }

    return (cptPosPossible == 0) ? NODIRECTION : rectiligne_converted[posPossible[rand() % cptPosPossible]];
}