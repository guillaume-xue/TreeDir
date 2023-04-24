#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Treedir.h"

// Variable globale

char * dupliquer_char(const char * s){
    char *res = malloc(strlen(s) * sizeof (char));
    for(size_t i=0;i<strlen(s);i++){
        res[i] = s[i];
    }
    return res;
}

noeud * n = NULL;

// Test validité du chemin

bool test_validite_chemin(char * c){
    //Verifie que la chaine n'est pas vide, ne commence pas par "/" et ne termine pas par "/"
    if(c[0] == '\0' || (c[0] == '/' && c[1] == '\0')|| c[strlen(c)] == '/'){
        return false;
    }
    return true;
}

// Base

void creer_racine(){
    n = malloc(sizeof (noeud));
    n->est_dossier = true;
    n->nom[0] = '\0';
    n->pere = n;
    n->racine = n;
    n->fils = NULL;
}

// fonction ls

void print_fils(liste_noeud * f){
    printf("%s\n", f->no->nom);
    if(f->succ != NULL){
        print_fils(f->succ);
    }
}

void ls(){
    if(n->fils != NULL){
        print_fils(n->fils);
    }
}

// fonction cd

void cd_racine(){
    n = n->racine;
}

// fonction cd ..

void cd_pere(){
    n = n->pere;
}

// fonction cd_chem

char *substr(char * src, int pos, int len) {
    char *dest = (char *) malloc(len+1);
    strncat(dest,src+pos,len);
    return dest;
}

noeud * find_noeud(liste_noeud * l, char * c){
    if (strcmp(l->no->nom, c) == 0){
        return l->no;
    }
    if (l != NULL) return find_noeud(l->succ, c);
    return NULL;
}

void cd_chem(char * c){
    //assert(test_validite_chemin(c) && "Chemin avec un format non autorise.");
    noeud * res = NULL;
    const char * sep = "/";
    char * strTok = NULL;
    if (strcmp(substr(c,0,1), sep) == 0){
        cd_racine();
    }
    res = n;
    strTok = strtok(c, sep);
    while (strTok != NULL && res->fils != NULL){
        res = find_noeud(res->fils,strTok);
        strTok = strtok(NULL, sep);
    }
    if(strTok != NULL && res->fils == NULL){
        printf("Erreur : Le chemin indique n existe pas. ");
    }
    if(strTok == NULL && !res->est_dossier){
        printf("Erreur : Le chemin indique n est pas un dossier.");
    }
    n = res;
}

// fonction pwd

void pwd_fils(noeud * n){
    if (n != n->racine){
        pwd_fils(n->pere);
        printf("/%s",n->nom);
    }
}

void pwd(){
    if(n == n->racine) printf("/"); // affiche juste "/" si c'est la racine
    pwd_fils(n);
}

// Verifie s'il y'a un duplicata de nom

bool verif_existe_dupli(noeud * n, char * c){
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

// fonction auxilière de mkdir et touch

noeud * creer_noeud(noeud * pere, char * c, bool b){
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

liste_noeud * creer_liste_noeud(noeud * pere, char * c, bool b){
    liste_noeud * res = malloc(sizeof (liste_noeud));
    res->no = creer_noeud(pere, c, b);
    res->succ = NULL;
    return res;
}

liste_noeud * creer_fils(noeud * no, char * c, bool b){
    if (no->fils == NULL){
        no->fils = creer_liste_noeud(no, c, b);
    }else{
        assert(!verif_existe_dupli(no, c) && "Il est existe deja un dossier ou fichier avec ce nom.");
        liste_noeud * tmp = no->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = creer_liste_noeud(no, c, b);
    }
    return NULL;
}

// fonction mkdir

void mkdir(char * c){
    creer_fils(n, c, true);
}

// fonction touch

void touch(char * c){
    creer_fils(n, c, false);
}

// fonction rm chem

int get_last_slash(char * c){
    int i = strlen(c);
    while (c[i] != '/' && i != 0){
        i--;
    }
    return i;
}

void rm_cut(noeud * no, char * c){
    if(strcmp(no->fils->no->nom, c) == 0) {
        no->fils = no->fils->succ;
    }else{
        liste_noeud * tmp = no->fils;
        while (strcmp(dupliquer_char(tmp->succ->no->nom), c) != 0 && tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = tmp->succ->succ;
    }
}

void rm_no(noeud * no);

void rm_succ(liste_noeud * l){
    if(l != NULL){
        rm_succ(l->succ);
        rm_no(l->no);
        free(l);
    }
}

void rm_no(noeud * no){
    if(no->fils != NULL){
        rm_succ(no->fils);
    }
    free(no);
}

bool verif_arbo(noeud * n_a_supp, noeud * n_actuel);

void rm(char * c){
    noeud * tmp = n;
    cd_chem(dupliquer_char(c));
    noeud * rm = n;
    assert(rm != NULL && "Chemin NULL");
    if(rm != tmp){
        if(verif_arbo(tmp, rm)){
            rm_cut(rm->pere, substr(c, get_last_slash(c) + 1, strlen(c)));
            rm_no(rm);
        }
    }
    n = tmp;
}

// Verifie si deux arborescence ses superpose

bool verif_arbo(noeud * n_a_supp, noeud * n_actuel){
    assert((n_a_supp != NULL) && "Impossible de supprimer la racine.");
    noeud * tmp = n_actuel;
    while (tmp != tmp->racine)
    {
        assert(n_a_supp != tmp && "");
        tmp = tmp->pere;
    }
    return true;

}

// fonction cp chem1 chem2

void cp_no(noeud * n1, noeud * n2);

void cp_succ(noeud * n, liste_noeud * l){
    if (l->no->est_dossier){
        mkdir(l->no->nom);
    }else{
        touch(l->no->nom);
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

void cp(char * c1, char * c2){
    noeud * tmp = n;
    cd_racine();
    cd_chem(dupliquer_char(c1));
    noeud * cp = n;
    n = tmp;
    cd_chem(substr(c2,0, get_last_slash(c2)));
    if (strcmp(substr(c2, 0, 1), "/") == 0){
        mkdir(substr(c2, get_last_slash(c2) + 1, strlen(c2)));
        cd_chem(substr(c2, get_last_slash(c2) + 1,strlen(c2)));
    }else{
        mkdir(substr(c2, get_last_slash(c2), strlen(c2)));
        cd_chem(substr(c2, get_last_slash(c2),strlen(c2)));
    }
    noeud * cl = n;
    cp_no(cl, cp);
    n = tmp;
}

// fonction mv chem1 chem2

void mv(char * c1, char * c2){
    cp(c1, c2);
    rm(c1);
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