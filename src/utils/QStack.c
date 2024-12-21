#include <stdlib.h>
#include <stdio.h>

#include "QStack.h"

struct _stack_queue *QS_init(){
    struct _stack_queue *res = calloc(1, sizeof(struct _stack_queue));
    if(!res){
        perror("[QStack] erreur allocation\n");
        return NULL;
    }
    return res;
}

void QS_destroy(struct _stack_queue *q){
    destroy_maillons(q->start);
    free(q);
}

void *QS_pop_first(struct _stack_queue *q){
    if(q == NULL) return NULL;
    if(q->start == NULL) return NULL;
    void *val = q->start->value;
    maillon *tmp = q->start;
    q->start = q->start->next;
    if(q->start == NULL) q->end = NULL;
    free(tmp);
    q->size -= 1;
    return val;
}

void *QS_pop(struct _stack_queue *s){
    if(s == NULL) return NULL;
    if(s->end == NULL) return NULL;
    void *val = s->end->value;
    maillon *tmp = s->end;
    s->end = s->end->prev;
    if(s->end == NULL) s->start = NULL;
    free(tmp);
    s->size -= 1;
    return val;
}

void QS_add(struct _stack_queue *s, void *elt){
    if(s == NULL) return;
    if(s->start == NULL || s->end == NULL){
        maillon *m = init_maillon(elt);
        s->start = m;
        s->end = m;
        s->size = 1;
        return;
    }

    s->end->next = init_maillon(elt);
    s->end->next->prev = s->end;
    s->size += 1;
    return;
}
void QS_add_first(struct _stack_queue *s, void *elt){
    if(s == NULL) return;
    if(s->start == NULL || s->end == NULL){
        maillon *m = init_maillon(elt);
        s->start = m;
        s->end = m;
        s->size = 1;
        return;
    }
    maillon *tmp = init_maillon(elt);
    tmp->next = s->start;
    s->start->prev = tmp;
    s->start = tmp;
    s->size += 1;
    return;
}