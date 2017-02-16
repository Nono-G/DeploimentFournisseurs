#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include "lecteur.h"

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("argc : %d\n", argc);
	}else{
		Data* data = load_instance(argv[1]);
		printf("%d \n", data->connection[data->facility_count][data->client_count]);
		free_data(data);
	}
	return 0;
}