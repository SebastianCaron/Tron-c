#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

int hashmap_is_in(hashmap *h, void *val){
    if(h == NULL) return 0;

    int hashv = h->hash(val);
    while(hashv < 0) hashv += h->size;
    hashv %= h->size;

    return maillon_is_in(h->tab[hashv], val, h->equal);
}
int hashmap_add(hashmap *h, void *val){
    if(h == NULL) return 0;

    int hashv = h->hash(val);
    while(hashv < 0) hashv += h->size;
    hashv %= h->size;

    maillon *nm = init_maillon(val);
    if(nm == NULL) return 0;
    nm->next = h->tab[hashv];

    h->tab[hashv] = nm;

    return 1;
}

void hashmap_remove(hashmap *h, void *val){
    if(h == NULL) return;

    int hashv = h->hash(val);
    while(hashv < 0) hashv += h->size;
    hashv %= h->size;

    h->tab[hashv] = maillon_remove(h->tab[hashv], val, h->equal);
}

hashmap *init_hashmap(int size, int (*equal)(void *v1, void *v2), int (*hash)(void *v)){
    hashmap *res = calloc(1, sizeof(hashmap));
    if(res == NULL){
        perror("[HASHMAP] erreur init hashmap");
        return NULL;
    }
    if(size <= 0) size = DEFAULT_SIZE_HASH_MAP;
    res->size = size;

    res->tab = calloc(res->size, sizeof(maillon *));
    if(res->tab == NULL){
        perror("[HASHMAP] erreur init hashmap tab");
        free(res);
        return NULL;
    }

    res->hash = hash;
    res->equal = equal;

    return res;
}

void destroy_hashmap(hashmap *h){
    if(h == NULL) return;
    for(int i = 0; i < h->size; i++){
        destroy_maillons(h->tab[i]);
    }
    free(h->tab);
    free(h);
}

