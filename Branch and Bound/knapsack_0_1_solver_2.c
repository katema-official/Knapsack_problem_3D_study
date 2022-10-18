#include "utils.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "knapsack_0_1_solver_2.h"


int solve_knapsack_0_1_recursive_v1(int* volumes, int len, int max_capacity, int index, int current_volume_occupied){
    if(index >= len || current_volume_occupied == max_capacity){
        return current_volume_occupied;
    }
    printf("index = %d, current_volume_occupied = %d\n", index, current_volume_occupied);

    int current_volume = volumes[index];

    int v1 = current_volume_occupied + 0;
    int v2 = current_volume_occupied + current_volume;

    int sol1 = 0;
    int sol2 = 0;

    if(v1 <= max_capacity){
        sol1 = solve_knapsack_0_1_recursive_v1(volumes, len, max_capacity, index + 1, v1);
    }

    if(v2 <= max_capacity){
        sol2 = solve_knapsack_0_1_recursive_v1(volumes, len, max_capacity, index + 1, v2);
    }

    return max(current_volume_occupied, max(sol1, sol2));
}


//----------------------------ANOTHER APPROACH-------------------------------
int solve_knapsack_0_1_recursive_v2(int* volumes, int len, int max_capacity, int max_depth){
    int dim = min(max_capacity + 1, pow(2, len));
    int* partial_solutions = malloc(dim * sizeof(int));
    if(partial_solutions == NULL){
        printf("my final message. save da world. goodbye\n");
        exit(-1);
    }
    partial_solutions[0] = 0;
    int psi = 1;
    int* partial_solutions_index = &psi;//index of the first free position (so it's also the length)
    
    int index = 0;

    while(index < len){
        max_depth = min(max_depth, len - index);
        int nodes_at_last_level = pow(2, max_depth);
        
        int* new_array = malloc((*partial_solutions_index) * nodes_at_last_level * sizeof(int));
        
        for(int i = 0; i < *partial_solutions_index; i++){
            explore_subtree(new_array, i*nodes_at_last_level, i*nodes_at_last_level + nodes_at_last_level,
                    partial_solutions[i], volumes, index, index+max_depth, max_capacity, 0);
        }

        int partial_solutions_index_this_iteration = *partial_solutions_index;
        for(int j = 0; j < partial_solutions_index_this_iteration * nodes_at_last_level; j++){
            if(new_array[j] != -1){     //means that there is a feasible value that we could add to the partial_solutions
                add_new(partial_solutions, partial_solutions_index, new_array[j]);
            }
        }

        free(new_array);
        index += max_depth;

    }


    int res = max_in_array(partial_solutions, *partial_solutions_index);
    free(partial_solutions);

}

//put in "new_array"", from index "start" to index "end" (first included, second excluded)
//the values found.
//This algorithm starts from the root value and creates all the sum combinations of the values in
//"volumes" with index starting from "index" and ending ad "depth".
//The final recursive call (base case) will fill the position "pos" of the "partial_solutions" array.
void explore_subtree(int* new_array, int start, int end, 
        int root_value, int* volumes, int index, int depth, int max_capacity, int pos){

    if(index == depth){
        if(root_value > max_capacity){ 
            new_array[start + pos] = -1;   //to be considered as "this cell is empty". Contains an unfeasible solution (exceeds the capacity)
        }else{
            new_array[start + pos] = root_value;
        }
        return;
    }

    int offset = pos + pow(2, depth - index)/2; //to fill the array like a tree

    //printf("index = %d\n", index);

    int v1 = root_value + 0;
    int v2 = root_value + volumes[index];
    explore_subtree(new_array, start, end, v1, volumes, index+1, depth, max_capacity, pos);
    explore_subtree(new_array, start, end, v2, volumes, index+1, depth, max_capacity, offset);
}

//checks, in the array "partial_solutions" of length "*partial_solutions_index", if there already
//is a "val" element. If there is NOT, it add it in the last position and increments by one
//"*partial_solution_index"
void add_new(int* partial_solutions, int* partial_solutions_index, int val){
    for(int i = 0; i < *partial_solutions_index; i++){
        if(partial_solutions[i] == val){
            return;
        }
    }
    
    //if the algorithm arrive here, it means that "val" is not present in "partial_solutions",
    //therefore it must be added.
    printf("number of elements in partial_solutions (-1) = %d\n", *partial_solutions_index - 1);
    partial_solutions[*partial_solutions_index] = val;
    *partial_solutions_index = *partial_solutions_index + 1;    //its length increases
    
    return;
}


int max_in_array(int* a, int len){
    int max = -1;
    for(int i = 0; i < len; i++){
        //printf("%d ", a[i]);
        if(a[i] > max){
            max = a[i];
        }
    }
    return max;
}






/*
int solve_knapsack_0_1_recursive_v2(int* volumes, int len, int max_capacity, 
        int index, int current_volume_occupied){
    int* possible_solutions = malloc(pow(2, len) * sizeof(int));
    possible_solutions[0] = 0;




}*/



