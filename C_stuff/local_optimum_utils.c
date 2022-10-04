#include "utils_boxes.h"
#include "sa_performance_stuff.h"
#include "local_optimum_utils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


//******************************************************************
//*******FOR SAVING THE LOCAL OPTIMUM FOUND IN THIS RUN*************
//******************************************************************
//I make array of boxes, a,b, and c-chain because we might have multiple
//solutions that have the same volume, and I want them all because afterwards
//we might implement a procedure that takes the best solution among them.
int array_of_local_optimum_dim;    //can be customized 
int array_of_local_optimum_index;   //the first empty cell of the arrays of the local optimum
box** array_of_local_optimum_boxes;
int** array_of_local_optimum_a;
int** array_of_local_optimum_b;
int** array_of_local_optimum_c;
int best_volume_local_optimum; //volume of the best solution found in this run.
//useful to understand if a better local optimum has been found.

void initialize_array_local_optimum(){
    array_of_local_optimum_dim = 50;
    array_of_local_optimum_index = 0;
    best_volume_local_optimum = 0;

    array_of_local_optimum_boxes = (box**) malloc(array_of_local_optimum_dim*sizeof(box*));
    array_of_local_optimum_a = (int**) malloc(array_of_local_optimum_dim*sizeof(int*));
    array_of_local_optimum_b = (int**) malloc(array_of_local_optimum_dim*sizeof(int*));
    array_of_local_optimum_c = (int**) malloc(array_of_local_optimum_dim*sizeof(int*));

    //we also clear the contents of the results file
    FILE* f = fopen("final_results.txt", "w");
    fclose(f);
}


void free_array_local_optimum(){
    free(array_of_local_optimum_boxes);
    free(array_of_local_optimum_a);
    free(array_of_local_optimum_b);
    free(array_of_local_optimum_c);
}

//soft reset of the data
void reset_array_local_optimum(){
    for(int i = 0; i < array_of_local_optimum_index; i++){
        free(array_of_local_optimum_boxes[i]);
        free(array_of_local_optimum_a[i]);
        free(array_of_local_optimum_b[i]);
        free(array_of_local_optimum_c[i]);
    }
    array_of_local_optimum_index = 0;
    best_volume_local_optimum = 0;
}

//function that, given a newly generated solution, saves it in the array of local optimums
//if it is better than the others found so far
void evaluate_new_solution(int volume_of_this_solution, box* boxes, int* a, int* b, int* c, int n_boxes){
    if(volume_of_this_solution == best_volume_local_optimum){
        //append the solution to the array
        save_local_optimum(boxes, a, b, c, n_boxes, 0);
    }else if (volume_of_this_solution > best_volume_local_optimum){
        best_volume_local_optimum = volume_of_this_solution;
        save_local_optimum(boxes, a, b, c, n_boxes, 1);
    }
}

//function that saves in array_of_local_optimum_boxes and array_of_local_optimum_x
//the solution given in boxes and a (,b and c).
//append_or_overwrite = 0 -> Append the solution to the array
//append_or_overwrite = 1 -> overwrite the array with the new solution
void save_local_optimum(box* boxes, int* a, int* b, int* c, int n_boxes, int append_or_overwrite){

    if(append_or_overwrite == 0 && array_of_local_optimum_index == array_of_local_optimum_dim) return;

    box* local_optimum_boxes = (box*) malloc(n_boxes*sizeof(box));
    int* local_optimum_a = (int*) malloc(n_boxes*sizeof(int));
    int* local_optimum_b = (int*) malloc(n_boxes*sizeof(int));
    int* local_optimum_c = (int*) malloc(n_boxes*sizeof(int));

    //copy of the local optimum found
    copy_boxes(&local_optimum_boxes, boxes, n_boxes);
    copy_sequence(&local_optimum_a, a, n_boxes);
    copy_sequence(&local_optimum_b, b, n_boxes);
    copy_sequence(&local_optimum_c, c, n_boxes);

    if(append_or_overwrite == 0){
        //append
        array_of_local_optimum_boxes[array_of_local_optimum_index] = local_optimum_boxes;
        array_of_local_optimum_a[array_of_local_optimum_index] = local_optimum_a;
        array_of_local_optimum_b[array_of_local_optimum_index] = local_optimum_b;
        array_of_local_optimum_c[array_of_local_optimum_index] = local_optimum_c;
        array_of_local_optimum_index++;
    }else if(append_or_overwrite == 1){ //could be done better initializing array to null and freeing it all
        //better local optimum found, discard preceeding solutions
        for(int i = 0; i < array_of_local_optimum_index; i++){
            free(array_of_local_optimum_boxes[i]);
            free(array_of_local_optimum_a[i]);
            free(array_of_local_optimum_b[i]);
            free(array_of_local_optimum_c[i]);
        }
        array_of_local_optimum_index = 0;

        //now save the new best solution found so far
        array_of_local_optimum_boxes[array_of_local_optimum_index] = local_optimum_boxes;
        array_of_local_optimum_a[array_of_local_optimum_index] = local_optimum_a;
        array_of_local_optimum_b[array_of_local_optimum_index] = local_optimum_b;
        array_of_local_optimum_c[array_of_local_optimum_index] = local_optimum_c;
        array_of_local_optimum_index++;
        
    }
}

void result_print_local_optimum_found(int n_boxes, char** boxes_names){
    FILE* f = fopen("final_results.txt", "a");
    for(int k = 0; k < array_of_local_optimum_index; k++){
        box* boxes = array_of_local_optimum_boxes[k];
        fprintf(f, "%d\n", n_boxes);
        for(int i = 0; i < n_boxes; i++){
            fprintf(f, "%d %d %d %d %d %d %s\n", boxes[i].xlen, boxes[i].ylen, boxes[i].zlen,
                    boxes[i].x0, boxes[i].y0, boxes[i].z0, boxes_names[i]);
        }
        int* a = array_of_local_optimum_a[k];
        for(int i = 0; i < n_boxes; i++){
            fprintf(f, "%d ", a[i]);
        }
        fprintf(f, "\n");
        int* b = array_of_local_optimum_b[k];
        for(int i = 0; i < n_boxes; i++){
            fprintf(f, "%d ", b[i]);
        }
        fprintf(f, "\n");
        int* c = array_of_local_optimum_c[k];
        for(int i = 0; i < n_boxes; i++){
            fprintf(f, "%d ", c[i]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void print_occupancy(int cont_x, int cont_y, int cont_z){
    printf("The occupancy of the solution(s) found is %lf. Volume occupied is %d, while total volume is %d.\n", 
        (double) best_volume_local_optimum/ (double) (cont_x*cont_y*cont_z), best_volume_local_optimum, cont_x*cont_y*cont_z);
}
