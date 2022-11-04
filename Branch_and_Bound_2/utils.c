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

void append_progress_file_partial_solution(box* boxes_placed, int n_boxes){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "%d\n", n_boxes);
    for(int i = 0; i < n_boxes; i++){
        fprintf(fptr, "%d %d %d %d %d %d %s\n", 
            boxes_placed[i].xlen, boxes_placed[i].ylen, boxes_placed[i].zlen,
            boxes_placed[i].x0, boxes_placed[i].y0, boxes_placed[i].z0, "a");
    }
    fclose(fptr);
}

void append_progress_file_box_volumes_number(int n_boxes){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "%d\n", n_boxes);
    fclose(fptr);
}

void append_progress_file_box_volume_excluded(box box_excluded){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "%d %d %d %d %d %d %s\n", 
        box_excluded.xlen, box_excluded.ylen, box_excluded.zlen,
        box_excluded.x0, box_excluded.y0, box_excluded.z0, "a");
    fclose(fptr);
}

//to print after the partial solution and (possibly) the volumes to exclude have been printed.
void append_progress_file_solution_separator(){
    FILE* fptr = fopen(filename_progress,"a");
    fprintf(fptr, "---\n");
    fclose(fptr);
}




