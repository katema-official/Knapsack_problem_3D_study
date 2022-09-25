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

}