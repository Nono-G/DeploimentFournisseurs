#ifndef __PRIORITY_H__
#define __PRIORITY_H__

#include "util.h"

typedef struct {
    double priority;
    int type;
    int indice;
} node_t;

typedef struct {
    node_t** nodes;
    int len;
    int size;
} heap_t;

void push (heap_t *h, double priority, int type, int indice);
node_t* pop (heap_t *h);
int is_empty(heap_t* h);
heap_t* create_heap(int size);
void display_heap(heap_t* h);
void free_heap(heap_t* h);

#endif
