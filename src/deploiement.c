#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include "lecteur.h"

int eval (Data* data, int* ouverts);

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("argc : %d\n", argc);
	}else{
		Data* data = load_instance(argv[1]);
		printf("%d \n", data->connection[data->facility_count][data->client_count]);
		printf("%d \n", data->connection[1][1]);
		int test[2] = {0,0};
		printf("eval(O) : %d\n",eval(data,test));
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


