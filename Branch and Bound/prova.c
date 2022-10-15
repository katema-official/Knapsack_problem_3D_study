#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "extreme_points.h"
#include "boxes.h"
#include "knapsack_0_1_solver.h"
#include "utils.h"

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
    free(a);

    point* points = malloc(n*sizeof(point));

    points[0].x = 1;
    printf("points[0] = %d\n", points[0].x);

    free(points);
    box* boxes;

    int volumes[3] = {2,4,6};
    solve_knapsack_0_1(volumes, 3, 8);


}






