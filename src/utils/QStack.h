#pragma once

#include "maillon.h"

typedef struct _stack_queue{
    maillon *start;
    maillon *end;
    int size;
} stack;

typedef stack queue;

struct _stack_queue *QS_init();

void QS_destroy(struct _stack_queue *q);

void *QS_get_first(struct _stack_queue *q);

void *QS_pop(struct _stack_queue *s);

void QS_add(struct _stack_queue *s, void *elt);

