#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "../model/model.h"

int **load_map(char *path, int nb_lignes, int nb_colonnes){
    int nbfc = 0;
    int nbfl = 0;

    count_nb_lignes_colonnes(path, &nbfl, &nbfc);
    grid *g = load_grid_as_it_is(path, nbfl, nbfc);
    if(!g){
        return NULL;
    }

    grid *ng = upscale_grid(g, nb_lignes, nb_colonnes);
    destroy_grid(g);
    return ng;
}

// CHARGE LA GRILLE DEPUIS LE FICHIER 
grid *load_grid_as_it_is(char *path, int nb_lignes, int nb_colonnes){
    FILE *f = fopen(path, 'r');
    if(f == NULL){
        perror("[UTILS] ERREUR OUVERTURE FICHIER");
        return NULL;
    }

    grid *g = calloc(1, sizeof(grid));
    if(!g){
        perror("[UTILS] ERREUR ALLOCATION GRID");
        fclose(f);
        return NULL;
    }
    g->nb_colonnes = nb_colonnes;
    g->nb_lignes = nb_lignes;

    g->grid = allocate_grid(nb_lignes, nb_colonnes);
    if(g->grid == NULL){
        destroy_grid(g);
        fclose(f);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    int len = 0;
    int i = 0;
    int k = 0;
    while(k < g->nb_lignes){
        if(!fscanf(f, "%s", &buffer)){
            perror("[UTILS] ERREUR LECTURE FICHIER");
            destroy_grid(g);
            fclose(f);
            return NULL;
        }
        len = strlen(buffer);

        for(i = 0; i < len; i++){
            switch (buffer[i])
            {
            case '#':
                g->grid[k][i] = WALL;
                break;
            case ' ':
                g->grid[k][i] = EMPTY;
                break;
            default:
                break;
            }
        }
        for(i = len; i < g->nb_colonnes; i++){
            g->grid[k][i] = WALL;
        }
        k++;
    }

    fclose(f);
    return g;
}

void count_nb_lignes_colonnes(char *path, int *nb_lignes, int *nb_colonnes){
    if(!nb_lignes || !nb_colonnes){
        return;
    }
    FILE *f = fopen(path, 'r');
    if(f == NULL){
        perror("[UTILS] ERREUR OUVERTURE FICHIER");
        return;
    }

    char buffer[BUFFER_SIZE];
    int len = 0;

    while(fscanf(f, "%s", &buffer)){
        len = strlen(buffer);
        (*nb_colonnes) = (*nb_colonnes) > len ? (*nb_colonnes) : len;
        (*nb_lignes)++;
    }

    fclose(f);
}

// RETOURNE UNE GRILLE A LA TAILLE nb_lignes*nb_colonnes 
grid *upscale_grid(grid *g, int nb_lignes, int nb_colonnes){
    grid *ng = calloc(1, sizeof(grid));
    if(!ng){
        perror("[UTILS] ERREUR ALLOCATION GRID");
        return NULL;
    }
    ng->nb_colonnes = nb_colonnes;
    ng->nb_lignes = nb_lignes;

    int scale_x =  nb_colonnes / g->nb_colonnes;
    int scale_y = nb_lignes / g->nb_lignes;
}

int **allocate_grid(int nb_lignes, int nb_colonnes){
    int **res = calloc(nb_lignes, sizeof(int *));
    if(res == NULL){
        perror("[UTILS] ERREUR ALLOCATION GRID LINES");
        return NULL;
    }
    for(int i = 0; i < nb_lignes; i++){
        res[i] = calloc(nb_colonnes, sizeof(int));
        if(res[i] == NULL){
            perror("[UTILS] ERREUR ALLOCATION GRID LINE");
            return NULL;
        }
    }
    return res;
}

// LIBERE LA GRILLE
void destroy_grid(grid *g){
    if(!g) return;

    for(int i = 0; i < g->nb_lignes; i++){
        free(g->grid[i]);
    }
    free(g->grid);

    free(g);
}