#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils_boxes.h"






int main(){
    srand(time(NULL));

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
        b.x0 = 0;
        b.y0 = 0;
        b.z0 = 0;
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

    for(int i = 0; i < n_boxes; i++){
        printf("%d - %d - %d\n", boxes[i].xlen, boxes[i].ylen, boxes[i].zlen);
    }


    for(int i = 0; i < n_boxes; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    for(int i = 0; i < n_boxes; i++){
        printf("%d ", b[i]);
    }
    printf("\n");
    for(int i = 0; i < n_boxes; i++){
        printf("%d ", c[i]);
    }
    printf("\n");

    printf("oooddoo\n");


    return 0;
}




