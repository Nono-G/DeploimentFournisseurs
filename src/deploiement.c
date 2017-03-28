#include "util.h"
#include "glouton1.h"
#include "glouton2.h"
#include "lp.h"
#include "arondialeatoire.h"

#include <getopt.h>
#include <time.h>

void usage(const char* exec) {
    printf("usage: %s [12ladh] fichier\n", exec);
    printf("\t-1: Glouton 1\n");
    printf("\t-2: Glouton 2 (Default)\n");
    printf("\t-l: Programmation Linéaire\n");
    printf("\t-a: Programmation Linéaire + Arrondi Aléatoire\n");
    printf("\t-d: Afficher les fournisseurs ouverts\n");
    exit(0);
}

void display_affect(int* affect, int size) {
    printf("Les fourniseurs suivants sont ouverts :");
    int cpt = 0;
    for (int i = 0; i < size; ++i) {
        if (affect[i]) {
            if (cpt % 5 == 0) printf("\n");
            printf("\t%d", i+1);
            cpt++;
        }
    }
    printf("\nSoit %d fournissseur(s) ouvert(s).\n", cpt);
}

void deploy(Data * data, result* (*funct)(Data*), char* funct_name, int display) {
    printf("%s\n", funct_name); // cosmétique
    clock_t start = clock();
    result* r = funct(data);
    clock_t end = clock();
    if (display) display_affect(r->open, data->facility_count);
    printf("Pour un coût de %d\n", r->value);
    printf("Calculé en %f secondes.\n\n", (double)(end - start) / CLOCKS_PER_SEC);
    free_result(r);
}

int main(int argc, char *argv[]) {
    if (argc - 1 < 1) usage(argv[0]);

    int flag1 = 0;
    int flag2 = 0;
    int lflag = 0;
    int aflag = 0;
    int dflag = 0;
    int c;
    opterr = 0;

    while ((c = getopt (argc, argv, "12ladh")) != -1) {
        switch (c) {
            case '1':
                flag1 = 1;
                break;
            case '2':
                flag2 = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 'a':
                aflag = 1;
                break;
            case 'd':
                dflag = 1;
                break;
            case 'h':
                usage(argv[0]);
            case '?':
                fprintf(stderr, "Option inconnu `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    Data* data = load_instance2(argv[optind]);
    if (flag1) deploy(data, glouton1, "GLOUNTON 1", dflag);
    if (flag2) deploy(data, glouton2, "GLOUNTON 2", dflag);
    if (lflag) deploy(data, lp, "PROGRAMMATION LINEAIRE", dflag);
    if (aflag) deploy(data, aa , "ARRONDI ALEATOIRE", dflag);
    if (! flag1 && ! flag2 && ! lflag && ! aflag) deploy(data, glouton2, "GLOUNTON 2", dflag);
    free_data(data);
    return 0;
}
