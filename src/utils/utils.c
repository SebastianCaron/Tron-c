#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "../model/model.h"

grid *load_map(char *path, int nb_lignes, int nb_colonnes){
    int nbfc = 0;
    int nbfl = 0;
    count_nb_lignes_colonnes(path, &nbfl, &nbfc);

    grid *g = load_grid_as_it_is(path, nbfl, nbfc);
    if(!g){
        return NULL;
    }
    // display_grid(g);

    grid *ng = upscale_grid(g, nb_lignes, nb_colonnes);
    destroy_grid(g);
    return ng;
}

// CHARGE LA GRILLE DEPUIS LE FICHIER 
grid *load_grid_as_it_is(char *path, int nb_lignes, int nb_colonnes){
    FILE *f = fopen(path, "r");
    if(f == NULL){
        perror("[UTILS] ERREUR OUVERTURE FICHIER");
        return NULL;
    }

    grid *g = calloc(1, sizeof(grid));
    if(g == NULL){
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
        // if(!fscanf(f, "%s", buffer)){
        //     perror("[UTILS] ERREUR LECTURE FICHIER");
        //     destroy_grid(g);
        //     fclose(f);
        //     return NULL;
        // }
        fgets(buffer, BUFFER_SIZE, f);
        len = strlen(buffer)-1;
        // printf("%s", buffer);

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
    FILE *f = fopen(path, "r");
    if(f == NULL){
        perror("[UTILS] ERREUR OUVERTURE FICHIER");
        return;
    }

    char buffer[BUFFER_SIZE];
    int len = 0;

    while(!feof(f)){
        fgets(buffer, BUFFER_SIZE, f);
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

    ng->grid = allocate_grid(ng->nb_lignes, ng->nb_colonnes);

    float scale_x =  (float) nb_colonnes / (float) g->nb_colonnes;
    float scale_y = (float) nb_lignes / (float) g->nb_lignes;
    
    if(scale_x == 0) scale_x = 1;
    if(scale_y == 0) scale_y = 1;


    for(int i = 0; i < nb_lignes; i++){
        for(int j = 0; j < nb_colonnes; j++){
            int i_y = i/scale_y;
            int i_x = j/scale_x;

            ng->grid[i][j] = g->grid[i_y][i_x];
        }
    }

    return ng;
}

int **allocate_grid(int nb_lignes, int nb_colonnes){
    int **res = calloc(nb_lignes, sizeof(int *));
    if(res == NULL){
        perror("[UTILS] ERREUR ALLOCATION GRID LINES : ");
        printf("%d x %d\n", nb_lignes, nb_colonnes);
        return NULL;
    }
    for(int i = 0; i < nb_lignes; i++){
        res[i] = calloc(nb_colonnes, sizeof(int));
        if(res[i] == NULL){
            perror("[UTILS] ERREUR ALLOCATION GRID LINE");
            printf("%d x %d\n", nb_lignes, nb_colonnes);
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


int string_equal(char *a, char *b){
    if(a == NULL || b == NULL) return 0;
    int i = 0;
    while(1){
        if(a[i] == '\0' && b[i] == '\0') return 1;
        if(a[i] == '\0' || b[i] == '\0') return 0;
        if(a[i] != b[i]) return 0;

        i += 1;
    }
    return 0;
}


void display_grid(grid *g){
    for(int i = 0; i < g->nb_lignes; i++){
        printf("[");
        for(int j = 0; j < g->nb_colonnes-1; j++){
            printf("%d, ", g->grid[i][j]);
        }
        if(g->nb_colonnes >= 1)printf("%d", g->grid[i][g->nb_colonnes-1]);
        printf("]\n");
    }
}
void display_grid_i(int **tab, int nb_lignes, int nb_colonnes){
    for(int i = 0; i < nb_lignes; i++){
        printf("[");
        for(int j = 0; j < nb_colonnes-1; j++){
            printf("%d, ", tab[i][j]);
        }
        if(nb_colonnes >= 1)printf("%d", tab[i][nb_colonnes-1]);
        printf("]\n");
    }
}