#include <stdio.h>
#include <stdlib.h>

#include "maillon.h"

maillon *init_maillon(int x, int y){
    maillon *res = calloc(1, sizeof(maillon));
    if(res == NULL){
        perror("[HASHMAP] erreur init maillon");
        return NULL;
    }
    res->x = x;
    res->y = y;
    return res;
}

void destroy_maillons(maillon *m){
    if(m == NULL) return;
    destroy_maillons(m->next);
    free(m);
}

maillon *maillon_remove(maillon *m, int x, int y){
    if(m == NULL) return NULL;
    maillon *current = m;
    maillon *prec = NULL;
    while(current != NULL){
        if(current->x == x && current->y == y){
            maillon *next = current->next;
            free(current);
            if(prec == NULL) return next;
            prec->next = next;
            return m;
        }
        prec = current;
        current = current->next;
    }
    return m;
}
int maillon_is_in(maillon *m, int x, int y){
    if(m == NULL) return 0;
    maillon *current = m;
    while(current != NULL){
        if(current->x == x && current->y == y){
            return 1;
        }
        current = current->next;
    }
    return 0;
}