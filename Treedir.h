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

extern void creer_racine();
extern void ls();
extern void cd_chem(char * c);
extern void cd_racine();
extern void cd_pere();
extern void pwd();
extern void mkdir(char * c);
extern void touch(char * c);
extern void rm(char * c);
extern void cp(char * c1, char * c2);
extern void mv(char * c1, char * c2);
extern void print(noeud * n);

extern void read_file(FILE * f);
extern char * dupliquer_char_s(const char * s);

#endif