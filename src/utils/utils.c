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

    g->grid = calloc(g->nb_lignes, sizeof(int *));
    if(g->grid == NULL){
        perror("[UTILS] ERREUR ALLOCATION GRID LINES");
        destroy_grid(g);
        fclose(f);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    int len = 0;
    int *line;
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
        line = malloc(sizeof(int)*g->nb_colonnes);
        if(line == NULL){
            perror("[UTILS] ERREUR ALLOCATION GRID LINE");
            destroy_grid(g);
            fclose(f);
            return NULL;
        }

        for(i = 0; i < len; i++){
            switch (buffer[i])
            {
            case '#':
                line[i] = WALL;
                break;
            case ' ':
                line[i] = EMPTY;
                break;
            default:
                break;
            }
        }
        for(i = len; i < g->nb_colonnes; i++){
            line[i] = WALL;
        }
        g->grid[k++] = line;
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