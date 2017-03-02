#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <limits.h>
#include "lecteur.h"

typedef struct result {
    int* open;
    int value;
} result;

typedef struct beta_return {
    int* y_clients;
    int y_size;
    double value;
} beta_return;

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int priority;
    int type;
    int indice;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

void push (heap_t *h, int priority, int type, int indice) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = (node_t *)realloc(h->nodes, h->size * sizeof (node_t));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].priority > priority) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i].priority = priority;
    h->nodes[i].type = type;
    h->nodes[i].indice = indice;
    h->len++;
}

node_t* pop (heap_t *h) {
    int i, j, k;
    if (!h->len) return NULL;
    node_t data = h->nodes[1];  //     ICICICICICICICICICICICICICICICICICICICICIC
    h->nodes[1] = h->nodes[h->len];
    h->len--;
    i = 1;
    while (1) {
        k = i;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].priority < h->nodes[k].priority)
            k = j;
        if (j + 1 <= h->len && h->nodes[j + 1].priority < h->nodes[k].priority)
            k = j + 1;
        if (k == i) break;
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    h->nodes[i] = h->nodes[h->len + 1];
    return data;
}

// int main () {
//     heap_t *h = (heap_t *)calloc(1, sizeof (heap_t));
//     push(h, 3, "Clear drains");
//     push(h, 4, "Feed cat");
//     push(h, 5, "Make tea");
//     push(h, 1, "Solve RC tasks");
//     push(h, 2, "Tax return");
//     int i;
//     for (i = 0; i < 5; i++) {
//         printf("%s\n", pop(h));
//     }
//     return 0;
// }

int eval (Data* data, int* ouverts);
result* glouton1 (Data* data);
beta_return* beta (int fournisseur_i, Data* data, int* clients_connectes, int* fournisseur_ouverts);
void free_beta_return(beta_return* p);
void display_affect(int* o, int size){
    for (int i = 0; i < size; ++i)
        printf("[%d]%d \n", i, o[i]);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("argc : %d\n", argc);
    }else{
        Data* data = load_instance(argv[1]);
        //result* r = glouton1(data);
        int* test_clients_co = malloc(data->client_count*sizeof(int));
        test_clients_co[1] = 1;
        int* test_fournisseurs_o = malloc(data->facility_count*sizeof(int));
        test_fournisseurs_o[0] = 1;
        beta_return* b = beta (4,data,test_clients_co,test_fournisseurs_o);
        printf("BETA : %f\n", b->value);
        printf("BETA : %d\n", b->y_size);
        display_affect(b->y_clients, b->y_size);

        free_beta_return(b);
        free(test_fournisseurs_o);
        free(test_clients_co);
        //display_affect(r->open, data->facility_count);
        //printf("VALUE : %d\n", r->value);

        //free(r->open);
        //free(r);
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

void permute(int i, int j, int* array) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void placer_pivot(Data* data, int fournisseur_i, int g, int d, int* array, int *p) {
    int i;
    int pivot = data->connection[fournisseur_i][g];
    *p = g;
    for (i = g + 1; i <= d; i++) {
        if (data->connection[fournisseur_i][i] < pivot) {
            (*p)++;
            if (i != *p) permute(i, *p, array);
        }
    }
    permute(*p, g, array);
}

void tri_rapide_back(Data* data, int fournisseur_i, int g, int d, int* array) {
    int p;
    if (g < d) {
        placer_pivot(data, fournisseur_i, g, d, array, &p);
        tri_rapide_back(data, fournisseur_i, g, p - 1, array);
        tri_rapide_back(data, fournisseur_i, p + 1, d, array);
    }
}

void tri_rapide(Data* data, int fournisseur_i, int* array, int arraysize) {
    int debut = 0;
    tri_rapide_back(data, fournisseur_i, debut, arraysize - 1, array);
}

//Penser a free le return
//Retourne struct :
//  value : le ration
//  y_clients : le tableau de clients y
//  y_size : taille explicite de y_clients
beta_return* beta (int fournisseur_i, Data* data, int* clients_connectes, int* fournisseur_ouverts){//i énuméré à partir de 1 !
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
        j++;
    }
    res-=somme;
    //Fin partie 1
    //Trier les clients par coûts de connexion
    int nClientsNC = 0;
    int* clientsNC = malloc(data->client_count*sizeof(int));
    j=0;
    while(j < data->client_count){
        if(!clients_connectes[j]){
            clientsNC[nClientsNC] = j;
            nClientsNC++;
        }
        j++;
    }
    tri_rapide(data, fournisseur_i, clientsNC, nClientsNC);
    //clientsNC est trié
    display_affect(clientsNC, nClientsNC);
    beta_return* ret = malloc(sizeof(beta_return));
    ret->y_clients = malloc(data->client_count*sizeof(int));
    ret->y_size = 0;
    j=0;
    double newRes;
    while(j<nClientsNC){
        newRes = res+data->connection[fournisseur_i][clientsNC[j]]/(ret->y_size+1);
        if(newRes < res){
            ret->y_clients[ret->y_size] = clientsNC[j];
            ret->y_size = (ret->y_size+1);
            res = newRes;
        }else{
            break;
        }
        j++;
    }
    ret->value = res;
    free(clientsNC);
    return ret;
}

void free_beta_return(beta_return* p){
    free(p->y_clients);
    free(p);
}
