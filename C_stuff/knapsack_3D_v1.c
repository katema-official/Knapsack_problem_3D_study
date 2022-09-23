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


    int debug = 1;
    if(debug){
        printf("DEBUG");
        boxes[0].xlen = 37;
        boxes[0].ylen = 16;
        boxes[0].zlen = 46;
        boxes[1].xlen = 19;
        boxes[1].ylen = 33;
        boxes[1].zlen = 43;
        boxes[2].xlen = 9;
        boxes[2].ylen = 48;
        boxes[2].zlen = 44;
        boxes[3].xlen = 18;
        boxes[3].ylen = 15;
        boxes[3].zlen = 33;
        boxes[4].xlen = 6;
        boxes[4].ylen = 9;
        boxes[4].zlen = 9;
        a[0] = 1; a[1] = 4; a[2] = 2; a[3] = 5; a[4] = 3;
        b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4; b[4] = 5;
        a[0] = 4; a[1] = 2; a[2] = 5; a[3] = 1; a[4] = 3;
    }


    //*************************
    //****a bunch of prints****
    //*************************
    debug_print(n_boxes, boxes, a, b, c);

    printf("oooddoo\n");




    simulated_annealing_knapsack_3D(a, b, c, boxes, n_boxes, 0, 10, cont_x, cont_y, cont_z);


    return 0;
}




