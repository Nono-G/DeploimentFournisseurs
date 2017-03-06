#ifndef __PRIORITY_H__
#define __PRIORITY_H__

#include "util.h"

typedef struct {
    int priority;
    int type;
    int indice;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

void push (heap_t *h, int priority, int type, int indice);
node_t* pop (heap_t *h);

#endif
