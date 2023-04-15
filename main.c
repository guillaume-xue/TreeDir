#include "Treedir.h"

int main(){
    racine = creer_racine();
    mkdir(racine, "Cours");
    racine = cd_chem(racine, "Cours");
    mkdir(racine, "ProjetC");
    mkdir(racine, "Anglais");
    racine = cd_racine(racine);
    touch(racine, "edt");
    cp(racine, "Cours", "/Td");
    char s1[] = "/Td/ProjetC";
    rm(racine, s1);
    char s2[] = "/Td/Anglais";
    rm(racine, s2);
    racine = cd_chem(racine, "Td");
    mkdir(racine, "td1");
    mkdir(racine, "td2");
    char s3[] = "/Cours/ProjetC";
    char s4[] = "/CopieProjetC";
    cp(racine, s3, s4);
    racine = cd_racine(racine);
    char s5[] = "/Td";
    char s6[] = "/Cours/Td";
    mv(racine, s5, s6);
    print(racine->racine);


    /*
    noeud * racine = creer_racine();

    mkdir(racine,"Cours");
    mkdir(racine,"Td");
    touch(racine, "edt");

    racine = cd_chem(racine, "Cours");
    mkdir(racine, "ProjetC");
    mkdir(racine, "Anglais");
    racine = cd_pere(racine);
    racine = cd_chem(racine, "Td");
    touch(racine, "td1");
    touch(racine, "td2");
    racine = cd_pere(racine);
    racine = cd_chem(racine, "Cours");
    racine = cd_chem(racine, "ProjetC");

    print(racine->racine);
     */
    return 0;
}