//COMPILATION
/*
gcc -Wall -std=c11 -Iinc src/lp.c -lglpk -o lp
*/

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#define TAILLE_MATRICE_GLPK 1000001
#define TAILLE_NOM 100

typedef struct {
    int client_count;
    int facility_count;
    int* opening_cost;
    int** connection;
} Data;

void solv(Data* data);
int iVar (int i, int j, Data* data);
Data* load_instance(char* filename);
void free_data(Data* data);
void skipLine(FILE *fp);


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("USAGE: %s nom_fichier_probleme\n", argv[0]);
		return -1;
	}
	Data* data = load_instance(argv[1]);
	solv(data);
	free_data(data);
	return 0;
}

void solv(Data* data){
	char nom[TAILLE_NOM];
	//I PROBLEME
	glp_prob* lp;
	lp = glp_create_prob();
	glp_set_prob_name(lp,"Deploiement");
	//II OBJECTIF
	glp_set_obj_dir(lp,GLP_MIN);//Minimiser
	//III VARIABLES
	int nVar = (data->facility_count)+((data->facility_count)*(data->client_count)); 
	glp_add_cols(lp,nVar);
	//III.1 Ouvertures
	int i = 1;
	while(i<=data->facility_count){//LES FOURNISSEURS
		sprintf(nom, "x%d", i);
		glp_set_col_name(lp,i,nom);
		glp_set_col_kind(lp,i,GLP_IV);//Var Entière
		glp_set_col_bnds(lp,i,GLP_DB, 0.0, 1.0);//xi € {0,1}
		glp_set_obj_coef(lp,i,data->opening_cost[i]);
		i++;
	}
	//III.2 Connexions
	i=1;
	while(i<=data->facility_count){//LES FOURNISSEURS
		int j= 1;
		while(j<=data->client_count){//LES CLIENTS
			int var = iVar(i,j,data);
			sprintf(nom, "y%d-%d", i,j);
			glp_set_col_name(lp, var, nom);
			glp_set_col_bnds(lp, var, GLP_LO, 0.0, 0.0);
			glp_set_obj_coef(lp, var, data->connection[i][j]);
			j++;
		}
		i++;
	}
	//IV CONTRAINTES ET MATRICE
	int* ia = malloc(TAILLE_MATRICE_GLPK*sizeof(int));
	int* ja = malloc(TAILLE_MATRICE_GLPK*sizeof(int));
	double* ar = malloc(TAILLE_MATRICE_GLPK*sizeof(int));
	int iMat = 1;
	int nConstr = (data->client_count)+((data->facility_count)*(data->client_count)); 
	glp_add_rows(lp,nConstr);
	//IV.1 "Je dois me connecter a un fournisseur"
	int j = 1;
	while(j <= data-> client_count){
		sprintf(nom, "Client %d Connecte", j);
		glp_set_row_name(lp, i, nom);
		glp_set_row_bnds(lp, i, GLP_FX, 1.0, 1.0);//Total des ouvertures pour le client = 1
		int i = 1;
		while(i <= data->facility_count){
			ia[iMat]=j;
			ja[iMat]=iVar(i,j,data);
			ar[iMat]=1.0;
			iMat++;
			i++;
		}
		j++;
	}
	//IV.2 "Le fournisseur auquel je me connecte doit être ouvert"
	j = 1;
	while(j <= data->client_count){
		int i = 1;
		while(i <= data->facility_count){
			sprintf(nom, "y%d-%d < x%d", i,j,i);
			glp_set_row_name(lp, i, nom);
			glp_set_row_bnds(lp, i, GLP_LO, 0.0, 0.0);// 0 <= Yij - Xi
			ia[iMat]=(j-1)*data->facility_count+i;
			ja[iMat]=i;
			ar[iMat]=1.0;
			ia[iMat+1]=(j-1)*data->facility_count+i;
			ja[iMat+1]=iVar(i,j,data);
			ar[iMat+1]=-1.0;
			iMat++;
			i++;
		}
		j++;
	}
	//V RESOLUTION
	glp_load_matrix(lp,iMat, ia,ja,ar);
	glp_write_lp(lp,NULL,"aaaaaaa");
	glp_simplex(lp, NULL);
	free(ia);
	free(ja);
	free(ar);
	glp_delete_prob(lp);
	glp_free_env();


}

int iVar (int i, int j, Data* data){
	int nF = data->facility_count;
	return nF+j+(i-1)*nF;
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

void skipLine(FILE *fp){
    while (fgetc(fp)!='\n');
}