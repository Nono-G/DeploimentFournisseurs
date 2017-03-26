#include "priority.h"

void push (heap_t* h, int priority, int type, int indice) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = (node_t*) realloc(h->nodes, h->size * sizeof(node_t));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].priority > priority) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j /= 2;
    }
    h->nodes[i].priority = priority;
    h->nodes[i].type = type;
    h->nodes[i].indice = indice;
    h->len++;
}

node_t* pop (heap_t *h) {
    int i, j, k;
    if (!h->len) return NULL;
    node_t* data = &(h->nodes[1]);
    h->nodes[1] = h->nodes[h->len];
    h->len--;
    i = 1;
    while (1) {
        k = i;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].priority < h->nodes[k].priority)
            k = j;
        if (j + 1 <= h->len && h->nodes[j + 1].priority < h->nodes[k].priority)
            k = j + 1;
        if (k == i) break;
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    h->nodes[i] = h->nodes[h->len + 1];
    return data;
}

heap_t* create_heap() {
    heap_t* r = (heap_t*) calloc(1, sizeof(heap_t));
    r->nodes = (node_t*) malloc(sizeof(node_t));
    return r;
}

void free_heap(heap_t* h) {
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
