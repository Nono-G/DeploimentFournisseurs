#include "arondialeatoire.h"
#include <time.h>
#include <string.h>

result* aa (Data* data){
    result* r = (result*) malloc(sizeof(result));
    r->open = (int*) malloc(data->facility_count * sizeof(int));
    int* current_open = (int*) malloc(data->facility_count * sizeof(int));
    double* reslp = lpsolv(data, LP_RELAX);
    r->value = INT_MAX;
    srand(time(NULL));
    int k = 0;
    int current_val;
    while(k<AA_TENTATIVES){
        int i = 0;
        while(i<data->facility_count){
            double alea = (double)rand()/(double)RAND_MAX;
            //*TRACE*/printf("A: %f\nV: %f\n",alea, reslp[i]);
            if(alea < reslp[i]){
                current_open[i] = 1;
            }else{
                current_open[i] = 0;
            }
            i++;
        }
        current_val = eval(data, current_open);
        //*TRACE*/printf("VAL %d\n",current_val);
        if(r->value > current_val){
            //*TRACE*/printf("%d] Val : %d\n",k,current_val);
            r->value = current_val;
            memcpy(r->open, current_open, data->facility_count*sizeof(int));
        }
        k++;
    }
    free(current_open);
    free(reslp);
    return r;
}
