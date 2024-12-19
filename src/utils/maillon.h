#pragma once

typedef struct _maillon{
    void *value;
    struct _maillon *next;
    struct _maillon *prev;
} maillon;

maillon *init_maillon(void *value);
void destroy_maillons(maillon *m);

maillon *maillon_remove(maillon *m, void *value, int (*equal)(void *v1, void *v2));
int maillon_is_in(maillon *m, void *value, int (*equal)(void *v1, void *v2));