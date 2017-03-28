#include "glouton1.h"

result* glouton1 (Data* data) {
    result* r = (result*) malloc(sizeof(result));
    r->open = (int*) malloc(data->facility_count * sizeof(int));
    for (int i = 0; i < data->facility_count; ++i)
        r->open[i] = 0;
    int old_value;
    while (42) {
        old_value = eval(data, r->open);
        int min_value, i_min, value;
        int i = 0;
        while(r->open[i]){i++;}//Skip true values
        r->open[i] = 1;
        min_value = eval(data, r->open);
        i_min = i;
        r->open[i] = 0;
        while(i < data->facility_count){
            if(!r->open[i]){
                r->open[i] = 1;
                value = eval(data, r->open);
                r->open[i] = 0;
                if (value < min_value){
                    min_value = value;
                    i_min = i;
                }
            }
            i++;
        }
        if(min_value < old_value){
            r->open[i_min] = 1;
        }else{
            break;
        }
    }
    r->value = old_value;
    return r;
}
