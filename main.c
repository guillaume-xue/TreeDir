#include <printf.h>
#include "Treedir.h"

extern noeud * n;

int main(){

    creer_racine();
    mkdir(dupliquer_char("Cours"));
    cd_chem(dupliquer_char("Cours"));
    mkdir(dupliquer_char("ProjetC"));
    mkdir(dupliquer_char("Anglais"));
    cd_racine();
    touch(dupliquer_char("edt"));
    cp(dupliquer_char("Cours"), dupliquer_char("/Td"));
    print(n->racine);
    printf("\n");

    rm(dupliquer_char("/Td/ProjetC"));
    print(n->racine);
    printf("\n");

    rm(dupliquer_char("/Td/Anglais"));
    print(n->racine);
    printf("\n");

    cd_chem(dupliquer_char("Td"));
    printf("\n");

    mkdir(dupliquer_char("td1"));
    mkdir(dupliquer_char("td2"));
    print(n->racine);
    printf("\n");

    cp(dupliquer_char("/Cours/ProjetC"), dupliquer_char("CopieProjetC"));
    print(n->racine);


    /*
    creer_racine();

    mkdir("Cours");
    mkdir("Td");
    touch("edt");

    cd_chem("Cours");
    mkdir("ProjetC");
    mkdir("Anglais");
    cd_pere(n);
    cd_chem(n, "Td");
    touch(n, "td1");
    touch(n, "td2");
    cd_pere(n);
    cd_chem(n, "Cours");
    cd_chem(n, "ProjetC");

    print(n->n);
     */
    return 0;
}