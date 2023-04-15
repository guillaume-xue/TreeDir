#include "Treedir.h"

int main(){
    noeud * n = creer_racine();
    mkdir(n, "Cours");
    n = cd_chem(n,"Cours");
    mkdir(n, "ProjetC");
    mkdir(n, "Anglais");
    n = cd_racine(n);
    touch(n, "edt");
    cp(n,"Cours", "/Td");
    char s1[] = "/Td/ProjetC";
    rm(n, s1);
    char s2[] = "/Td/Anglais";
    rm(n, s2);
    n = cd_chem(n,"Td");
    mkdir(n,"td1");
    mkdir(n,"td2");
    char s3[] = "/Cours/ProjetC";
    char s4[] = "/CopieProjetC";
    cp(n, s3, s4);
    n = cd_racine(n);
    char s5[] = "/Td";
    char s6[] = "/Cours/Td";
    mv(n,s5, s6);
    print(n->racine);


    /*
    noeud * n = creer_racine();

    mkdir(n,"Cours");
    mkdir(n,"Td");
    touch(n, "edt");

    n = cd_chem(n, "Cours");
    mkdir(n, "ProjetC");
    mkdir(n, "Anglais");
    n = cd_pere(n);
    n = cd_chem(n, "Td");
    touch(n, "td1");
    touch(n, "td2");
    n = cd_pere(n);
    n = cd_chem(n, "Cours");
    n = cd_chem(n, "ProjetC");

    print(n->racine);
     */
    return 0;
}