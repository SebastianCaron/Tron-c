#pragma once

#define PORT 7604

typedef struct{
    int nb_colonnes;
    int nb_lignes;
    int **grid;
} grid;

typedef enum {NBJOUEUR = 1, GRID = 2, POSITIONS = 3, SCORES = 4, MOUVEMENT = 5, ISOVER = 6, START = 7, NEEDGRID = 8, ENDPACKET = 9} f_packet;

int **load_map(char *path, int nb_colonnes, int nb_lignes);