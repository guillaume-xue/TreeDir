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

// fonction ls

void print_fils(liste_noeud *f){
    printf("‰s\n",f->no->nom);
    if(f != NULL){
        print_fils(f->succ);
    }
}

// fonction cd_chem

bool str_equals(const char * c1, const char * c2){
    if (strlen(c1) != strlen(c2)) return false;
    for (int i = 0; c1[i] != '\0'; ++i) {
        if(c1[i] != c2[i]) return false;
    }
    return true;
}

noeud * find_noeud(liste_noeud * l, char c[]){
    if (strcmp(l->no->nom, c) == 0){
        return l->no;
    }
    if (l != NULL) return find_noeud(l->succ, c);
}

// fonction mkdir nom et touch nom

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
        liste_noeud * tmp = pere->fils;
        while (tmp->succ != NULL){
            tmp = tmp->succ;
        }
        tmp->succ = creer_liste_noeud(pere, c, b);
    }
}

// fonction rm chem

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

// fonction cp chem1 chem2

void mkdir(noeud * n, char c[]);
void touch(noeud * n, char c[]);
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

int get_last_slash(char c[]){
    int i = strlen(c);
    while (c[i] != '/'){
        i--;
    }
    return i;
}

char *substr(char src[],int pos,int len) {
    char *dest = (char *) malloc(len+1);
    strncat(dest,src+pos,len);
    return dest;
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

