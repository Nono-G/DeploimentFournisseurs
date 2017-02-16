#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int client_count;
	int facility_count;
	int* opening_cost;
	int** connection;
} Data;

void skipLine(FILE *fp);
Data* load_instance(char* filename);
void free_data(Data* data);