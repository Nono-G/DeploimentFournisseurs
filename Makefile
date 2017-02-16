all : deploiement
	
deploiement : deploiement.o lecteur.o
	gcc -std=c11 -Wall -o deploiement deploiement.o lecteur.o

deploiement.o : lecteur.h
	gcc -std=c11 -Wall -o deploiement.o -c deploiement.c

lecteur.o :
	gcc -std=c11 -Wall -o lecteur.o -c lecteur.c

clean :
	rm *.o