#pragma once

#include "maillon.h"

#define DEFAULT_SIZE_HASH_MAP 1024

typedef struct hashmap{
    maillon **tab;
    int size;
    int (*hash)(int x, int y);
} hashmap;

int hashmap_is_in(hashmap *h, int x, int y);
int hashmap_add(hashmap *h, int x, int y);
void hashmap_remove(hashmap *h, int x, int y);

hashmap *init_hashmap(int size, int (*hash)(int x, int y));
void destroy_hashmap(hashmap *h);

