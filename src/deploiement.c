#include "util.h"
#include "quicksort.h"
#include "priority.h"

beta_return* beta2(Data* data, int supplier, int* openned_supplier, int* connected_client);

#define TYPE_FOURNISEUR 1
#define TYPE_CLIENT 2

void display_int(int* o, int size){
    for (int i = 0; i < size; ++i)
        printf("[%d]%d \n", i, o[i]);
}

void display_doubles(double* o, int size){
    for (int i = 0; i < size; ++i)
        printf("[%d]%f \n", i, o[i]);
}

int main(int argc, char *argv[]){
    /*
    SEGFAULT si le fichier n'est pas ouvrable(n'existe pas), faut-il le tester ?
    */
    if(argc != 4){
        printf("USAGE : %s (g1|g2|lp|aa) (aff|val) nom_du_fichier_probleme\n", argv[0]);
        printf("\t g1 : Algo Glouton 1\n");
        printf("\t g2 : Algo Glouton 2\n");
        printf("\t lp : Programmation Linéaire\n");
        printf("\t aa : Programmation Linéaire + Arrondi Aléatoire\n");
        printf("\t aff : Afficher les fournisseurs ouverts\n");
        printf("\t val : Seulement la valeur\n");
        return 1;
    }
    Data* data = load_instance2(argv[3]);
    result* r;
    if(strcmp(argv[1], "g1")==0){
        r = glouton1(data);

    }else if(strcmp(argv[1], "g2")==0){
        // printf("Glouton 2 pas prêt !\n");
        r = glouton2(data);
        // return 2;
    }else if(strcmp(argv[1], "lp")==0){
        r = lp(data);
    }else if(strcmp(argv[1], "aa")==0){
        r = aa(data);
    }

    if(strcmp(argv[2], "aff")==0){display_int(r->open, data->facility_count);}
    printf("VALUE : %d\n", r->value);

    free_result(r);
    free_data(data);
    return 0;
}
/*
    int* test_clients_co = malloc(data->client_count*sizeof(int));
    test_clients_co[1] = 1;
    int* test_fournisseurs_o = malloc(data->facility_count*sizeof(int));
    test_fournisseurs_o[0] = 1;
    beta_return* b = beta (4,data,test_clients_co,test_fournisseurs_o);
    printf("BETA : %f\n", b->value);
    printf("BETA : %d\n", b->y_size);
    display_int(b->y_clients, b->y_size);

    free_beta_return(b);
    free(test_fournisseurs_o);
    free(test_clients_co);
        //display_int(r->open, data->facility_count);
        //printf("VALUE : %d\n", r->value);

        //free(r->open);
        //free(r);
    free_data(data);
    return 0;
*/

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

result* glouton2(Data* data) {
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
    while ((current = pop(tas)) != NULL) { /// BBBBBBBBBBBBBBBBBBBBBBBBBBBOOOOOOOOOOOOOOUUUUUUUUUUUUCCCCCCCCCCCCCCLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEEEEEEEEE INFINI
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
    printf("BETA\n");
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
