#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"


int main(){
    char** ciao = malloc(3*sizeof(char*));
    //ciao[0] = malloc(5*sizeof(char));
    ciao[0] = "ciao";
    //ciao[1] = malloc(5*sizeof(char));
    ciao[1] = "come";
    //ciao[2] = malloc(5*sizeof(char));
    ciao[2] = "stai";

    for(int i = 0; i < 3; i++){
        printf("%s ", ciao[i]);
    }

    char** ciao2 = malloc(3*sizeof(char*));
    for(int i = 0; i < 3; i++){
        int x = strlen((ciao[(i + 1) % 3]))+1;
        printf("len x = %d\n", x);
        ciao2[i] = malloc(x * sizeof(char));
    }
    strcpy(ciao2[0], ciao[1]);
    strcpy(ciao2[1], ciao[2]);
    strcpy(ciao2[2], ciao[0]);
    free(ciao);
    ciao = ciao2;
    for(int i = 0; i < 3; i++){
        printf("%s ", ciao[i]);
        free(ciao[i]);
    }
    free(ciao);

    reset_progress_file();
}