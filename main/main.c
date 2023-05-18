#include <printf.h>
#include <stdlib.h>
#include <stdio.h>
#include "Treedir.h"

extern noeud * n;

int main(int argc, char* argv[]){

    if (argc == 2){

        char * filename = argv[1];

        FILE * f = fopen(filename, "r");
        if (f == NULL){
            printf("Fichier non trouver.\n");
            exit(1);
        }

        creer_racine();
        read_file(f);

        //Permet d'afficher l'arborescence sous forme d'arbre, utile que pour debug.
        print_mode_arbre(n->racine, 0);

        fclose(f);
        fermeture(n->racine);

    }

    return EXIT_SUCCESS;
}