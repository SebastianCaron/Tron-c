#pragma once

#define DEFAULT_SIZE_HASH_MAP 1024

typedef struct _maillon{
    void *value;
    struct _maillon *next;
} maillon;

typedef struct hashmap{
    maillon **tab;
    int size;

    int (*equal)(void *v1, void *v2);
    int (*hash)(void *v);
} hashmap;

int hashmap_is_in(hashmap *h, void *val);
int hashmap_add(hashmap *h, void *val);
void hashmap_remove(hashmap *h, void *val);

hashmap *init_hashmap(int size, int (*equal)(void *v1, void *v2), int (*hash)(void *v));
void destroy_hashmap(hashmap *h);

maillon *init_maillon(void *value);
void destroy_maillons(maillon *m);

maillon *maillon_remove(maillon *m, void *value, int (*equal)(void *v1, void *v2));
int maillon_is_in(maillon *m, void *value, int (*equal)(void *v1, void *v2));