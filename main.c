#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct noeud;
struct liste_noeud;
struct noeud {
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};

struct liste_noeud {
    struct noeud *no;
    struct liste_noeud *succ;
};

typedef struct noeud noeud;

typedef struct liste_noeud liste_noeud;

// Base

noeud * creer_racine(){
    noeud * res = malloc(sizeof (noeud));
    res->est_dossier = true;
    res->nom[100] = '\0';
    res->pere = res;
    res->racine = res;
    res->fils = NULL;
    return res;
}

// fonction ls

void print_fils(liste_noeud *f){
    printf("‰s\n",f->no->nom);
    if(f != NULL){
        print_fils(f->succ);
    }
}

void ls(noeud *n){
    if(n->fils != NULL){
        print_fils(n->fils);
    }
}

// fonction cd_chem

int get_next_slash(int pos, const char * c){
    int i = pos;
    while (c[i] != '/' && c[i] !='\0'){
        i++;
    }
    return i;
}

char * substring(int first, int last, const char * c){
    char * tmp[strlen(c)];
    int j = 0;
    for(int i = first; i <= last; i++){
        tmp[j] = (char *) c[i];
        j++;
    }
    for (int i = 0; tmp[i] != '\0'; i++){
        printf("%c", tmp[i]);
    }
    printf("%d %d %s %s\n",first, last, c, tmp);
    return tmp;
}

noeud * find_noeud(liste_noeud * l, const char * c){
    liste_noeud * tmp = l;
    while (tmp->succ != NULL){
        if (strcmp(tmp->no->nom, c) == 0){
            return tmp->no;
        }
        tmp = tmp->succ;
    }
}

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

// fonction mkdir nom et touch nom

noeud * creer_noeud(noeud * pere, const char * c, bool b){
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

liste_noeud * creer_liste_noeud(noeud * pere, const char * c, bool b){
    liste_noeud * res = malloc(sizeof (liste_noeud));
    res->no = creer_noeud(pere, c, b);
    res->succ = NULL;
    return res;
}

struct liste_noeud * creer_fils(noeud * pere, const char * c, bool b){
    if (pere->fils == NULL){
        pere->fils = creer_liste_noeud(pere, c, b);
    }else{
        liste_noeud * tmp = pere->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = creer_liste_noeud(pere, c, b);
    }
}

void mkdir(noeud * n, const char * c){
    creer_fils(n, c, true);
}

void touch(noeud * n, const char * c){
    creer_fils(n, c, false);
}

// fonction rm chem

void rm_no(noeud * n);

void rm_succ(liste_noeud * n){
    if(n->succ != NULL){
        rm_succ(n->succ);
    }
    rm_no(n->no);
    free(n);
}

void rm_no(noeud * n){
    if(n->fils != NULL){
        rm_succ(n->fils);
    }
    free(n);
}


void rm(noeud * n, const char * c){
    noeud * tmp = cd_chem(n, c);
    rm_no(tmp);
    tmp->fils = NULL;
}

// fonction cp chem1 chem2

int get_last_slash(const char * c){
    int i = strlen(c);
    while (c[i] != '/'){
        i++;
    }
    return i;
}

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

void print_list(liste_noeud * l){
    liste_noeud * tmp = l;
    while (tmp->succ != NULL){
        printf(" %s (%s),", tmp->no->nom, tmp->no->est_dossier ? "D" : "F");
        tmp = tmp->succ;
    }
    printf(" %s (%s)", tmp->no->nom, tmp->no->est_dossier ? "D" : "F");

}

int nb_fils(liste_noeud * l){
    int i = 1;
    liste_noeud * tmp = l;
    while (tmp->succ != NULL){
        i++;
        tmp = tmp->succ;
    }
    return i;
}

void print(noeud * n){
    char * nom = n->nom;
    char * est_dossier =  n->est_dossier ? "D" : "F";
    char * pere = strcmp("", n->pere) ? "/" : n->pere->nom;
    int nb = nb_fils(n->fils);
    if (n->pere == n){
        printf("Noeud / (%s), %d fils :", est_dossier, nb);
    }else{
        printf("Noeud %s (%s), pere : %s, %d fils :", nom, est_dossier, pere, nb);
    }
    print_list(n->fils);
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
    print(n);
    return 0;
}
