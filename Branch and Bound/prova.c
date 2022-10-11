#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define n 3

int function_1(int* a){
    a[0] = 3;
}


int main(){
    int* a = (int*) malloc (n*sizeof(int));
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    for(int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    function_1(a);
    for(int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }


}






