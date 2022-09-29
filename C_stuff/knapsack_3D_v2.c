#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils_boxes.h"


int main(){
    srand(time(NULL));

    //cleanup of a utility file
    FILE* p = fopen("./results.txt", "w");
    fclose(p);

    //define container dimensions
    int cont_x = 100;
    int cont_y = 100;
    int cont_z = 100;

    int n_boxes = 0;
    box* boxes;

    int read_from_file = 1;
    if(read_from_file){
        //adapter from https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
        FILE* f = fopen("./input.txt", "r");
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
            b.name = (char*) malloc(strlen(info_of_a_box[3])*sizeof(char));
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




    //****************************************************
    //**************WHERE THE MAGIC HAPPENS***************
    //****************************************************
    simulated_annealing_knapsack_3D(a, b, c, boxes, n_boxes, 0, 600, cont_x, cont_y, cont_z);


    return 0;
}




