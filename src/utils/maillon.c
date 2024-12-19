#include <stdio.h>
#include <stdlib.h>

#include "maillon.h"

maillon *init_maillon(void *value){
    maillon *res = calloc(1, sizeof(maillon));
    if(res == NULL){
        perror("[HASHMAP] erreur init maillon");
        return NULL;
    }
    res->value = value;
    return res;
}

void destroy_maillons(maillon *m){
    if(m == NULL) return;
    destroy_maillons(m->next);
    free(m->value);
    free(m);
}

maillon *maillon_remove(maillon *m, void *value, int (*equal)(void *v1, void *v2)){
    if(m == NULL) return NULL;
    maillon *current = m;
    maillon *prec = NULL;
    while(current != NULL){
        if(equal(current->value, value) == 1){
            maillon *next = current->next;
            free(current->value);
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
int maillon_is_in(maillon *m, void *value, int (*equal)(void *v1, void *v2)){
    if(m == NULL) return 0;
    maillon *current = m;
    while(current != NULL){
        if(equal(current->value, value) == 1){
            return 1;
        }
        current = current->next;
    }
    return 0;
}