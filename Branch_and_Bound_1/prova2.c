#include <stdio.h>
#include <stdlib.h>
#include "knapsack_0_1_solver_2.h"
#include "knapsack_0_1_solver.h"
#include "extreme_points.h"
#include "boxes.h"


int* volumes;
int n_volumes;

int cont_x;
int cont_y;
int cont_z;

int main(){
    /*
    
    int volumes_2[4] = {1,3,2,4};
    int sol = solve_knapsack_0_1_recursive_v1(volumes_2, 4, 6, 0, 0);
    printf("SOL = %d\n", sol);

    int n = 20;
    int* volumes_3 = malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        volumes_3[i] = 20000000 + rand()% 10000000;//111111 * (i%5) + 1;
        printf("%d \n", volumes_3[i]);
    }
    int capacity = (1000*1000*1000) / 3;// - 150*350*100;//100000000;
    printf("capacity = %d\n", capacity);
    //free(volumes_3);
    //sol = solve_knapsack_0_1_recursive_v1(volumes_3, n, capacity, 0, 0);
    //printf("SOL = %d\n", sol);

    //9:19 - 9:24, optimal solution.
    printf("AAAAA");

    int volumes_4[4] = {1,3,2,4};
    sol = solve_knapsack_0_1_recursive_v2(volumes_4, 4, 9, 3);
    printf("SOL v2 (piccolino) = %d\n", sol);


    



    sol = solve_knapsack_0_1_v2(volumes_3, n, capacity);
    printf("SOL v1 = %d\n", sol);


    //sol = solve_knapsack_0_1_recursive_v2(volumes_3, n, capacity, 5);
    printf("SOL v2 = %d\n", sol);

    


    //si comportano uguali, ma a parità di scatole:
    //il primo vince in modo schiacciante
    //mentre il secondo (recursive) va più lento
    //capacity = 10000000
    //n = 15
    //boxes completely randomly generated

    //if we instead keep the same capacity, have a lot of boxes BUT the number of different
    //volumes among them is low, the second is better.
    //capacity = 100000000
    //n = 25
    //there are only 5 categories of boxes


    //Sì ma wtf quei risultati vengono dalla versione super esponenziale e da quella
    //delle somme corretta, lol what

    */

    point* points = malloc(7*sizeof(point));
    for(int i = 0; i < 7; i++){
        points[i].x = i;
        points[i].y = i;
        points[i].z = i;
        points[i].width = i;
        points[i].height = i;
        points[i].depth = i;
    }
    int indexes[3] = {1, 3, 5};
    exclude_unavailable_points(&points, 7, indexes, 3);
    for(int i = 0; i < 7-3; i++){
        printf("i = %d, %d %d %d %d %d %d\n", i, points[i].x, points[i].y, points[i].z,
            points[i].width, points[i].height, points[i].depth);
    }

}