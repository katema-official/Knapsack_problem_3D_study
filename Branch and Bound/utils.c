#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

double get_random() { return (double)rand() / (double)RAND_MAX; }

int capacity_minus_placed_boxes(int capacity, box* boxes_placed, int bp_len){
    for(int i = 0; i < bp_len; i++){
        capacity -= boxes_placed[i].xlen * boxes_placed[i].ylen * boxes_placed[i].zlen;
    }
    return capacity;
}

void reset_progress_file(){
    FILE* fptr = fopen("./results/progresses.txt","w");
    fclose(fptr);
}

void append_progress_file(box* boxes_placed, int n_boxes){
    FILE* fptr = fopen("./results/progresses.txt","a");
    fprintf(fptr, "%d\n", n_boxes);
    for(int i = 0; i < n_boxes; i++){
        fprintf(fptr, "%d %d %d %d %d %d %s\n", 
            boxes_placed[i].xlen, boxes_placed[i].ylen, boxes_placed[i].zlen,
            boxes_placed[i].x0, boxes_placed[i].y0, boxes_placed[i].z0, "a");
    }
    fclose(fptr);
}




