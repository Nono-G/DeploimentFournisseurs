#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef struct {
    int client_count;
    int facility_count;
    int* opening_cost;
    int** connection;
} Data;

typedef struct result {
    int* open;
    int value;
} result;

typedef struct beta_return {
    int* y_clients;
    int y_size;
    double value;
} beta_return;

Data* load_instance2(char* filename);
void free_data(Data* data);
void free_beta_return(beta_return* p);
void free_result(result* r);

void display_int(int* o, int size);

#endif
