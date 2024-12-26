#pragma once

typedef struct _maillon{
    int x;
    int y;
    struct _maillon *next;
    struct _maillon *prev;
} maillon;

maillon *init_maillon(int x, int y);
void destroy_maillons(maillon *m);

maillon *maillon_remove(maillon *m, int x, int y);
int maillon_is_in(maillon *m, int x, int y);