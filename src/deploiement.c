#include "util.h"
#include "glouton1.h"
#include "glouton2.h"
#include "lp.h"
#include "arondialeatoire.h"

#include <getopt.h>

void usage(const char* exec) {
    printf("usage: %s [12ladh] nom_du_fichier_probleme\n", exec);
    printf("\t-1: Algo Glouton 1\n");
    printf("\t-2: Algo Glouton 2 (Default)\n");
    printf("\t-l: Programmation Linéaire\n");
    printf("\t-a: Programmation Linéaire + Arrondi Aléatoire\n");
    printf("\t-d: Afficher les fournisseurs ouverts\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc - 1 < 1) usage(argv[0]);

    int flag1 = 0;
    // int flag2 = 0;
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
                // flag2 = 1;
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
    result* r;

    if (flag1) r = glouton1(data);
    else if (lflag) r = lp(data);
    else if (aflag) r = aa(data);
    else r = glouton2(data);

    if (dflag) display_int(r->open, data->facility_count);

    printf("VALUE : %d\n", r->value);

    free_result(r);
    free_data(data);
    return 0;
}
