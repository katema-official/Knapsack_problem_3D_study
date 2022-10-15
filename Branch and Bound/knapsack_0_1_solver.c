#include "knapsack_0_1_solver.h"
#include <stdlib.h>
#include <stdio.h>


//this version of the knapsack considers that each item has the same value for
//its volume and value
int solve_knapsack_0_1(int* volumes, int n_items, int capacity){
    int** B = (int**) malloc((n_items+1)*sizeof(int*));
    for(int i = 0; i < n_items+1; i++){
        B[i] = (int*) malloc((capacity+1)*sizeof(int));
    }
    
    //initialization: the subproblems without items or capacity have as best solution 0
    for(int i = 0; i < n_items+1; i++){
        B[i][0] = 0;
    }
    for(int i = 0; i < capacity+1; i++){
        B[0][i] = 0;
    }

    //now, the value of each cell of each row can be fully determined by the the previous row
    for(int row = 1; row < n_items + 1; row++){
        int volume_row = volumes[row-1];
        for(int col = 1; col < capacity + 1; col++){
            if(volume_row <= col){  //this item could be part of the solution
                if((volume_row + B[row-1][col - volume_row]) > B[row-1][col]){
                    B[row][col] = volume_row + B[row-1][col - volume_row];
                }else{
                    B[row][col] = B[row-1][col];
                }
            }else{
                B[row][col] = B[row - 1][col];  //thw volume of this item is more than the current capacity
            }
        }
    }

    for(int row = 0; row < n_items + 1; row++){
        for(int col = 0; col < capacity + 1; col++){
            printf("%d ", B[row][col]);
        }
        printf("\n");
    }

    printf("aaa = %d", B[n_items][capacity - 1]);


    for(int i = 0; i < n_items+1; i++){
        free(B[i]);
    }
    free(B);
}