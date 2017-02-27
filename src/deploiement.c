#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <limits.h>
#include "lecteur.h"

typedef struct result {
    int* open;
    int value;
} result;

int eval (Data* data, int* ouverts);
result* glouton1 (Data* data);
void beta (int fournisseur_i, Data* data, int* clients_connectes, int* fournisseur_ouverts);

void display_affect(int* o, int size) {
    for (int i = 0; i < size; ++i)
        printf("[%d]%d \n", i, o[i]);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("argc : %d\n", argc);
    }else{
        Data* data = load_instance(argv[1]);
        result* r = glouton1(data);

        display_affect(r->open, data->facility_count);
        printf("VALUE : %d\n", r->value);

        free(r->open);
        free(r);
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
	return (!sum) ? INT_MAX : sum;
}

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

//BETA - EN CHANTIER
void beta (int fournisseur_i, Data* data, int* clients_connectes, int* fournisseur_ouverts){//i énuméré à partir de 1 !
    //Partie 1 :: 2*fi - Sig[j c/c S](c(j,O)-c_i,j)+
    double res = 2*data->opening_cost[fournisseur_i];
    int j = 1;
    int somme = 0;
    while(j <= data->client_count){
        if(! clients_connectes[j-1]){//N'est pas connecté, j c/c S
            //c(j,O)
                int k = 1;
                while(k <= data->facility_count && (!fournisseur_ouverts[k-1])){k++;}
                int k_min = k;
                while(k <= data->facility_count){
                    if(fournisseur_ouverts[k-1] && data->connection[k_min][j]>data->connection[k][j]){
                        k_min = k;
                    }
                    k++;
                }
                int calc = data->connection[k_min][j] - data->connection[fournisseur_i][j];
                calc = (calc>0)? calc : 0; //calc = max(calc,0)
                somme += calc;
            //
        }
    }
    res-=somme;
    //Fin partie 1
    //Beta doit déterminer Y ???
}
