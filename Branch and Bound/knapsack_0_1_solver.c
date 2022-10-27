#include "knapsack_0_1_solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "structs.h"
#include "utils.h"

kp_sol_node* kp_sol_list_head;

extern int* volumes;
extern int n_volumes;

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
                B[row][col] = B[row - 1][col];  //the volume of this item is more than the current capacity
            }
        }
    }

    for(int row = 0; row < n_items + 1; row++){
        /*for(int col = 0; col < capacity + 1; col++){
            printf("%d ", B[row][col]);
        }*/
        printf("%d", B[row][capacity]);
        printf("\n");
    }

    printf("aaa = %d\n", B[n_items][capacity]);
    int res = B[n_items][capacity];

    for(int i = 0; i < n_items+1; i++){
        free(B[i]);
    }
    free(B);

    return res;
}

int solve_knapsack_0_1_v2(int* volumes, int n_items, int capacity){
    int** B = (int**) malloc(2*sizeof(int*));
    printf("asdfasf");
    for(int i = 0; i < 2; i++){
        B[i] = (int*) malloc((capacity+1)*sizeof(int));
        if(B[i] == NULL){
            printf("allocazione fallita :(\n");
        }
    }
    printf("memory allocated: %d*4 byte", capacity+1);
    
    //initialization: the subproblems without items or capacity have as best solution 0
    for(int i = 0; i < 2; i++){
        B[i][0] = 0;
    }
    for(int i = 0; i < capacity+1; i++){
        B[0][i] = 0;
    }

    clock_t start, end;
    double cpu_time_used;

    //now, the value of each cell of each row can be fully determined by the the previous row
    for(int iteration = 0; iteration < n_items; iteration++){

        start = clock();

        int volume_row = volumes[iteration];
        for(int col = 1; col < capacity + 1; col++){
            if(volume_row <= col){  //this item could be part of the solution
                B[1][col] = max(volume_row + B[0][col - volume_row], B[0][col]);
                /*if((volume_row + B[0][col - volume_row]) > B[0][col]){
                    B[1][col] = volume_row + B[0][col - volume_row];
                }else{
                    B[1][col] = B[0][col];
                }*/
            }else{
                B[1][col] = B[0][col];  //the volume of this item is more than the current capacity
            }
        }

        end = clock();
        cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("completed iteration %d in %f\n", iteration, cpu_time_used);
        start = clock();

        //now copy the new row in the old one
        for(int col = 1; col < capacity + 1; col++){
            B[0][col] = B[1][col];
        }

        end = clock();
        cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("copied iteration %d in %f\n", iteration, cpu_time_used);
    }

    printf("aaa = %d\n", B[1][capacity]);
    int res = B[1][capacity];

    for(int i = 0; i < 2; i++){
        free(B[i]);
    }
    free(B);

    return res;
}





//works only if the array of items is ordered from the one with the LEAST volume
//to the one with the MOST volume. It basically removes an if (this code should
//be the base for the parallel CUDA implementation)

int solve_knapsack_0_1_v3(int* volumes, int n_items, int capacity){
    int** B = (int**) malloc(2*sizeof(int*));
    for(int i = 0; i < 2; i++){
        B[i] = (int*) malloc((capacity+1)*sizeof(int));
        if(B[i] == NULL){
            printf("allocazione fallita :(\n");
        }
    }
    printf("memory allocated: %d*4 byte", capacity+1);
    
    //initialization: the subproblems without items or capacity have as best solution 0
    for(int i = 0; i < 2; i++){
        B[i][0] = 0;
    }
    for(int i = 0; i < capacity+1; i++){
        B[0][i] = 0;
    }

    clock_t start, end;
    double cpu_time_used;

    //now, the value of each cell of each row can be fully determined by the the previous row
    for(int iteration = 0; iteration < n_items; iteration++){

        start = clock();

        int volume_row = volumes[iteration];
        for(int col = 1; col < capacity + 1; col++){
            if(col < volume_row){
                B[1][col] = B[0][col];
            }else{
                B[1][col] = volume_row + B[0][col - volume_row];
            }
        }

        end = clock();
        cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("completed iteration %d in %f\n", iteration, cpu_time_used);
        start = clock();

        //now copy the new row in the old one
        for(int col = 1; col < capacity + 1; col++){
            B[0][col] = B[1][col];
        }

        end = clock();
        cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("copied iteration %d in %f\n", iteration, cpu_time_used);
    }

    int res = B[1][capacity];

    for(int i = 0; i < 2; i++){
        free(B[i]);
    }
    free(B);

    return res;
}










