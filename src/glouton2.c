#include "glouton2.h"

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
        }
        j++;
    }
    printf("B1\n");
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
    display_int(clientsNC, nClientsNC);
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

int min_connexion_cost(Data* data, int client, int* openned_supplier) {
    int min = INT_MIN;
    for (int i = 1; i < data->facility_count; ++i)
        if (openned_supplier[i] && min > data->connection[i][client])
            min = data->connection[i][client];
    return min;
}

beta_return* beta2(Data* data, int supplier, int* openned_supplier, int* connected_client) {
    printf("BETA");
    beta_return* r = (beta_return*) malloc (sizeof(beta_return));
    r->y_clients = (int*) malloc (data->client_count * sizeof(int));
    r->y_size = 0;
    // I calcul
    double value = 2 * data->opening_cost[supplier]; // 2 * Fi
    int sum = 0;
    for (int j = 0; j < data->client_count; ++j) { // sum
        if (! connected_client[j]) { // client non connecté
            int cost = min_connexion_cost(data, j, openned_supplier) - data->connection[supplier][j]; // c(j, O) - Cij
            sum += (cost > 0) ? cost : 0; // (c(j, O) - Cij)+
        }
    }
    value -= sum; // 2 * Fi - Sum (c(j, O) - Cij)+
    // II trie les coût de connexion
    int* clients_s = (int*) malloc (data->client_count * sizeof(int));
    int clients_s_size = 0;
    for (int j = 0; j < data->client_count; ++j) {
        if (! connected_client[j]) // client non connecté
            clients_s[clients_s_size++] = j;
    }
    tri_rapide(data, supplier, clients_s, clients_s_size);
    // III Ajouter a Y
    double ratio = value, new_ratio;
    for (int j = 0; j < clients_s_size; ++j) {
        new_ratio = ratio + data->connection[supplier][clients_s[j]] / (r->y_size + 1);
        if (new_ratio < ratio) { // si le ratio augmente
            r->y_clients[r->y_size] = clients_s[j];
            r->y_size = r->y_size + 1;
            ratio = new_ratio;
        } else break;
    }
    free(clients_s);
    r->value = ratio;
    return r;
}

result* glouton2 (Data* data) {
    printf("INIT...\n");
    result* r = (result*) malloc (sizeof(result));
    r->open = (int*) malloc (data->facility_count * sizeof(int));
    for (int i = 0; i < data->facility_count; ++i)
        r->open[i] = 0;
    int* connected_client = (int*) malloc(data->client_count * sizeof(int));
    for (int i = 0; i < data->client_count; ++i)
        connected_client[i] = 0;
    heap_t* tas = create_heap(); // type : 1 = fournisseur, 2 = client
    printf("Fin init.\n");
    // Pour chaque i € F
    for (int i = 0; i < data->facility_count; ++i) {
        // beta_return* b = beta(i, data, connected_client, r->open);
        beta_return* b = beta2(data, i, r->open, connected_client);
        push(tas, (int) b->value, TYPE_FOURNISEUR, i);
        free_beta_return(b);
    }
    // tant que le tas n'est pas vide
    node_t* current;
    int cpt = 1000;
    while ((current = pop(tas)) != NULL && cpt-- > 0) { /// BBBBBBBBBBBBBBBBBBBBBBBBBBBOOOOOOOOOOOOOOUUUUUUUUUUUUCCCCCCCCCCCCCCLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEEEEEEEEE INFINI
        if (current->type == TYPE_FOURNISEUR) { // si c'est un fournisseur
            // beta_return* b = beta(current->indice, data, connected_client, r->open);
            beta_return* b = beta2(data, current->indice, r->open, connected_client);
            if (b->value == current->priority) { // current beta = beta
                r->open[current->indice] = 1; // O u i
                // on retire de S les connected_client connectés par i pour ce ratio
                for (int i = 0; i < data->client_count; ++i) {
                    if (! connected_client[i] &&
                            data->connection[current->indice][i] < b->value) {
                        connected_client[i] = 1;
                    }
                }
                // ajoute dans le tas les (Cij)j€S
                for (int i = 0; i < data->client_count; ++i) {
                    if (! connected_client[i]) {
                        push(tas, data->connection[current->indice][i], TYPE_CLIENT, i); // PROBLEME ICI CAR PRIORITÉ PLUS FAIBLE ! ! !! !
                    }
                }
            } else {
                // remet dans le tas la valeur actualisée
                push(tas, b->value, TYPE_FOURNISEUR, current->indice);
            }
            free_beta_return(b);
        } else { // c'est un client
            if (! connected_client[current->indice])
                connected_client[current->indice] = 1;
        }
    }
    free(connected_client);
    free_heap(tas);
    r->value = eval(data, r->open);
    return r;
}
