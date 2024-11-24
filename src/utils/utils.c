#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int **load_map(char *path, int nb_colonnes, int nb_lignes){
    FILE *file = fopen(path, 'r');

    if(file == NULL){
        perror("[UTILS] ERREUR OUVERTURE FICHIER");
        return NULL;
    }


}

// CHARGE LA GRILLE DEPUIS LE FICHIER 
grid *load_grid_as_it_is(FILE *f){
    char buffer[1024];

}

// RETOURNE UNE GRILLE A LA TAILLE nb_lignes*nb_colonnes 
grid *upscale_grid(grid *g, int nb_colonnes, int nb_lignes){

}

// LIBERE LA GRILLE
void destroy_grid(grid *g){
    
}