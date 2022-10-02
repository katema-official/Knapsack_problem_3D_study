#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils_boxes.h"
#include "sa_performance_stuff.h"
#include "local_optimum_utils.h"

//This main allows to run multiple times the simulated annealing algorithm starting
//from the same set of boxes.

void run_instance_of_simulated_annealing(int order, int n_boxes, box* boxes, 
        int cont_x, int cont_y, int cont_z){
    //generate the three lists a, b and c
    int* a = (int*) random_permutation_1_to_n(n_boxes);
    int* b = (int*) random_permutation_1_to_n(n_boxes);
    int* c = (int*) random_permutation_1_to_n(n_boxes);

    //one might want to order the boxes in descending volume value.
    if(order){
        for(int i = 0; i < n_boxes; i++){
            b[i] = i+1;
        }
    }

    box* copied_boxes = (box*) malloc(n_boxes*sizeof(box));
    copy_boxes(&copied_boxes, boxes, n_boxes);
    copy_boxes_name(&copied_boxes, boxes, n_boxes);

    simulated_annealing_knapsack_3D(a, b, c, copied_boxes, n_boxes, 1, 60, cont_x, cont_y, cont_z);

    free(a);
    free(b);
    free(c);
    for(int i = 0; i < n_boxes; i++){
        free(copied_boxes[i].name);
    }
    free(copied_boxes);

}

int main(){
    srand(time(NULL));

    //cleanup of a utility file
    FILE* p = fopen("./results.txt", "a");
    fclose(p);

    //define container dimensions
    int cont_x = 800;
    int cont_y = 700;
    int cont_z = 1000;

    int n_boxes = 0;
    box* boxes;

    int read_from_file = 1;
    if(read_from_file){
        //adapter from https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
        FILE* f = fopen("./input2.txt", "r");
        if (f == NULL){exit(EXIT_FAILURE);}

        //how many boxes are there?
        n_boxes = 0;
        while(!feof(f)){
            int ch = fgetc(f);
            if(ch == '\n'){
                n_boxes++;
            }
        }
        boxes = (box*) malloc(n_boxes*sizeof(box));
        rewind(f);

        //let's see the actual informations about those boxes.
        char * line = NULL;
        size_t len = 0;
        size_t read;
        char delim[] = "|";
        int box_index = 0;
        while ((read = getline(&line, &len, f)) != -1) {
            //protocol of a line: xlen|ylen|zlen|name\n
            char** info_of_a_box = (char**) malloc(4*sizeof(char*));
            line[read-1] = delim[0];
            line[read-2] = delim[0];
            char* token = strtok(line, delim);
            int i = 0;
            while(token != NULL){
                int len = strlen(token);
                info_of_a_box[i] = token;
                i++;
                token = strtok(NULL, delim);
            }
            box b;
            b.xlen = atoi(info_of_a_box[0]);
            b.ylen = atoi(info_of_a_box[1]);
            b.zlen = atoi(info_of_a_box[2]);
            b.name = (char*) malloc((strlen(info_of_a_box[3])+1)*sizeof(char));
            strcpy(b.name, info_of_a_box[3]);
            b.x0 = -1;
            b.y0 = -1;
            b.z0 = -1;
            boxes[box_index] = b;
            box_index++;
            free(info_of_a_box);
        }
        fclose(f);
        if(line){free(line);}



    }else{
    //generate boxes
        n_boxes = 5;
        boxes = (box*) malloc(n_boxes*sizeof(box));
        for(int i = 0; i < n_boxes; i++){
            box b;
            b.name = "pippo";
            b.xlen = rand()%50;
            b.ylen = rand()%50;
            b.zlen = rand()%50;
            b.x0 = -1;
            b.y0 = -1;
            b.z0 = -1;
            boxes[i] = b;
        }
    }

    int order_boxes = 1;
    if(order_boxes){
        qsort(boxes, n_boxes, sizeof(box), comparator_boxes_volume);
    }


    if(DEBUG_1)clear_generation_performance_file();
    initialize_array_local_optimum();
    for(int i = 0; i < 1; i++){
        //****************************************************
        //**************WHERE THE MAGIC HAPPENS***************
        //****************************************************
        if(DEBUG_1)initialize();
        run_instance_of_simulated_annealing(order_boxes, n_boxes, boxes, cont_x, cont_y, cont_z);
        reset_array_local_optimum();
    }
    free_array_local_optimum();

    for(int i = 0; i < n_boxes; i++){
        free(boxes[i].name);
    }
    free(boxes);

    return 0;
}




