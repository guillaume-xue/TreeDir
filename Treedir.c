#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Treedir.h"

// Test validité du chemin

bool test_validite_chemin(char c[]){
    //Verifie que la chaine n'est pas vide, ne commence pas par "/" et ne termine pas par "/"
    if(c[0] == '\0' || c[0] == "/" || c[(int)(sizeof(c)/sizeof(c[0]))-1] == "/"){
        return false;
    }
    return true;
}

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

void print_fils(liste_noeud *f){
    printf("‰s\n",f->no->nom);
    if(f->succ != NULL){
        print_fils(f->succ);
    }
}

void ls(noeud *n){
    if(n->fils != NULL){
        print_fils(n->fils);
    }
}

// fonction cd_chem

noeud * find_noeud(liste_noeud * l, char c[]){
    if (strcmp(l->no->nom, c) == 0){
        return l->no;
    }
    if (l != NULL) return find_noeud(l->succ, c);
}

noeud * cd_chem(noeud * n, char c[]){
    assert(test_validite_chemin(c) && "Chemin avec un format non autorise.");
    noeud * res = NULL;
    const char * sep = "/";
    char * strTok = NULL;
    if(c[0] == "/"){
        res = n->racine;
        strTok = strtok(c+1, sep);
    }else{
        res = n;
        strTok = strtok(c, sep);
    }
    while (strTok != NULL && res->fils != NULL){
        res = find_noeud(res->fils,strTok);
        strTok = strtok(NULL, sep);
    }
    if(strTok != NULL && res->fils == NULL){
        printf("Erreur : Le chemin indique n existe pas.");
        return NULL;
    }
    if(strTok == NULL && !res->est_dossier){
        printf("Erreur : Le chemin indique n est pas un dossier.");
        return NULL;
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

void pwd(noeud * n){// affiche juste "/" si c'est la racine
    if(n == n->racine){
        print("/");
    }else{
        pwd_fils(n);
    }
}

void pwd_fils(noeud * n){
    if (n != n->racine){
        pwd_fils(n->pere);
        printf("/%s",n->nom);
    }
}

// Verifie s'il y'a un duplicata de nom

bool verif_existe_dupli(noeud * n, char c[]){
    liste_noeud * tmp = n->fils;
    while (tmp != NULL)
    {
        if(strcmp(tmp->no->nom, c) == 0){
            return true;
        }
        tmp = tmp->succ;
    }
    return false;
}

// fonction mkdir et touch

noeud * creer_noeud(noeud * pere, char c[], bool b){
    noeud * res = malloc(sizeof (noeud));
    res->est_dossier = b;
    for (int i = 0; c[i] != '\0'; ++i) {
        res->nom[i] = c[i];
    }
    res->pere = pere;
    res->racine = pere->racine;
    res->fils = NULL;
    return res;
}

liste_noeud * creer_liste_noeud(noeud * pere, char c[], bool b){
    liste_noeud * res = malloc(sizeof (liste_noeud));
    res->no = creer_noeud(pere, c, b);
    res->succ = NULL;
    return res;
}

struct liste_noeud * creer_fils(noeud * pere, char c[], bool b){
    if (pere->fils == NULL){
        pere->fils = creer_liste_noeud(pere, c, b);
    }else{
        assert(!verif_existe_dupli(pere, c) && "Il est existe deja un dossier ou fichier avec ce nom.");
        liste_noeud * tmp = pere->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = creer_liste_noeud(pere, c, b);
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

char *substr(char src[],int pos,int len) {
    char *dest = (char *) malloc(len+1);
    strncat(dest,src+pos,len);
    return dest;
}

int get_last_slash(char c[]){
    int i = strlen(c);
    while (c[i] != '/' && i != 0){
        i--;
    }
    return i;
}

void rm_cut(noeud * n, char c[]){
    if(strcmp(n->fils->no->nom, c) == 0) {
        n->fils = n->fils->succ;
    } else{
        liste_noeud * tmp = n->fils;
        while (strcmp(tmp->succ->no->nom,c) != 0 && tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = tmp->succ->succ;
    }
}

void rm_no(noeud * n);

void rm_succ(liste_noeud * l){
    if(l != NULL){
        rm_succ(l->succ);
        rm_no(l->no);
        free(l);
    }
}

void rm_no(noeud * n){
    if(n->fils != NULL){
        rm_succ(n->fils);
    }
    free(n);
}

void rm(noeud * n, char c[]){
    char tmp[strlen(c)];
    strcpy(tmp,c);
    noeud * rm = cd_chem(n, tmp);
    assert(rm != NULL && "Chemin NULL");
    if(rm != n){
        assert(verif_arbo(rm, n));
    }
    rm_cut(rm->pere, substr(c, get_last_slash(c) + 1, strlen(c)));
    rm_no(rm);
}

// Verifie si deux arborescence ses superpose

bool verif_arbo(noeud * n_a_supp, noeud * n_actuel){
    assert((n_a_supp != NULL) && "Impossible de supprimer la racine.");
    noeud * tmp = n_actuel;
    while (tmp != tmp->racine)
    {
        assert(n_a_supp != tmp && "");
        tmp = tmp->pere
    }
    return true;

}

// fonction cp chem1 chem2

void cp_no(noeud * n1, noeud * n2);

void cp_succ(noeud * n, liste_noeud * l){
    if (l->no->est_dossier){
        mkdir(n,l->no->nom);
    }else{
        touch(n,l->no->nom);
    }
    if(l->succ != NULL){
        cp_succ(n,l->succ);
        cp_no(n->fils->no,l->no);
    }
}

void cp_no(noeud * n1, noeud * n2){
    if(n2->fils != NULL){
        cp_succ(n1, n2->fils);
    }
}

void cp(noeud * n, char c1[], char c2[]){
    noeud * cp = cd_chem(n->racine,c1);
    noeud * cl = cd_chem(n, substr(c2,0, get_last_slash(c2)));
    mkdir(cl, substr(c2, get_last_slash(c2) + 1, strlen(c2)));
    cl = cd_chem(cl, substr(c2, get_last_slash(c2) + 1,strlen(c2)));
    cp_no(cl, cp);
}

// fonction mv chem1 chem2

void mv(noeud * n, const char * c1, const char * c2){
    cp(n, c1, c2);
    rm(n, c1);
}

// fonction print

int nb_fils(liste_noeud * l){
    if (l == NULL) return 0;
    return 1 + nb_fils(l->succ);
}

void print_list(liste_noeud * l){
    if (l != NULL){
        printf(" %s (%s)", l->no->nom, l->no->est_dossier ? "D" : "F");
        if(l->succ != NULL) printf(",");
        print_list(l->succ);
    }
}

void print(noeud * n);

void print_succ(liste_noeud * l){
    print(l->no);
    if (l->succ != NULL) print_succ(l->succ);
}

void print_no(noeud * n){
    char * nom = n->nom;
    char * est_dossier =  n->est_dossier ? "D" : "F";
    char * pere = (n->pere->nom[0] == '\0') ? "/" : n->pere->nom;
    int nb = nb_fils(n->fils);
    if (n->nom[0] == '\0'){
        printf("Noeud / (%s), %d fils ", est_dossier, nb);
    }else{
        printf("Noeud %s (%s), pere : %s, %d fils ", nom, est_dossier, pere, nb);
    }
    if(nb != 0) printf(":");
    print_list(n->fils);
    printf("\n");
}

void print(noeud * n){
    print_no(n);
    if(n->fils != NULL) print_succ(n->fils);
}