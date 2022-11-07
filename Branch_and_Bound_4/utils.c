#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

char* filename_progress = "./results/progresses2.txt";

double get_random() { return (double)rand() / (double)RAND_MAX; }

int capacity_minus_placed_boxes(int capacity, box* boxes_placed, int bp_len){
    for(int i = 0; i < bp_len; i++){
        capacity -= boxes_placed[i].xlen * boxes_placed[i].ylen * boxes_placed[i].zlen;
    }
    return capacity;
}

void reset_progress_file(){
    FILE* fptr = fopen(filename_progress,"w");
    fclose(fptr);
}

void append_progress_file_partial_solution(box* boxes_placed, int n_boxes, 
                                    box* volumes_to_exclude, int n_volumes_to_exclude){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "%d\n", n_boxes);
    for(int i = 0; i < n_boxes; i++){
        fprintf(fptr, "%d %d %d %d %d %d %s\n", 
            boxes_placed[i].xlen, boxes_placed[i].ylen, boxes_placed[i].zlen,
            boxes_placed[i].x0, boxes_placed[i].y0, boxes_placed[i].z0, "a");
    }
    fprintf(fptr, "%d\n", n_volumes_to_exclude);
    for(int i = 0; i < n_volumes_to_exclude; i++){
        fprintf(fptr, "%d %d %d %d %d %d %s\n", 
            volumes_to_exclude[i].xlen, volumes_to_exclude[i].ylen, volumes_to_exclude[i].zlen,
            volumes_to_exclude[i].x0, volumes_to_exclude[i].y0, volumes_to_exclude[i].z0, "a");
    }
    fclose(fptr);
}



//to print after the partial solution and (possibly) the volumes to exclude have been printed.
//This tells also what is the "fate" of this node:
//0) it will generate at least another solution
//1) it won't generate new solutions, because of a dual bound lower than the primal bound
//2) it won't generate new solutions because there are no points available
void append_progress_file_solution_separator(int fate, int dual, int primal){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "---\n");
    switch(fate){
        case 0:
            fprintf(fptr, "--- explore: dual > primal - %f > %f\n", (float) dual, (float) primal);
        break;
        case 1:
            fprintf(fptr, "--- close: dual <= primal - %f <= %f\n", (float) dual, (float) primal);
        break;
        case 2:
            fprintf(fptr, "--- close: no more points available\n");
        break;
    }

    fclose(fptr);
}




