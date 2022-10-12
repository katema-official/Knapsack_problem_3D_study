#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "utils.h"
#include "boxes.h"
#include "extreme_points.h"

//#ifndef GLOBALS
//#define GLOBALS

int primal_bound = 0;
//define container dimensions
int cont_x = 800;
int cont_y = 700;
int cont_z = 1000;

node* head;

//#endif

int main(){
    srand(time(NULL));

    //cleanup of a utility file
    FILE* p = fopen("./results.txt", "a");
    fclose(p);
    
    int n_boxes = 0;
    box* boxes;
    char** boxes_names; //I keep the name of the boxes in a separate array.

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
        boxes_names = (char**) malloc(n_boxes*sizeof(char*));
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
            //b.name = (char*) malloc((strlen(info_of_a_box[3])+1)*sizeof(char));
            //strcpy(b.name, info_of_a_box[3]);
            boxes_names[box_index] = (char*) malloc((strlen(info_of_a_box[3])+1)*sizeof(char));
            strcpy(boxes_names[box_index], info_of_a_box[3]);
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
            boxes_names[i] = "foobar";
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





    //****************************************************
    //**************BRANCH AND BOUND START****************
    //****************************************************
    
    //when a node is detected to be infeasible or its dual bound is lower
    //than the actual primal bound (the super-optimal solution of the subproblem
    //is worse than the best known feasible solution), the node is closed
    int max_number_of_boxes_placeable = 0;
    if(order_boxes){
        int total_volume = cont_x*cont_y*cont_z;
        for(int i = n_boxes - 1; i >= 0; i++){
            int volume_of_this_box = boxes[i].xlen * boxes[i].ylen *boxes[i].zlen;
            if(volume_of_this_box <= total_volume){
                total_volume -= volume_of_this_box;
                max_number_of_boxes_placeable++;
            }else{
                break;
            }
        }
    }else{
        int max_number_of_boxes_placeable = n_boxes;
    }

    box* boxes_to_place = boxes;
    //there is also a maximum number of extreme points that there can be in a subproblem
    int max_extreme_points = (max_number_of_boxes_placeable - 1) * 2 + 3;

    node* first = (node*) malloc(sizeof(node));
    first->extreme_points = (point*) malloc(max_extreme_points * sizeof(point));
    first->boxes_to_place = (box*) malloc(n_boxes*sizeof(box));
    first->boxes_placed = (box*) malloc(max_number_of_boxes_placeable*sizeof(box));

    //initialize the first node
    point initial_point;

    first->extreme_points[0].x = 0;
    first->extreme_points[0].y = 0;
    first->extreme_points[0].z = 0;
    first->extreme_points[0].width = cont_x;
    first->extreme_points[0].height = cont_y;
    first->extreme_points[0].depth = cont_z;
    first->ep_len = 1;
    copy_boxes(&(first->boxes_to_place), boxes_to_place, n_boxes);
    first->btp_len = n_boxes;
    first->bp_len = 0;
    first->succ = NULL;

    head = first;

    //exhaustive B&B exploration begins: all nodes in the list represent open nodes
    //of the tree, that therfore must be explored.
    while(head != NULL){
        int ep_len = head->ep_len;
        int btp_len = head->btp_len;
        int bp_len = head->bp_len;
        for(int i = 0; i < btp_len; i++){
            //i take the boxes to place one at a time
            box new_box = head->boxes_to_place[i];
            int** rotations = rotations_of_box(new_box);

            for(int j = 0; j < 6; j++){
                //i consider the 6 ritations the box can take one at a time
                int* current_rotation = rotations[j];
                box new_box_copy = malloc(sizeof(box));
                copy_box(&new_box_copy, new_box);
                new_box_copy.xlen = current_rotation[0];
                new_box_copy.ylen = current_rotation[1];
                new_box_copy.zlen = current_rotation[2];

                for(k = 0; k < ep_len; k++){
                    //i consider one at a time all the possible points in which the box can be put
                    point p = head->extreme_points[k];
                    new_box_copy.x0 = p.x;
                    new_box_copy.y0 = p.y;
                    new_box_copy.z0 = p.z;

                    //we generate the new node only if this box, with this rotation, in that point,
                    //doesn't collide with another box AND gets outside of the container
                    if(!is_box_outside_container(new_box_copy)){
                        for(kk = 0; kk < bp_len; kk++){
                            box placed_box = head->placed_boxes[kk];
                            if(!do_boxes_overlap(new_box_copy, placed_box)){
                                //we can explore the node
                            }

                        }
                    }


                    free(new_box_copy);


                }

                


                //freeing
                for(int i=0; i < 6; i++){
                    free(rotations[i]);
                }
                free(rotations)
            }


        }
    }




}






