#include "random.h"

#include <stdio.h>
#include <stdlib.h>

int aleaEntreBornes(int min, int max){
    return rand() % (max - min + 1) + min;
}

int XY[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
direction converted[4] = {DOWN, UP, RIGHT, LEFT};

direction random_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions){
    int cptPosPossible = 0;
    position *bot_position = players[1];
    int posPossible[4] = {NODIRECTION};

    for(int k = 0; k<4; k++){
        int x = (bot_position->x)+XY[k][0];
        int y = (bot_position->y)+XY[k][1];
        // printf("x : %d; y : %d\n", x, y);
        if (x >= 0 && x <= nb_lignes-1 && y >= 0 && y <= nb_colonnes-1 && grid[y][x] == EMPTY) {
            if(directions[1] != NODIRECTION || (converted[k]&1) != (directions[1]&1)){ // VERIFIE DIRECTIONS OPPOSEE
                posPossible[cptPosPossible++] = k;
            } 
        }
        
    }

    return (cptPosPossible == 0) ? NODIRECTION : converted[posPossible[aleaEntreBornes(0, cptPosPossible-1)]];
}
