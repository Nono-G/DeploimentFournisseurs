# DeploimentFournisseurs (Projet APG)

Auteurs GOUDIAN Noé, BERNARDINI Mickaël

## Structure

* _src_ contiens les sources du projet
* _inc_ les fichiers headers
* _BlideKraup_ et _KoerkelGhosh-sym_ sont les fichiers de tests

## Compilation

Le projet contiens un _Makefile_, on peut donc compiler simplement en lançant la
 commande `$ make`.

## Utilisation

La compilation génère un exécutable nommé "deploiment". Cet exécutable fonctionne
 avec des options :

* `-1` Pour utiliser l'algorithme glouton 1
* `-2` Pour utiliser l'algorithme glouton 2
* `-l` Pour utiliser la programmation linéaire
* `-a` Pour utiliser la méthode de l'arrondi aléatoire

Toute ces options sont compatibles entre elles(ex: -12, -a1, ...).

* `-d` Permet d'afficher l'affectation des fournisseurs
* `-h` Permet d'afficher les usages

Pour exécuter:

`$ ./deploiment [12lahd] <nom du fichier probleme>`
