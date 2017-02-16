#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <limits.h>
#include "lecteur.h"

int eval (Data* data, int* ouverts);
int* glouton1 (Data* data);

void display_affect(int* o, int size) {
    for (int i = 0; i < size; ++i)
        printf("[%d]%d \n", i, o[i]);
}


int main(int argc, char *argv[]){
    if(argc != 2){
        printf("argc : %d\n", argc);
    }else{
        Data* data = load_instance(argv[1]);
        int* affect = glouton1(data);

        display_affect(affect, data->facility_count);

        free(affect);
        free_data(data);
    }
    return 0;
}

int eval (Data* data, int* ouverts){

	int sum = 0;
	//Somme Couts ouverture
	int i = 1;
	while(i <= data->facility_count){
		if(ouverts[i-1]){
			//printf("ouverture : %d\n", data->opening_cost[i]);
			sum += data->opening_cost[i];
		}
		i++;
	}
	//Somme couts connexion
	int j = 1;
	while(j <= data->client_count){
		i=1;
		while(!ouverts[i-1]){
			i++;
		}
		int iMin = i;
		while(i <= data->facility_count){
			if(ouverts[i-1] && (data->connection[i][j] < data->connection[iMin][j])){
				iMin = i;
			}
			i++;
		}
		//printf("iMin : %d\nConnection : %d\n", iMin,data->connection[iMin][j]);
		if(iMin <= data->facility_count){
			sum += data->connection[iMin][j];
		}
		j++;
	}
	return sum;
}

int* glouton1 (Data* data) {
    int* o = (int*) malloc(data->facility_count * sizeof(int));
    for (int i = 0; i < data->facility_count; ++i)
        o[i] = 0;
    while (42) {
        int old_value = eval(data, o);
        printf("OLD:%d\n", old_value);
        int min_value, i_min, value;
        int i = 1;
        while(o[i]){i++;}
        o[i] = 1;
        min_value = eval(data, o);
        i_min = i;
        o[i] = 0;
        while(i<= data->facility_count){
            if(!o[i]){
                o[i] = 1;
                printf("AFFECT 1\n");
                value = eval(data, o);
                o[i] = 0;
                if (value < min_value){
                    printf("MIN !\n");
                    min_value = value;
                    i_min = i;
                }
            }
            i++;
        }
        if(min_value < old_value){
            printf("OLD\n");
            o[i_min] = 1;
        }else{
            break;
        }
    }
    return o;
}
