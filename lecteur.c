#include "lecteur.h"
/*Exemple de loader que vous pouvez modifier Ã  souhait de maniÃ¨re Ã  
 *obtenir une structure de donnÃ©es qui vous correspond.
 *Le but est que vous n'y passiez pas trop de temps. */ 

void skipLine(FILE *fp){
	while (fgetc(fp)!='\n');
}

Data* load_instance(char* filename){
	FILE *fp;
   fp=fopen(filename, "r");
   skipLine(fp);
   Data* data=malloc(sizeof(data));
   int nothing;
   fscanf(fp,"%d %d %d", &data->facility_count, &data->client_count, &nothing);  
   data->opening_cost=malloc((1+data->facility_count)*sizeof(int));
   data->connection=malloc((1+data->facility_count)*sizeof(int*));
   for (int fac=1;fac<=data->facility_count;fac++){
	   fscanf(fp,"%d %d",&nothing, &data->opening_cost[fac]);
	   data->connection[fac]=malloc((1+data->client_count)*sizeof(int));
	   for(int client=1;client<=data->client_count; client++) {
		   fscanf(fp,"%d", &data->connection[fac][client]);
		   }
	   }
	fclose(fp);
	return data;
}

void free_data(Data* data){
	for (int fac=1; fac<=data->facility_count;fac++)
		free(data->connection[fac]);
	free(data->connection);
	free(data->opening_cost);
	free(data);
	
	return;
}