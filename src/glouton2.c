#include "glouton2.h"

//Penser a free le return
//Retourne struct :
//  value : le ration
//  y_clients : le tableau de clients y
//  y_size : taille explicite de y_clients
beta_return* beta (Data* data, int fournisseur_i, int* fournisseur_ouverts, int* clients_connectes){//i énuméré à partir de 1 !
    //Partie 1 :: 2*fi - Sig[j c/c S](c(j,O)-c_i,j)+
    double res = 1 * data->opening_cost[fournisseur_i];
    int j = 0;
    int somme = 0;
    while(j < data->client_count){
        if(clients_connectes[j]){//N'est pas connecté, j c/c S
            //c(j,O)
            int k = 0;
            while(k < data->facility_count && (!fournisseur_ouverts[k])){k++;}
            int k_min = k;
            while(k < data->facility_count){
                if(fournisseur_ouverts[k] && data->connection[k_min][j]>data->connection[k][j]){
                    k_min = k;
                }
                k++;
            }
            int calc = data->connection[k_min][j] - data->connection[fournisseur_i][j];
            calc = (calc>0)? calc : 0; //calc = max(calc,0)
            somme += calc;
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
    beta_return* ret = malloc(sizeof(beta_return));
    ret->y_clients = malloc(data->client_count*sizeof(int));
    for (int i = 0; i < data->client_count; ++i) {
        ret->y_clients[i] = 0;
    }
    ret->y_size = 0;
    j=0;
    double newRes;
    while(j<nClientsNC){
        newRes = res+data->connection[fournisseur_i][clientsNC[j]];
        if((newRes/(ret->y_size+1)) < (res/ret->y_size)){
            ret->y_clients[clientsNC[j]] = 1;
            ret->y_size = (ret->y_size+1);
            res = newRes;
        }else{
            break;
        }
        j++;
    }
    ret->value = res / (ret->y_size);
    free(clientsNC);
    return ret;
}

result* glouton2 (Data* data) {
    result* r = (result*) malloc (sizeof(result));
    r->open = (int*) malloc (data->facility_count * sizeof(int));
    for (int i = 0; i < data->facility_count; ++i)
        r->open[i] = 0;
    int* connected_client = (int*) malloc(data->client_count * sizeof(int));
    int connected_client_count = 0;
    for (int i = 0; i < data->client_count; ++i)
        connected_client[i] = 0;
    heap_t* tas = create_heap(1000000); // type : 1 = fournisseur, 2 = client
    // Pour chaque i € F
    for (int i = 0; i < data->facility_count; ++i) {
        beta_return* b = beta(data, i, r->open, connected_client);
        push(tas, b->value, TYPE_FOURNISEUR, i);
        free_beta_return(b);
    }
    // tant que le tas n'est pas vide
    node_t* current;
    while (connected_client_count < data->client_count && ! is_empty(tas)) {
        current = pop(tas);
        if (current->type == TYPE_FOURNISEUR) { // si c'est un fournisseur
            beta_return* b = beta(data, current->indice, r->open, connected_client);
            if (b->value == current->priority) { // current beta = beta
                r->open[current->indice] = 1; // O u i
                // on retire de S les connected_client connectés par i pour ce ratio
                for (int i = 0; i < data->client_count; ++i) {
                    if (b->y_clients[i]) {
                        connected_client[i] = 1;
                        connected_client_count++;
                    }
                }
                // ajoute dans le tas les (Cij)j€S
                for (int i = 0; i < data->client_count; ++i) {
                    if (! connected_client[i]) {
                        push(tas, data->connection[current->indice][i], TYPE_CLIENT, i);
                    }
                }
            } else {
                // remet dans le tas la valeur actualisée
                push(tas, b->value, TYPE_FOURNISEUR, current->indice);
            }
            free_beta_return(b);
        } else { // c'est un client
            if (! connected_client[current->indice]) {
                connected_client[current->indice] = 1;
                connected_client_count++;
            }
        }
        free(current);
    }
    free(connected_client);
    free_heap(tas);
    r->value = eval(data, r->open);
    return r;
}
