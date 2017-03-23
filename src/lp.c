#include "util.h"

int iVar (int i, int j, Data* data);
Data* load_instance(char* filename);

/*int main(int argc, char *argv[]){
	if(argc != 2){
		printf("USAGE: %s nom_fichier_probleme\n", argv[0]);
		return -1;
	}
	Data* data = load_instance(argv[1]);
	double* open = lpsolv(data, LP_RELAX);
	display_doubles(open, data->facility_count);
	free_data(data);
	free(open);
	return 0;
}*/

result* lp(Data* data){
	double* res = lpsolv(data, LP_INTEGERS);
	result* r = (result*) malloc(sizeof(result));
    r->open = (int*) malloc(data->facility_count * sizeof(int));
    int i = 0;
    while(i < data->facility_count){
    	r->open[i] = (int)res[i];
    	i++;
    }
    r->value = eval(data, r->open);
    free(res);
    return r;
}

result* aa(Data* data){
	result* r = (result*) malloc(sizeof(result));
    r->open = (int*) malloc(data->facility_count * sizeof(int));
    int* current_open = (int*)malloc(data->facility_count * sizeof(int));
    double* reslp = lpsolv(data, LP_RELAX);
    r->value = INT_MAX;
    srand(time(NULL));
    int k = 0;
    int current_val;
    while(k<AA_TENTATIVES){
    	int i = 0;
    	while(i<data->facility_count){
    		double alea = (double)rand()/(double)RAND_MAX;
    		//*TRACE*/printf("A: %f\nV: %f\n",alea, reslp[i]);
    		if(alea < reslp[i]){
    			current_open[i] = 1;
    		}else{
    			current_open[i] = 0;
    		}
    		i++;
    	}
    	current_val = eval(data, current_open);
    	//*TRACE*/printf("VAL %d\n",current_val);
    	if(r->value > current_val){
    		//*TRACE*/printf("%d] Val : %d\n",k,current_val);
    		r->value = current_val;
    		memcpy(r->open, current_open, data->facility_count*sizeof(int));
    	}
    	k++;
    }
    free(current_open);
    free(reslp);
    return r;
}

double* lpsolv(Data* data, int relax){
	/*if(relax){
		printf("Résolution avec contraites relaxées\n");
	}else{
		printf("Résolution avec variables entières\n");
	}*/
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
	while(i<=data->facility_count){//LES FOURNISSEURS i
		sprintf(nom, "x%d", i);
		//*TRACE*/printf("Var %d : %s (%d)\n", i, nom, data->opening_cost[i]);
		glp_set_col_name(lp,i,nom);
		if(! relax){
			glp_set_col_kind(lp,i,GLP_IV);//Var Entière
		}
		glp_set_col_bnds(lp,i,GLP_DB, 0.0, 1.0);//xi € {0,1}
		glp_set_obj_coef(lp,i, (double)data->opening_cost[i]);
		i++;
	}
	//III.2 Connexions
	i=1;
	while(i<=data->facility_count){//LES FOURNISSEURS i
		int j= 1;
		while(j<=data->client_count){//LES CLIENTS j
			int var = iVar(i,j,data);
			sprintf(nom, "y%d-%d", i,j);
			//*TRACE*/printf("Var %d : %s (%d)\n", var, nom, data->connection[i][j]);
			glp_set_col_name(lp, var, nom);
			glp_set_col_bnds(lp, var, GLP_LO, 0.0, 0.0);
			glp_set_obj_coef(lp, var, (double)data->connection[i][j]);
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
	while(j <= data->client_count){
		sprintf(nom, "Client %d Connecte", j);
		//*TRACE*/printf("Ctr %d : %s\n", j, nom);
		glp_set_row_name(lp, j, nom);
		glp_set_row_bnds(lp, j, GLP_FX, 1.0, 1.0);//Total des ouvertures pour le client = 1
		int i = 1;
		while(i <= data->facility_count){
			ia[iMat]=j;
			ja[iMat]=iVar(i,j,data);
			ar[iMat]=1.0;
			//*TRACE*/printf("m[%d,%d]=%f\n", ia[iMat], ja[iMat], ar[iMat]);
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
			sprintf(nom, "Connexion y%d-%d fournisseur ouvert", i,j);
			//*TRACE*/printf("Ctr %d : %s\n",data->client_count+i+(j-1)*data->facility_count, nom);
			glp_set_row_name(lp, data->client_count+i+(j-1)*data->facility_count, nom);
			glp_set_row_bnds(lp, data->client_count+i+(j-1)*data->facility_count, GLP_LO, 0.0, 0.0);// 0 <= Yij - Xi
			ia[iMat]=data->client_count+(j-1)*data->facility_count+i;
			ja[iMat]=i;
			ar[iMat]=1.0;
			//*TRACE*/printf("m[%d,%d]=%f\n", ia[iMat], ja[iMat], ar[iMat]);
			iMat++;
			ia[iMat]=data->client_count+(j-1)*data->facility_count+i;
			ja[iMat]=iVar(i,j,data);
			ar[iMat]=-1.0;
			//*TRACE*/printf("m[%d,%d]=%f\n", ia[iMat], ja[iMat], ar[iMat]);
			iMat++;
			i++;
		}
		j++;
	}
	glp_load_matrix(lp,iMat-1, ia,ja,ar);
	//*TRACE*/glp_write_lp(lp,NULL,"LP_PROB_DUMP.txt");
	//V RESOLUTION
	if(relax){//CAS RELAXE
		glp_smcp param;  //Paramètres pour le solveur
		glp_init_smcp(&param);
		param.msg_lev = GLP_MSG_OFF;// "TG"
		glp_simplex(lp, &param);
	}else{//CAS VALEUR ENTIERES
		glp_iocp param; //Paramètres pour le solveur
		glp_init_iocp(&param);
		param.presolve = GLP_ON; //Activer le présolveur (on sait pas pourquoi)
		param.msg_lev = GLP_MSG_OFF;// "TG"
		glp_intopt(lp,&param); //Résoudre avec l'algorithme int opt
	}
	//VI RESULTATS
	double* ret = (double*)(malloc(data->facility_count*sizeof(double)));
	if(relax){
		//*TRACE*/printf("Optimal : %f\n", glp_get_obj_val(lp));
		i=0;
		while(i<data->facility_count){
			ret[i] = glp_get_col_prim(lp, i+1);
			i++;
		}
	}else{
		//*TRACE*/printf("Optimal : %f\n", glp_mip_obj_val(lp));
		i=0;
		while(i<data->facility_count){
			ret[i] = glp_mip_col_val(lp, i+1);
			i++;
		}		
	}
	//VII MENAGE
	free(ia);
	free(ja);
	free(ar);
	glp_delete_prob(lp);
	glp_free_env();
	//
	return ret;
}

int iVar (int i, int j, Data* data){
	int nF = data->facility_count;
	int nC = data->client_count;
	return (nF+j+(i-1)*nC);
}
