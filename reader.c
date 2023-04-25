#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Treedir.h"

void read_file(FILE * f){
    char * c;
    if (f == NULL) exit(1);
    while(fgets(c, 50, f) != NULL){
        const char * sep = " ";
        char * strTok = NULL;
        strTok = strtok(c, sep);
        while (strTok != NULL){
            printf("%s", strTok);
            strTok = strtok(NULL, sep);
        }
    }
}