#include "priority.h"

void push (heap_t* h, double priority, int type, int indice) {
    h->nodes[h->len] = (node_t*) malloc(sizeof(node_t));
    h->nodes[h->len]->priority = priority;
    h->nodes[h->len]->type = type;
    h->nodes[h->len]->indice = indice;
    h->len = h->len + 1;
}

node_t* pop (heap_t *h) {
    if (h->len <= 0) return NULL;
    int min = h->nodes[0]->priority;
    int i_min = 0, i = 1;
    while(i < h->len) {
        if (min > h->nodes[i]->priority) {
            min = h->nodes[i]->priority;
            i_min = i;
        }
        i++;
    }
    node_t* r = h->nodes[i_min];
    i = i_min;
    while(i < h->len -1) {
        h->nodes[i] = h->nodes[i + 1];
        i++;
    }
    h->len = h->len - 1;
    return r;
}

int is_empty(heap_t* h) {
    return h->len == 0;
}

void display_heap(heap_t* h) {
    for (int i = 0; i < h->len; ++i) {
        printf("[%d]p:%f t:%d i:%d\n", i, h->nodes[i]->priority, h->nodes[i]->type, h->nodes[i]->indice);
    }
}

heap_t* create_heap(int size) {
    heap_t* r = (heap_t*) malloc(sizeof(heap_t));
    r->len = 0;
    r->nodes = (node_t**) malloc(size * sizeof(node_t*));
    return r;
}

void free_heap(heap_t* h) {
    for (int i = 0; i < h->len; ++i){
        free(h->nodes[i]);
    }
    free(h->nodes);
    free(h);
}

// int main () {
//     heap_t *h = (heap_t *)calloc(1, sizeof (heap_t));
//     push(h, 3, "Clear drains");
//     push(h, 4, "Feed cat");
//     push(h, 5, "Make tea");
//     push(h, 1, "Solve RC tasks");
//     push(h, 2, "Tax return");
//     int i;
//     for (i = 0; i < 5; i++) {
//         printf("%s\n", pop(h));
//     }
//     return 0;
// }
