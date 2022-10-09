#include "utils_boxes.h"
#include "sa_performance_stuff.h"
#include "local_optimum_utils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "upgrades_v2.h"

//0 = consider boxes such that the LBB point is inside the container.
//1 = consider only boxes that are completely contained in the container
int is_box_outside_container(box actual_box, int cont_x, int cont_y, int cont_z, int mode){
    if(mode == 0){
        if(actual_box.x0 >= cont_x || actual_box.y0 >= cont_y || actual_box.z0 >= cont_z){
            return 1;
        }
    }else{
        if(actual_box.x0 + actual_box.xlen > cont_x || 
            actual_box.y0 + actual_box.ylen > cont_y || 
            actual_box.z0 + actual_box.zlen > cont_z){
            return 1;
        }
    }
    return 0;
}

//function to get the index of a box that is currently inside the container
int get_index_of_box_inside_container(box* boxes, int n_boxes, int* b_chain, 
            int cont_x, int cont_y, int cont_z){
    int* array_of_indexes_inside_container = (int*) malloc(n_boxes*sizeof(int));
    int k = 0;
    for(int j = 0; j < n_boxes; j++){
        int actual_index_of_box = b_chain[j];
        if(!is_box_outside_container(boxes[actual_index_of_box - 1], cont_x, cont_y, cont_z, 0)){  //-1 because array go from 0 to n-1
            array_of_indexes_inside_container[k] = actual_index_of_box;
            k++;
        }
    }
    /*for(int kk = 0; kk < k; kk++){
        printf("%d, ", array_of_indexes_inside_container[kk]);
    }
    printf("\n");*/

    int first = random_element_of_array(array_of_indexes_inside_container, k);
    free(array_of_indexes_inside_container);

    return first;
}

//the indexes of the boxes to swap will be *x and *y. This function grants that *x
//is ALWAYS the index of a box inside the container, while *y is the index of any other box.
void get_indexes_of_boxes_to_swap(int* x, int* y, box* boxes, int n_boxes, int* b_chain, 
            int cont_x, int cont_y, int cont_z){
    *x = get_index_of_box_inside_container(boxes, n_boxes, b_chain, cont_x, cont_y, cont_z);
    
    int* r = malloc((n_boxes - 1) * sizeof(int));
    for(int i=0;i<n_boxes-1;i++){
        if(i+1 < *x){
            r[i]=i+1;
        }else{
            r[i]=i+2;
        }
    }
    int second = random_element_of_array(r, n_boxes - 1);
    *y = second;
    free(r);
}

int random_element_of_array(int* array, int n) {
    int elem_selected = rand() % n;
    return array[elem_selected];
}

/*
//This function must return the index of a box that, in the current configuration,
//has its LBB point inside the container.
//To do so, we could simply iterate the boxes array till we find the first box outside the
//container, but that could be done better.
//Assuming that we already know what was the index of the last box inside the container (i) last
//time we checked, we now start our search from that position, finding the new last box index.
//THIS FUNCTION ASSUMES THAT AT LEAST ONE BOX IS INSIDE THE CONTAINER (boxes that don't fit
//the container shouldn't be given as input in the first place to this whole algorithm)
int choose_index_box_inside_container(box* boxes, int n_boxes, 
                    int cont_x, int cont_y, int cont_z, int* b_chain, int i){
    if(i == -1){
        //it's the first time calling the function
        for(int j = 0; j < n_boxes; j++){
            int actual_index_of_box = b_chain[j];
            if(is_box_outside_container(boxes[actual_index_of_box], cont_x, cont_y, cont_z)){
                return j-1;
            }
        }
        return n_boxes - 1;
    }else{
        //if last time we checked the box in position "i" of b-chain was inside the
        //container, this time we won't have to search much more
        int end_search = 0;
        while(!end_search){
            int actual_index_of_box = b_chain[i];
            //we have to distinguish two cases:
            //1) the current box is outside the container. This means that the last box
            //inside the container is before this one.
            //2) the current box is inside the container. This means that either this is
            //the last box inside the container, or one of its successor is the last one
            //inside the container


        }
    }
}
*/

