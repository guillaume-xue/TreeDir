#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Treedir.h"

char * dupliquer_char_s(const char * s){
    char *res = malloc(strlen(s) * sizeof (char));
    for(size_t i=0;i<strlen(s);i++){
        res[i] = s[i];
    }
    return res;
}

void make_cmd(char * cmd, char * chem1, char * chem2){
    if(chem1 == NULL && chem2 == NULL){
        if (strcmp(cmd, "cd") == 0){
            cd_racine();
        } else if (strcmp(cmd, "ls") == 0){
            ls();
        } else if (strcmp(cmd, "pwd") == 0){
            pwd();
        }else{
            printf("Commande non trouver.\n");
        }
    }else if(chem2 == NULL){
        if (strcmp(cmd, "cd") == 0){
            if (strcmp(chem1, "..") == 0) cd_pere();
            else cd_chem(chem1);
        } else if (strcmp(cmd, "mkdir") == 0){
            mkdir(chem1);
        } else if (strcmp(cmd, "touch") == 0){
            touch(chem1);
        }else if (strcmp(cmd, "rm") == 0){
            rm(chem1);
        }else{
            printf("Commande non trouver.\n");
        }
    }else{
        if (strcmp(cmd, "cp") == 0){
            cp(chem1,chem2);
        } else if (strcmp(cmd, "mv") == 0){
            mv(chem1, chem2);
        } else {
            printf("Commande non trouver.\n");
        }
    }
}

void read_file(FILE * f){
    char * c = malloc(sizeof(char));
    while(fgets(c, 50, f) != NULL){

        int count = 0;
        char * cmd;
        char * chem1 = NULL;
        char * chem2 = NULL;

        const char * sep = " \n";
        char * strTok = NULL;
        strTok = strtok(c, sep);

        cmd = dupliquer_char_s(strTok);
        strTok = strtok(NULL, sep);

        while (strTok != NULL){
            if (count == 0) chem1 = dupliquer_char_s(strTok);
            if (count == 1) chem2 = dupliquer_char_s(strTok);
            strTok = strtok(NULL, sep);
            count++;
        }

        make_cmd(cmd, chem1, chem2);
    }
}