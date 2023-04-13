#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Base

noeud * creer_racine(){
    noeud * res = malloc(sizeof (noeud));
    res->est_dossier = true;
    res->nom[0] = '\0';
    res->pere = res;
    res->racine = res;
    res->fils = NULL;
    return res;
}

// fonction ls

void ls(noeud *n){
    if(n->fils != NULL){
        print_fils(n->fils);
    }
}

// fonction cd_chem

noeud * cd_chem(noeud * n, char c[]){
    noeud * res = n;
    const char * sep = "/";
    char * strTok = strtok(c, sep);
    while (strTok != NULL && res->fils != NULL){
        res = find_noeud(res->fils,strTok);
        strTok = strtok(NULL, sep);
    }
    return res;
}

// fonction cd

noeud * cd_racine(noeud * n){
    return n->racine;
}

// fonction cd ..

noeud * cd_pere(noeud * n){
    return n->pere;
}

// fonction pwd

void pwd(noeud * n){
    if (n != n->racine){
        pwd(n->pere);
        printf("/%s",n->nom);
    }
}

// fonction mkdir

void mkdir(noeud * n, char c[]){
    creer_fils(n, c, true);
}

// fonction touch

void touch(noeud * n, char c[]){
    creer_fils(n, c, false);
}

// fonction rm chem


void rm(noeud * n, char c[]){
    char tmp[strlen(c)];
    strcpy(tmp,c);
    noeud * rm = cd_chem(n, tmp);
    rm_cut(rm->pere, substr(c, get_last_slash(c) + 1, strlen(c)));
    rm_no(rm);
}

// fonction cp chem1 chem2

void cp(noeud * n, char c1[], char c2[]){
    noeud * tmp = cd_racine(n);
    noeud * cp = cd_chem(tmp,c1);
    noeud * cl = cd_chem(tmp, substr(c2,0, get_last_slash(c2)));
    mkdir(cl, substr(c2, get_last_slash(c2)+1,strlen(c2)));
    cl = cd_chem(cl, substr(c2, get_last_slash(c2)+1,strlen(c2)));
    cp_no(cl, cp);
}

// fonction mv chem1 chem2

void mv(noeud * n, const char * c1, const char * c2){
    cp(n, c1, c2);
    rm(n, c1);
}

// fonction print

void print(noeud * n){
    print_no(n);
    if(n->fils != NULL) print_succ(n->fils);
}

int main() {

    noeud * n = creer_racine();
    mkdir(n, "Cours");
    n = cd_chem(n,"Cours");
    mkdir(n, "ProjetC");
    mkdir(n, "Anglais");
    n = cd_racine(n);
    touch(n, "edt");
    cp(n,"Cours", "/Td");
    char c[] = "/Td/ProjetC";
    rm(n,c);
    char t[] = "/Td/Anglais";
    rm(n,t);
    n = cd_chem(n,"Td");
    mkdir(n,"td1");
    mkdir(n,"td2");
    char s[] = "/Cours/ProjetC";
    char s2[] = "/CopieProjetC";
    cp(n, s, s2);
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
