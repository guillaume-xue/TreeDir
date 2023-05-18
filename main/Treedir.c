#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Treedir.h"

// Variable globale

noeud * n = NULL;

// Test validité du chemin

bool verif_last_case(char * c){
    for (char * i = c; *i != '\0'; i++){
        if(*i == '/' && *(i+1) == '\0'){
            return true;
        }
    }
    return false;
}

bool test_validite_chemin(char * c){
    //Verifie que la chaine n'est pas vide, ne commence pas par "/" et ne termine pas par "/"
    if(*c == '\0' || verif_last_case(c)){
        //printf("test %s \n", *c);
        return false;
    }
    return true;
}

// Base

void creer_racine(){
    n = calloc(1, sizeof (noeud));
    n->est_dossier = true;
    n->nom[0] = '\0';
    n->pere = n;
    n->racine = n;
    n->fils = NULL;
}

// fonction ls

void print_fils(liste_noeud * f){
    printf("%s\n", f->no->nom);
    if(f->succ != NULL) print_fils(f->succ);
}

void ls(){
    if(n->fils != NULL) print_fils(n->fils);
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

noeud * find_noeud(liste_noeud * l, char * c){
    if (strcmp(l->no->nom, c) == 0) return l->no;
    if (l->succ != NULL) return find_noeud(l->succ, c);
    return NULL;
}

void cd_chem(char * c){
    assert(test_validite_chemin(c) && "Chemin avec un format non autorise.");

    char * str = malloc(strlen(c) * sizeof (char) + 1);
    memmove(str, c, strlen(c) * sizeof (char) + 1);
    noeud * curr = NULL;
    const char * sep = "/";
    char * strTok = NULL;

    if (*str == *sep) cd_racine();    // si chemin absolut

    curr = n;
    strTok = strtok(str, sep);
    while (strTok != NULL && curr->fils != NULL){
        curr = find_noeud(curr->fils, strTok);
        strTok = strtok(NULL, sep);
    }
    free(str);
    if(strTok != NULL && curr->fils == NULL) assert(false && "Erreur : Le chemin indique n'existe pas.");
    if(strTok == NULL && !curr->est_dossier) assert(false && "Erreur : Le chemin indique n'est pas un dossier.");
    n = curr;
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
    printf("\n");
}

// Verifie s'il y'a un duplicata de nom

bool verif_existe_dupli(noeud * n, char * c){
    liste_noeud * tmp = n->fils;
    while (tmp != NULL){
        if(strcmp(tmp->no->nom, c) == 0){
            return true;
        }
        tmp = tmp->succ;
    }
    return false;
}

// fonction auxilière de mkdir et touch

noeud * creer_noeud(noeud * pere, char * c, bool b){
    noeud * res = calloc(1, sizeof (noeud));
    res->est_dossier = b;
    for (int i = 0; c[i] != '\0'; ++i) {
        res->nom[i] = c[i];
    }
    res->pere = pere;
    res->racine = pere->racine;
    res->fils = NULL;
    return res;
}

liste_noeud * creer_list_noeud(noeud * pere, char * c, bool b){
    liste_noeud * res = calloc(1, sizeof (liste_noeud));
    res->no = creer_noeud(pere, c, b);
    res->succ = NULL;
    return res;
}

void creer_fils(noeud * no, char * c, bool b){
    if (no->fils == NULL){
        no->fils = creer_list_noeud(no, c, b);
    }else{
        assert(!verif_existe_dupli(no, c) && "Il est existe deja un dossier ou fichier avec ce nom.");
        liste_noeud * tmp = no->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = creer_list_noeud(no, c, b);
    }
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

bool have_slash(char * c){
    for (char* tmp = c; *tmp != '\0' ; tmp++){
        if(*tmp == '/') return true;
    }
    return false;
}

bool verif_arbo(noeud * n_a_supp, noeud * n_actuel){    // Verifie si deux arborescence se superpose
    noeud * tmp = n_a_supp;
    while (tmp != tmp->racine){
        assert(n_actuel != tmp && "Le chemin actuel se superpose avec le chemin à copier/supprimer");
        tmp = tmp->pere;
    }
    return true;
}

int get_last_slash(char * c){
    int pos = strlen(c);
    while (c[pos] != '/' && pos != 0){
        pos--;
    }
    return pos;
}

char *get_last_no_name(char * c){
    char * name = malloc(strlen(c) * sizeof (c) + 1);
    size_t length = strlen(c);
    size_t start = get_last_slash(c) + 1;
    if (have_slash(c) == 0) start--;
    size_t cutLength = length - start;
    memmove(name, c + start, cutLength + 1);
    return name;
}

void rm_no(noeud * no);

void rm_cut(noeud * no, char * c){
    if(strcmp(no->fils->no->nom, c) == 0) {
        liste_noeud * tmp = no->fils;
        no->fils = no->fils->succ;
        rm_no(tmp->no);
        free(tmp);
    }else{
        liste_noeud * tmp = no->fils;
        char * wait = dupliquer_char_s(tmp->succ->no->nom);
        while (strcmp(wait, c) != 0 && tmp->succ != NULL){
            tmp = tmp->succ;
            free(wait);
            wait = dupliquer_char_s(tmp->succ->no->nom);
        }
        free(wait);
        liste_noeud * tmpln = tmp->succ;
        tmp->succ = tmp->succ->succ;
        rm_no(tmpln->no);
        free(tmpln);
    }
}

void rm_succ(liste_noeud * l){
    if(l != NULL){
        rm_succ(l->succ);
        rm_no(l->no);
        free(l);
    }
}

void rm_no(noeud * no){
    if(no->fils != NULL) rm_succ(no->fils);
    free(no);
}

void rm(char * c){
    assert(test_validite_chemin(c) && "Chemin avec un format non autorise.");

    noeud * curr = n;    // noeud de départ
    cd_chem(c);
    noeud * rm = n;     // noeud qui précède le fichier/dossier à supprimer

    if(curr == rm) assert(verif_arbo(curr, rm) == 0 && "Chemin avec un format non autorise.");

    char * rm_name = get_last_no_name(c);
    rm_cut(rm->pere, rm_name);
    free(rm_name);
    n = curr;
}

// fonction cp chem1 chem2

char *substr(char * src, int pos, int len) {
    if(len > 0){
        char *dest = calloc(len+1, sizeof(char));
        strncat(dest,src+pos,len);
        return dest;
    }
    return NULL;
}

void cp(char * c1, char * c2){
    assert(test_validite_chemin(c1) && "Chemin avec un format non autorise. (cp c1)");
    assert(test_validite_chemin(c2) && "Chemin avec un format non autorise. (cp c2)");

    noeud * curr = n;
    char * name = get_last_no_name(c2);
    cd_chem(c1);
    
    noeud * cp = n;
    n = curr;
    assert(verif_arbo(curr, cp));
    char * chem2;
    chem2 = dupliquer_char_s(c2);

    if(*chem2 == '/'){
        cd_racine();
        free(chem2);
        if(get_last_slash(c2) == 0){
            chem2 = NULL;
        }else{
            chem2 = substr(c2, 1, get_last_slash(c2));
        }
    }else{
        free(chem2);
        chem2 = substr(c2, 0, get_last_slash(c2));
    }

    if(chem2 != NULL) cd_chem(chem2);

    mkdir(name);
    cd_chem(name);
    
    noeud * cl = n;
    memmove(cl, cp, sizeof (noeud));
    n = curr;

    free(chem2);
    free(name);
}

void change_adresse_remove_old(noeud * n1){
    noeud * tmp = n1->pere;
    liste_noeud * tmpln = tmp->fils;
    if(tmpln->no == n1){
        tmp->fils = tmpln->succ;
        free(tmpln);
    }else{
        while(tmpln->succ->no != n1){
            tmpln = tmpln->succ;
        }
        liste_noeud * tmplnbis = tmpln->succ;
        tmpln->succ = tmplnbis->succ;
        free(tmplnbis);
    }
}

void change_adresse_add_new(noeud * n1, noeud * n2){
    assert(!verif_existe_dupli(n2, n1->nom) && "Il est existe deja un dossier ou fichier avec ce nom. (mv)");
    liste_noeud * ln = calloc(1, sizeof(liste_noeud));
    ln->no = n1;
    ln->succ = NULL;
    if(n2->fils == NULL){
        n2->fils = ln;
    }else{
        liste_noeud * tmp = n2->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = ln;
    }
    change_adresse_remove_old(n1);
}

// fonction mv chem1 chem2

void mv(char * c1, char * c2){
    assert(test_validite_chemin(c1) && "Chemin avec un format non autorise. (mv c1)");
    assert(test_validite_chemin(c2) && "Chemin avec un format non autorise. (mv c2)");
    const char * sep = "/";
    char * strTok = NULL;
    noeud * res1 = NULL;
    noeud * res2 = NULL;
    res1 = n;
    res2 = n;

    char * chem1 = dupliquer_char_s(c1);
    char * chem2 = dupliquer_char_s(c2);
    if (*chem1 == *sep) res1 = n->racine;
    if (*chem2 == *sep) res2 = n->racine;
    free(chem1);
    free(chem2);

    strTok = strtok(c1, sep);
    while (strTok != NULL && res1->fils != NULL){
        res1 = find_noeud(res1->fils, strTok);
        assert(res1 != NULL && "Ce chemin est innacessible depuis l'emplacement actuel (mv1)");
        strTok = strtok(NULL, sep);
    }
    strTok = strtok(c2, sep);
    while (strTok != NULL && res2->fils != NULL){
        res2 = find_noeud(res2->fils,strTok);
        assert(res2 != NULL && "Ce chemin est innacessible depuis l'emplacement actuel (mv2)");
        strTok = strtok(NULL, sep);
    }
    change_adresse_add_new(res1, res2);
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

void print(noeud * n);

void print_succ(liste_noeud * l){
    print(l->no);
    if (l->succ != NULL) print_succ(l->succ);
}

void print(noeud * n){
    print_no(n);
    if (n->fils != NULL) print_succ(n->fils);
}

void print1(){
    printf("\n");
    print(n->racine);
}

void print_mode_dossier(liste_noeud * tmp, int x);
void print_mode_arbre(noeud * n, int x);
void print_mode_fichier(noeud * n, int x);

void print_mode_arbre(noeud * n, int x){
    print_mode_fichier(n, x);
    if(n->fils != NULL){
        print_mode_dossier(n->fils, x);
    }
}

void print_mode_dossier(liste_noeud * tmp, int x){
    print_mode_arbre(tmp->no, x+1);
    if(tmp->succ != NULL) print_mode_dossier(tmp->succ, x);
}

void print_mode_fichier(noeud * n, int x){
    for (size_t i = 0; i < x; i++)
    {
        printf("--");
    }
    
    printf("%s", n->nom);
    printf("\n");
}

void fermeture(noeud * n);
void fermetureBis(liste_noeud * n);

void fermeture(noeud * n){
    if(n->fils != NULL){
        fermetureBis(n->fils);
        free(n->fils);
    }
    free(n);
}

void fermetureBis(liste_noeud * n){
    if(n->succ != NULL){
        fermetureBis(n->succ);
        free(n->succ);
    }
    fermeture(n->no);
    
}