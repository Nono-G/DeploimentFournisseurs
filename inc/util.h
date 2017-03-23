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
void display_doubles(double* o, int size);

//LINEAR PROGRAMMING
#define TAILLE_MATRICE_GLPK 1000001
#define TAILLE_NOM 100
#define LP_RELAX 1
#define LP_INTEGERS 0
double* lpsolv(Data* data, int relax);

#endif
