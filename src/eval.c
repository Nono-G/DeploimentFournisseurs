#include "eval.h"

int eval (Data* data, int* ouverts){
    int sum = 0;
    //Somme Couts ouverture
    int i = 0;
    while(i < data->facility_count){
        if(ouverts[i]){
            //printf("ouverture : %d\n", data->opening_cost[i]);
            sum += data->opening_cost[i];
        }
        i++;
    }
    //Somme couts connexion
    int j = 0;
    while(j < data->client_count){
        i = 0;
        while(i < data->facility_count && !ouverts[i]){
            i++;
        }
        int iMin = i;
        while(i < data->facility_count){
            if(ouverts[i] && (data->connection[i][j] < data->connection[iMin][j])){
                iMin = i;
            }
            i++;
        }
        //printf("iMin : %d\nConnection : %d\n", iMin,data->connection[iMin][j]);
        if(iMin < data->facility_count){
            sum += data->connection[iMin][j];
        }
        j++;
    }
    return (!sum) ? INT_MAX : sum;
}
