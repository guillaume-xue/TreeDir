#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
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

noeud * cd_chem(noeud * n, const char * c){
    noeud * res = n;
    const char * sep = "/";
    char * strTok = strtok(c, sep);
    while (strTok != NULL){
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

void mkdir(noeud * n, const char * c){
    creer_fils(n, c, true);
}

// fonction touch

void touch(noeud * n, const char * c){
    creer_fils(n, c, false);
}

// fonction rm chem


void rm(noeud * n, const char * c){
    noeud * tmp = cd_chem(n, c);
    rm_no(tmp);
    tmp->fils = NULL;
}

// fonction cp chem1 chem2

void cp(noeud * n, const char * c1, const char * c2){
    noeud * copy = cd_chem(n, c1);
    noeud * res = cd_chem(n, substring(0, get_last_slash(c2),c2));
    mkdir(res, substring(get_last_slash(c2),strlen(c2),c2));
    res = cd_chem(n, c2);
    res->fils = malloc(sizeof (liste_noeud));
    memcpy(res->fils, copy->fils, sizeof(liste_noeud));
}

// fonction mv chem1 chem2

void mv(noeud * n, const char * c1, const char * c2){
    cp(n, c1, c2);
    rm(n, c1);
}

// fonction print

void print(noeud * n){
    print_noeud(n);
    if(n->fils != NULL) print_succ(n->fils);
}

int main() {

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
    return 0;
}
