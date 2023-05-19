#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Treedir.h"

char * dupliquer_char_s(const char * s){
    char* res = (char*)calloc(strlen(s)+1, sizeof(char));
    strncpy(res, s, strlen(s) + 1);
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
        } else if (strcmp(cmd, "printAux") == 0){
            print();
        }else{
            assert(false && "Commande non trouver.");
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
            assert(false && "Commande non trouver.");
        }
    }else{
        if (strcmp(cmd, "cp") == 0){
            cp(chem1,chem2);
        } else if (strcmp(cmd, "mv") == 0){
            mv(chem1, chem2);
        } else {
            assert(false && "Commande non trouver.");
        }
    }

}

void read_file(FILE * f){
    char* c = (char*)calloc(100, sizeof(char));
    const char * sep = " \n";
    char* cBis = fgets(c, 100, f);
    while(cBis != NULL){
        if(*c != '\n' && *c != '\0'){
            int count = 0;
            char * cmd = NULL;
            char * chem1 = NULL;
            char * chem2 = NULL;
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
            free(chem1);
            free(chem2);
            free(cmd);
            free(strTok);
        }
        cBis = fgets(c, 100, f);
    }
    free(c);
    free(cBis);
}