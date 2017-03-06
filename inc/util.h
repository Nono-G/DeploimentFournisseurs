#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <limits.h>

#define BUZZSIZE 1000

typedef struct result {
    int* open;
    int value;
} result;

typedef struct beta_return {
    int* y_clients;
    int y_size;
    double value;
} beta_return;

typedef struct {
    int client_count;
    int facility_count;
    int* opening_cost;
    int** connection;
} Data;

Data* load_instance(char* filename);
void free_data(Data* data);
void free_beta_return(beta_return* p);

#endif
