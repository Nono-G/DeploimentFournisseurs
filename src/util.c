#include "util.h"

#define BUZZSIZE 1000

Data* load_instance2(char* name) {
    FILE* f = fopen(name, "r");
    if (f == NULL) {
        perror("fopen");
        fprintf(stderr, "Impossible d'ouvrir le fichier %s (existe-t-il ?)\n", name);
        exit(1);
    }
    int t1, f_count, c_count, o_cost, c_cost;
    char filename[BUZZSIZE];
    char t2[BUZZSIZE];
    Data* d = (Data*) malloc (sizeof(Data));
    fscanf(f, "%s %s", t2, filename);
    fscanf(f, "%d %d %d", &f_count, &c_count, &t1);
    d->facility_count = f_count;
    d->client_count = c_count;
    //printf("F : %d\nC : %d\n", d->facility_count, d->client_count);
    d->opening_cost = (int*) malloc (d->facility_count * sizeof(int));
    d->connection = (int**) malloc (d->client_count * sizeof(int*));
    for (int i = 0; i < d->facility_count; ++i) {
        fscanf(f, "%d %d", &t1, &o_cost);
        d->opening_cost[i] = o_cost;
        d->connection[i] = (int*) malloc (d->client_count * sizeof(int));
        for (int j = 0; j < d->client_count; ++j) {
            fscanf(f, "%d", &c_cost);
            d->connection[i][j] = c_cost;
        }
    }
    fclose(f);
    return d;
}

void free_data(Data* data){
    for (int fac = 0; fac < data->facility_count; ++fac){
        //printf("FAC %d\n", fac);
        free(data->connection[fac]);
    }
    free(data->connection);
    free(data->opening_cost);
    free(data);

    return;
}

void free_beta_return(beta_return* p){
    free(p->y_clients);
    free(p);
}

void free_result(result* r){
    free(r->open);
    free(r);
}

void display_int(int* o, int size){
    for (int i = 0; i < size; ++i)
        printf("[%d]%d \n", i, o[i]);
}
