#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    int* a = malloc(3*sizeof(int));
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    int* b;
    memcpy(&b, &a, sizeof(a));
    b[0] = 4;
    printf("a[0] = %d\n", a[0]);  //gives 4, [$#17]
    a[0] = 5;
    printf("a[0] = %d\n", b[0]);

    char** strings = malloc(3*sizeof(char*));
    strings[0] = malloc(9*sizeof(char));
    strings[1] = malloc(2*sizeof(char*));
    strings[2] = malloc(5*sizeof(char));
    strcpy(strings[0], "alessioo\0");
    strcpy(strings[1], "a\0");
    strcpy(strings[2], "samu\0");
    for(int i = 0; i < 3; i++){
        printf("string = %s\n", strings[i]);
    }
    
    printf("aaa");
    free(strings[0]);
    free(strings[1]);
    free(strings[2]);
    free(strings);

    strings = malloc(3*sizeof(char*));  //if I comment this it works wtf!?
    strings[0] = "adasdsc";
    strings[1] = "fijvcson";
    strings[2] = "kjn";
    for(int i = 0; i < 3; i++){
        printf("string = %s, len = %ld\n", strings[i], strlen(strings[i]));
    }

}