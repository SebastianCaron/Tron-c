#include "big.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/hashmap.h"
#include "../utils/QStack.h"


int position_hash_bot(int x, int y){
    return x * 100 + y;
}

int is_in_hashs(hashmap **h, int size, int x, int y){
    if(size == 0) return -1;

    for(int i = 0; i < size; i++){
        if(hashmap_is_in(h[i], x, y) == 1){
            return i;
        }
    }

    return -1;
}

int distance_from(int x1, int y1, int x2, int y2){
    int xd = x1 - x2;
    if(xd < 0) xd = -xd;
    int yd = y1 - y2;
    if(yd < 0) yd = -yd;
    return yd + xd;
}

void bfs_from(int nb_lignes, int nb_colonnes, int **grid, hashmap *h, int x, int y){
    queue *q = QS_init();
    QS_add(q, x, y);
    hashmap_add(h, x, y);

    while(q->size > 0){
        int pos_x = QS_show_first(q)->x;
        int pos_y = QS_show_first(q)->y;
        QS_pop_first(q);

        if((pos_y > 0) && hashmap_is_in(h, pos_x, pos_y-1) == 0){
            if(grid[pos_y-1][pos_x] == EMPTY){
                hashmap_add(h, pos_x, pos_y-1);
                QS_add(q, pos_x, pos_y-1);
            }
        }
        if((pos_y < nb_lignes - 1) && hashmap_is_in(h, pos_x, pos_y+1) == 0){
            if(grid[pos_y+1][pos_x] == EMPTY){
                hashmap_add(h, pos_x, pos_y+1);
                QS_add(q, pos_x, pos_y+1);
            }
        }
        if((pos_x > 0) && hashmap_is_in(h, pos_x+1, pos_y) == 0){
            if(grid[pos_y][pos_x+1] == EMPTY){
                hashmap_add(h, pos_x+1, pos_y);
                QS_add(q, pos_x+1, pos_y);
            }
        }
        if((pos_x < nb_colonnes - 1) && hashmap_is_in(h, pos_x-1, pos_y) == 0){
            if(grid[pos_y][pos_x-1] == EMPTY){
                hashmap_add(h, pos_x-1, pos_y);
                QS_add(q, pos_x-1, pos_y);
            }
        }
    }
    QS_destroy(q);
}

direction espace_vital_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot){
    int directionBot[4][2] = {{-1,0},{0,-1},{1,0},{0,1}};
    int dir_conv[4] = {UP, LEFT, DOWN, RIGHT};
    // direction current_direction = directions[index_bot];

    hashmap *harray[3];
    int size = 0;

    position *pos_bot = players[index_bot];
    int x = 0; 
    int y = 0;
    int best = 0;
    int best_size = 1;
    int best_i[4] = {0};
    int i = 0;
    for(i = 0; i < 4; i++){
        x = pos_bot->x + directionBot[i][1];
        y = pos_bot->y + directionBot[i][0];
        if(x >= 0 && x < nb_colonnes && y >= 0 && y < nb_lignes && grid[y][x] == EMPTY){
            int is_in = is_in_hashs(harray, size, x, y);
            if(is_in == -1){
                harray[size++] = init_hashmap(-1, position_hash_bot);
                bfs_from(nb_lignes, nb_colonnes, grid, harray[size-1], x, y);
                // printf("DIR : %d, SIZE : %d\n", i, harray[size-1]->nb_elt);
                if(harray[size-1]->nb_elt > best){
                    best_size = 1;
                    best = harray[size-1]->nb_elt;
                    best_i[best_size-1] = i;
                }else if (harray[size-1]->nb_elt == best)
                {
                    best_size++;
                    best_i[best_size-1] = i;
                }
            }else{
                if(harray[is_in]->nb_elt > best){
                    best_size = 1;
                    best = harray[is_in]->nb_elt;
                    best_i[best_size-1] = i;
                }else if (harray[is_in]->nb_elt == best)
                {
                    best_size++;
                    best_i[best_size-1] = i;
                }
            }            
        }
    }

    int bd = 0;
    int bdistance = 1000000000;
    int df = 0;

    // FROM CENTER
    for(i = 0; i < best_size; i++){
        x = pos_bot->x + directionBot[best_i[i]][1];
        y = pos_bot->y + directionBot[best_i[i]][0];
        df = distance_from(x, y, nb_colonnes/2, nb_lignes/2);
        if(df < bdistance){
            bd = best_i[i];
            bdistance = df;
        }
    }

    for(int i = 0; i < size; i++){
        destroy_hashmap(harray[i]);
    }

    return dir_conv[bd];
    
}


direction immitateur_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot){
    int directionBot[4][2] = {{-1,0},{0,-1},{1,0},{0,1}};
    int dir_conv[4] = {UP, LEFT, DOWN, RIGHT};
    // direction current_direction = directions[index_bot];

    hashmap *harray[3];
    int size = 0;

    position *pos_bot = players[index_bot];
    int x = 0; 
    int y = 0;
    int best = 0;
    int best_size = 1;
    int best_i[4] = {0};
    int i = 0;
    for(i = 0; i < 4; i++){
        x = pos_bot->x + directionBot[i][1];
        y = pos_bot->y + directionBot[i][0];
        if(x >= 0 && x < nb_colonnes && y >= 0 && y < nb_lignes && grid[y][x] == EMPTY){
            int is_in = is_in_hashs(harray, size, x, y);
            if(is_in == -1){
                harray[size++] = init_hashmap(-1, position_hash_bot);
                bfs_from(nb_lignes, nb_colonnes, grid, harray[size-1], x, y);
                // printf("DIR : %d, SIZE : %d\n", i, harray[size-1]->nb_elt);
                if(harray[size-1]->nb_elt > best){
                    best_size = 1;
                    best = harray[size-1]->nb_elt;
                    best_i[best_size-1] = i;
                }else if (harray[size-1]->nb_elt == best)
                {
                    best_size++;
                    best_i[best_size-1] = i;
                }
            }else{
                if(harray[is_in]->nb_elt > best){
                    best_size = 1;
                    best = harray[is_in]->nb_elt;
                    best_i[best_size-1] = i;
                }else if (harray[is_in]->nb_elt == best)
                {
                    best_size++;
                    best_i[best_size-1] = i;
                }
            }            
        }
    }

    int bd = 0;
    int bdistance = 1000000000;
    int df = 0;

    int index_first_alive = 0;
    i = 0;
    while(directions[i] == NODIRECTION || i == index_bot){
        i++;
    }
    index_first_alive = i;

    // FROM FIRST PLAYER ALIVE
    for(i = 0; i < best_size; i++){
        x = pos_bot->x + directionBot[best_i[i]][1];
        y = pos_bot->y + directionBot[best_i[i]][0];
        df = distance_from(x, y, players[index_first_alive]->x, players[index_first_alive]->y);
        if(df < bdistance){
            bd = best_i[i];
            bdistance = df;
        }
    }

    for(int i = 0; i < size; i++){
        destroy_hashmap(harray[i]);
    }

    return dir_conv[bd];
    
}