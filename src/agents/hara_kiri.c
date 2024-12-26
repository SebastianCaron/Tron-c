#include "hara_kiri.h"

#include <stdio.h>
#include <stdlib.h>

direction hara_kiri_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions){
    // Faire une liste des directions possible du bot
    // Regarder position suivante du joueur en fonction de sa direction
    // Prendre la direction du bot qui se rapproche le plus du joueur 

    int directionBotTest[4][2] = {{-1,0},{0,-1},{1,0},{0,1}};
    direction current_direction = directions[1];

    int xJ = players[0]->x;
    int yJ = players[0]->y;
    if(current_direction != NODIRECTION){
        xJ = players[0]->x + directionBotTest[current_direction-1][1];
        yJ = players[0]->y + directionBotTest[current_direction-1][0];
    }

    int distancePlusCourte = 100000;
    int indexDistancePlusCourte = 0;

    for (int i = 0; i < 4; i++) {
        int xBot = players[1]->x + directionBotTest[i][1];
        int yBot = players[1]->y + directionBotTest[i][0];
        if (grid[yBot][xBot] == EMPTY) {
            int distance = ((xBot - xJ) * (xBot - xJ)) + ((yBot - yJ) * (yBot - yJ));
            if (distancePlusCourte > distance) {
                indexDistancePlusCourte = i+1;
                distancePlusCourte = distance;
            }
        }
    }

    return (distancePlusCourte == 100000) ? RIGHT : indexDistancePlusCourte;
}