#ifndef LISTE_H
#define LISTE_H
#include <stdbool.h>


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

extern noeud * n;

extern noeud * creer_racine();
extern void ls(noeud *n);
extern noeud * cd_chem(noeud * n, char c[]);
extern noeud * cd_racine(noeud * n);
extern noeud * cd_pere(noeud * n);
extern void pwd(noeud * n);
extern void mkdir(noeud * n, char c[]);
extern void touch(noeud * n, char c[]);
extern void rm(noeud * n, char c[]);
extern void cp(noeud * n, char c1[], char c2[]);
extern void mv(noeud * n, char c1[], char c2[]);
extern void print(noeud * n);

#endif