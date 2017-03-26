#ifndef __LPSOLV_H__
#define __LPSOLV_H__

#include "util.h"

#define TAILLE_MATRICE_GLPK 1000001
#define TAILLE_NOM 100
#define LP_INTEGERS 0
#define LP_RELAX 1
#define AA_TENTATIVES 1000

double* lpsolv (Data* data, int relax);

#endif
