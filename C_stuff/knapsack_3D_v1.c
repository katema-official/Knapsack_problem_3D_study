#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils_boxes.h"


int main(){
    srand(time(NULL));

    //define container dimensions
    int cont_x = 100;
    int cont_y = 90;
    int cont_z = 80;

    //generate boxes
    int n_boxes = 5;
    box* boxes = (box*) malloc(n_boxes*sizeof(box));
    for(int i = 0; i < n_boxes; i++){
        box b;
        b.name = "";
        b.xlen = rand()%50;
        b.ylen = rand()%50;
        b.zlen = rand()%50;
        b.x0 = -1;
        b.y0 = -1;
        b.z0 = -1;
        boxes[i] = b;
    }

    //generate the three lists a, b and c
    int* a = (int*) random_permutation_1_to_n(n_boxes);
    int* b = (int*) random_permutation_1_to_n(n_boxes);
    int* c = (int*) random_permutation_1_to_n(n_boxes);

    //one might want to order the boxes in descending volume value.
    int boxes_ordered = 1;
    if(boxes_ordered){
        qsort(boxes, n_boxes, sizeof(box), comparator_boxes_volume);
        for(int i = 0; i < n_boxes; i++){
            b[i] = i+1;
        }
    }


    //*************************
    //****a bunch of prints****
    //*************************
    debug_print(n_boxes, boxes, a, b, c);

    printf("oooddoo\n");




    simulated_annealing_knapsack_3D(a, b, c, boxes, n_boxes, 0, 10, cont_x, cont_y, cont_z);


    return 0;
}




