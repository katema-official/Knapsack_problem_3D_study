#include <stdio.h>
#include <stdlib.h>
#include "knapsack_0_1_solver_2.h"


int main(){
    int volumes_2[4] = {1,3,2,4};
    int sol = solve_knapsack_0_1_recursive_v1(volumes_2, 4, 6, 0, 0);
    printf("SOL = %d\n", sol);

    int n = 30;
    int* volumes_3 = malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        volumes_3[i] = i;
    }
    int capacity = 10000;
    free(volumes_3);
    //sol = solve_knapsack_0_1_recursive_v1(volumes_3, n, capacity, 0, 0);
    //printf("SOL = %d\n", sol);

    //9:19 - 9:24, optimal solution.


    int volumes_4[4] = {1,3,2,4};
    //sol = solve_knapsack_0_1_recursive_v2(volumes_4, 4, 9, 3);
    //printf("SOL v2 (piccolino) = %d\n", sol);


    sol = solve_knapsack_0_1_recursive_v2(volumes_3, n, capacity, 3);
    printf("SOL v2 = %d\n", sol);

}