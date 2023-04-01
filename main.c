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

noeud * creer_racine(){
    noeud * res = malloc(sizeof (noeud));
    res->est_dossier = true;
    res->nom[0] = '\0';
    res->pere = res;
    res->racine = res;
    res->fils = NULL;
    return res;
}

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

int get_next_slash(int pos, const char * c){
    int i = pos;
    while (c[i] != '/' && c[i] !='\0'){
        i++;
    }
    return i;
}

char * get_next_name_noeud(int fist, int last, const char * c){
    char * tmp[strlen(c)];
    for(int i=fist; i<=last; i++){
        tmp[i] = (char *) c[i];
    }
    return tmp;
}

noeud * find_noeud(liste_noeud * l, const char * c){
    assert(l == NULL);
    if (strcmp(l->no->nom, c)){
        return l->no;
    }else{
        find_noeud(l->succ, c);
    }
}

noeud * cd_chem(noeud * n, const char * c){
    assert(c[0] == '/');
    noeud * res = n;
    char * tmp;
    int i = 0, j;
    while (c[i] != '\0'){
        assert(res->fils != NULL);
        j = i;
        i = get_next_slash(j+1, c);
        tmp = get_next_name_noeud(j, i, c);
        res = find_noeud(res->fils,tmp);
    }
    return res;
}

noeud * cd_racine(noeud * n){
    return n->racine;
}

noeud * cd_pere(noeud * n){
    return n->pere;
}

void pwd(noeud * n){
    if (n != n->racine){
        pwd(n->pere);
        printf("/%s",n->nom);
    }
}




int main() {

    return 0;
}