//let's also define some functions that, given a subproblem with:
//-a given set of boxes already placed (that tells us how many and which boxes still need to be placed)
//-a given maximum capacity
//tells us, looking at a data structure, if that solution has already been found
int get_dual_bound_using_kp_0_1(int* volumes_of_boxes_placed, int n_boxes_placed, int max_capacity){
    
    //first, let's check if there is a kp_sol_node, in our list of saved solutions, that
    //has the same capacity and the same boxes. Since we want to keep this list ordered,
    //our search can stop as soon as we exceed the max_capacity.
    int found = 0;
    kp_sol_node* tmp_front;
    kp_sol_node* tmp_back;

    if(kp_sol_list_head == NULL){
        //The very first node. It must be created.
        kp_sol_node* new = create_new_kp_sol_node(max_capacity, n_boxes_placed, volumes_of_boxes_placed);
        new->succ = NULL;
        kp_sol_list_head = new;
        return new->best_solution;
    }

    tmp_back = kp_sol_list_head;
    tmp_front = tmp_back->succ;

    //if the capacity is the lowest one (lower than the one on the head of the list)
    //we have to update the head of the list
    if(max_capacity < kp_sol_list_head->max_capacity){
        kp_sol_node* new = create_new_kp_sol_node(max_capacity, n_boxes_placed, volumes_of_boxes_placed);
        new->succ = kp_sol_list_head;
        kp_sol_list_head = new;
        return new->best_solution;
    }

    //we can trash all the solutions with lower capacity than the one of this subproblem
    while(tmp_front != NULL && tmp_back->max_capacity < max_capacity){
        tmp_back = tmp_back->succ;
        tmp_front = tmp_back->succ;
    }

    //we now have to examine all the solutions with the same capacity of our subproblem
    while(tmp_front != NULL && tmp_back->max_capacity == max_capacity && 
            tmp_front->max_capacity <= max_capacity){
        //We have to check if the current node is the one we're looking for.
        //The node is the one we are looking for if the capacity is the same and the placed
        //boxes are the same.
        if(tmp_back->n_placed_boxes_volumes == n_boxes_placed){
            int equal = are_array_contents_the_same(tmp_back->placed_boxes_volumes,
                                        volumes_of_boxes_placed, n_boxes_placed);
            if(equal == 1){
                //ok, they are really the same: return this solution
                printf("found solution of already sovled problem: %d\n", tmp_back->best_solution);
                return tmp_back->best_solution;
            }
        }
    }

    //if we have arrived here, it's because we reached the last node in the list.
    //If it has the same capacity of our subproblem, we have to check if this last
    //node is the one with the solution we are looking for. 
    if(tmp_back->max_capacity == max_capacity && tmp_back->n_placed_boxes_volumes == n_boxes_placed){
        int equal = are_array_contents_the_same(tmp_back->placed_boxes_volumes,
                                    volumes_of_boxes_placed, n_boxes_placed);
        if(equal == 1){
            printf("found solution of already sovled problem: %d\n", tmp_back->best_solution);
            return tmp_back->best_solution;
        }
    }

    //If it is not, we have to solve the subproblem ourselves.
    kp_sol_node* new = create_new_kp_sol_node(max_capacity, n_boxes_placed, volumes_of_boxes_placed);
    //set the successor
    new->succ = tmp_front;
    //link it to the list
    tmp_back->succ = new;
    printf("Computed solution of new problem: %d\n", new->best_solution);
    return new->best_solution;


}

//function to check if two integer arrays (that contain volumes, that is, all the values
//are > 0, and this is important because otherwise this doesn't work) have the same values 
//inside, doesn't matter the order
int are_array_contents_the_same(int* first_array, int* second_array, int n){
    int* first = malloc(n*sizeof(int));
    int* second = malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        first[i] = first_array[i];
        second[i] = second_array[i];
    }

    for(int i = 0; i < n; i++){
        int ok = 0;
        for(int j = 0; j < n; j++){
            if(first[i] == second[j]){
                second[j] = -1;
                ok = 1;
                break;
            }
        }
        if(!ok){
            free(first);
            free(second);
            return 0;
        }
    }

    free(first);
    free(second);
    return 1;

}

kp_sol_node* create_new_kp_sol_node(int max_capacity, int n_boxes_placed, int* volumes_of_boxes_placed){
    kp_sol_node* new = malloc(sizeof(kp_sol_node));

    //set its max capacity
    new->max_capacity = max_capacity;

    //set the array of the volumes of the boxes already placed
    new->n_placed_boxes_volumes = n_boxes_placed;
    int* vols = (int*) malloc(n_boxes_placed*sizeof(int));
    for(int i = 0; i < n_boxes_placed; i++){
        vols[i] = volumes_of_boxes_placed[i];
    }
    new->placed_boxes_volumes = vols;

    //-----compute the solution for this subproblem (the dual bound)-----
    
    //copy the volumes of the boxes placed
    int* volumes_of_boxes_placed_copy = malloc(n_boxes_placed*sizeof(int));
    for(int i = 0; i < n_boxes_placed; i++){
        volumes_of_boxes_placed_copy[i] = volumes_of_boxes_placed[i];
    }

    //build the array with the volumes of boxes not placed
    int* volumes_for_computing_dual_bound = malloc((n_volumes - n_boxes_placed) * sizeof(int));
    int index = 0;
    for(int i = 0; i < n_volumes; i++){
        int equal = 0;
        for(int j = 0; j < n_boxes_placed; j++){
            //if, in the complete volumes array, i found this same volume, i don't add
            //it to the volumes considered for solving the problem, and i forget about it
            if(volumes[i] == volumes_of_boxes_placed_copy[j]){
                equal = 1;
                volumes_of_boxes_placed_copy[j] = -1;
                break;
            }
        }
        if(!equal){
            volumes_for_computing_dual_bound[index] = volumes[i];
            index++;
        }
    }

    //now, compute the dual bound and save it
    int dual_bound = solve_knapsack_0_1(volumes_for_computing_dual_bound, 
                                        n_volumes - n_boxes_placed, max_capacity);
    
    new->best_solution = dual_bound;

    free(volumes_for_computing_dual_bound);
    free(volumes_of_boxes_placed_copy);

    //--------------------------------------------------------------------

    return new;
}


