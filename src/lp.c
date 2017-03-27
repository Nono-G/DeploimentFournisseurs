#include "lp.h"

result* lp(Data* data){
    printf("PROGRAMMATION LINEAIRE :\n");
    double* res = lpsolv(data, LP_INTEGERS);
    result* r = (result*) malloc(sizeof(result));
    r->open = (int*) malloc(data->facility_count * sizeof(int));
    int i = 0;
    while(i < data->facility_count){
        r->open[i] = (int)res[i];
        i++;
    }
    r->value = eval(data, r->open);
    free(res);
    return r;
}
