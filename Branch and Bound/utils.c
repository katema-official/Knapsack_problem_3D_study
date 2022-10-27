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
}
