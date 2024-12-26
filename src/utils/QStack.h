#pragma once

#include "maillon.h"

typedef struct _stack_queue{
    maillon *start;
    maillon *end;
    int size;
} stack;

typedef stack queue;

struct _stack_queue *QS_init();


maillon *QS_show_first(struct _stack_queue *q);
maillon *QS_show_last(struct _stack_queue *q);

void QS_destroy(struct _stack_queue *q);

void QS_pop_first(struct _stack_queue *q);

void QS_pop(struct _stack_queue *s);

void QS_add(struct _stack_queue *s, int x, int y);

void QS_add_first(struct _stack_queue *s, int x, int y);

