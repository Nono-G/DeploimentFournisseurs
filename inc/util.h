#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define BUZZSIZE 1000

//PARSE
typedef struct {
    int client_count;
    int facility_count;
    int* opening_cost;
    int** connection;
} Data;

Data* load_instance(char* filename);
void free_data(Data* data);

//
typedef struct result {
    int* open;
    int value;
} result;

typedef struct beta_return {
    int* y_clients;
    int y_size;
    double value;
} beta_return;

void display_doubles(double* o, int size);
void free_beta_return(beta_return* p);
void free_result(result* r);
int eval (Data* data, int* ouverts);
result* glouton1 (Data* data);
result* glouton2(Data* data);
beta_return* beta (int fournisseur_i, Data* data, int* clients_connectes, int* fournisseur_ouverts);

//LINEAR PROGRAMMING
#define TAILLE_MATRICE_GLPK 1000001
#define TAILLE_NOM 100
#define LP_RELAX 1
#define LP_INTEGERS 0
#define AA_TENTATIVES 1000
double* lpsolv(Data* data, int relax);
result* lp(Data* data);
result* aa(Data* data);

#endif
