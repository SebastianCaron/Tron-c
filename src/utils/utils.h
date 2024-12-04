#pragma once

#define PORT 7604
#define BUFFER_SIZE 1024

typedef struct{
    int nb_colonnes;
    int nb_lignes;
    int **grid;
} grid;

typedef enum {NBJOUEUR = 1, GRID = 2, POSITIONS = 3, SCORES = 4, MOUVEMENT = 5, ISOVER = 6, START = 7, NEEDGRID = 8, ENDPACKET = 9} f_packet;

grid *load_map(char *path, int nb_colonnes, int nb_lignes);
grid *load_grid_as_it_is(char *path, int nb_lignes, int nb_colonnes);
void count_nb_lignes_colonnes(char *path, int *nb_lignes, int *nb_colonnes);
grid *upscale_grid(grid *g, int nb_lignes, int nb_colonnes);
int **allocate_grid(int nb_lignes, int nb_colonnes);
void destroy_grid(grid *g);

int string_equal(char *a, char *b);

void display_grid(grid *g);