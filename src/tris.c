#include "tris.h"

void permute(int i, int j, int* array) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void placer_pivot(Data* data, int fournisseur_i, int g, int d, int* array, int *p) {
    int i;
    int pivot = data->connection[fournisseur_i][g];
    *p = g;
    for (i = g + 1; i <= d; i++) {
        if (data->connection[fournisseur_i][i] < pivot) {
            (*p)++;
            if (i != *p) permute(i, *p, array);
        }
    }
    permute(*p, g, array);
}

void tri_rapide_back(Data* data, int fournisseur_i, int g, int d, int* array) {
    int p;
    if (g < d) {
        placer_pivot(data, fournisseur_i, g, d, array, &p);
        tri_rapide_back(data, fournisseur_i, g, p - 1, array);
        tri_rapide_back(data, fournisseur_i, p + 1, d, array);
    }
}

void tri_rapide(Data* data, int fournisseur_i, int* array, int arraysize) {
    int debut = 0;
    tri_rapide_back(data, fournisseur_i, debut, arraysize - 1, array);
}

void tri_lent(Data* data, int fournisseur_i, int* tab, int len){
    int k = 0;
    while(k < len){
        int iMin = k;
        int i = k+1;
        while(i < len){
            if(data->connection[fournisseur_i][tab[i]]<data->connection[fournisseur_i][tab[iMin]]){
                iMin = i;
            }
            i++;
        }
        int piv = tab[k];
        tab[k] = tab[iMin];
        tab[iMin] = piv;
        k++;
    }
}
