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

maillon *QS_show_first(struct _stack_queue *q){
    if(q == NULL) return NULL;
    if(q->start == NULL) return NULL;
    return q->start;
}

maillon *QS_show_last(struct _stack_queue *q){
    if(q == NULL) return NULL;
    if(q->end == NULL) return NULL;
    return q->end;
}

void QS_pop_first(struct _stack_queue *q){
    if(q == NULL) return;
    if(q->start == NULL) return;
    maillon *tmp = q->start;
    q->start = q->start->next;
    if(q->start == NULL) q->end = NULL;
    free(tmp);
    q->size -= 1;
    return;
}

void QS_pop(struct _stack_queue *s){
    if(s == NULL) return;
    if(s->end == NULL) return;
    maillon *tmp = s->end;
    s->end = s->end->prev;
    if(s->end == NULL) s->start = NULL;
    free(tmp);
    s->size -= 1;
    return;
}

void QS_add(struct _stack_queue *s, int x, int y){
    if(s == NULL) return;
    if(s->start == NULL || s->end == NULL){
        maillon *m = init_maillon(x, y);
        if(m == NULL){
            perror("[QS] erreur ajout\n");
            return;
        }
        s->start = m;
        s->end = m;
        s->size = 1;
        return;
    }
    s->end->next = init_maillon(x, y);
    if(s->end->next == NULL){
        perror("[QS] erreur ajout\n");
        return;
    }
    s->end->next->prev = s->end;
    s->end = s->end->next;
    s->size += 1;
    return;
}
void QS_add_first(struct _stack_queue *s, int x, int y){
    if(s == NULL) return;
    if(s->start == NULL || s->end == NULL){
        maillon *m = init_maillon(x, y);
        if(m == NULL){
            perror("[QS] erreur ajout\n");
            return;
        }
        s->start = m;
        s->end = m;
        s->size = 1;
        return;
    }
    maillon *tmp = init_maillon(x, y);
    if(tmp == NULL){
        perror("[QS] erreur ajout\n");
        return;
    }
    tmp->next = s->start;
    s->start->prev = tmp;
    s->start = tmp;
    s->size += 1;
    return;
}