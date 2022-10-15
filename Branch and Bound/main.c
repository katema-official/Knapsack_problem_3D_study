#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "utils.h"
#include "boxes.h"
#include "extreme_points.h"

int primal_bound = 0;
//define container dimensions
int cont_x = 100;//800;
int cont_y = 100;//700;
int cont_z = 100;//1000;

node* head;



void generate_new_node(point* extreme_points, int n_extreme_points, 
                        box* boxes_placed, int n_boxes_placed, 
                        box* boxes_to_place, int n_boxes_to_place,
                        box new_box, int i_ep, int i_btp);

int comp1(const void * a, const void * b);

int dual_bound_all_liquid(box* boxes_placed, int n_boxes_placed,
                         box* boxes_to_place, int n_boxes_to_place);

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
        for(int i = n_boxes - 1; i >= 0; i--){
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
    first->extreme_points = (point*) malloc(1 * sizeof(point)); //at first, there is only 1 extreme point
    first->boxes_to_place = (box*) malloc(n_boxes*sizeof(box));
    first->boxes_placed = NULL;//(box*) malloc(max_number_of_boxes_placeable*sizeof(box));

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
    printf("ciao");

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

                for(int k = 0; k < ep_len; k++){
                    //i consider one at a time all the possible points in which the box can be put
                    point p = head->extreme_points[k];
                    box new_box_copy;
                    copy_box(&new_box_copy, new_box);
                    new_box_copy.xlen = current_rotation[0];
                    new_box_copy.ylen = current_rotation[1];
                    new_box_copy.zlen = current_rotation[2];
                    new_box_copy.x0 = p.x;
                    new_box_copy.y0 = p.y;
                    new_box_copy.z0 = p.z;

                    //we generate the new node only if this box, with this rotation, in that point,
                    //doesn't collide with another box AND does not get outside of the container
                    if(!is_box_outside_container(new_box_copy)){
                        int can_place = 1;
                        for(int kk = 0; kk < bp_len; kk++){
                            box placed_box = head->boxes_placed[kk];
                            if(do_boxes_overlap(new_box_copy, placed_box)){
                                can_place = 0;
                            }
                        }
                        if(can_place){
                            //we can explore the node
                            generate_new_node(first->extreme_points, first->ep_len,
                                                first->boxes_placed, first->bp_len,
                                                first->boxes_to_place, first->btp_len,
                                                new_box_copy, k, i);
                        }
                    }

                }


                
            }

            //freeing
            for(int i=0; i < 6; i++){
                free(rotations[i]);
            }
            free(rotations);


        }
        node* tmp = head;
        head = head->succ;
        free(tmp);

    }

}

//i_ep = the index in extreme_points of the point used just now to place the box
//i_btp = index in boxes_to_place of the box just placed
void generate_new_node(point* extreme_points, int n_extreme_points, 
                        box* boxes_placed, int n_boxes_placed, 
                        box* boxes_to_place, int n_boxes_to_place,
                        box new_box, int i_ep, int i_btp){

    //copy the boxes placed, adding the new box placed
    box* new_boxes_placed = malloc((n_boxes_placed+1) * sizeof(box));
    copy_boxes(&new_boxes_placed, boxes_placed, n_boxes_placed);
    new_boxes_placed[n_boxes_placed] = new_box;
    
    //copy the extreme points by removing the point in which the box has been placed, then add the three new points
    point* new_extreme_points = get_copy_points_except_one(extreme_points, n_extreme_points, i_ep);
    point p1;
    point p2;
    point p3;
    p1.x = new_box.x0 + new_box.xlen;
    p1.y = 0;
    p1.z = 0;

    p2.x = 0;
    p2.y = new_box.y0 + new_box.ylen;
    p2.z = 0;

    p3.x = 0;
    p3.y = 0;
    p3.z = new_box.z0 + new_box.zlen;
    update_point_dimensions(&p1, new_boxes_placed, n_boxes_placed+1);
    update_point_dimensions(&p2, new_boxes_placed, n_boxes_placed+1);
    update_point_dimensions(&p3, new_boxes_placed, n_boxes_placed+1);
    new_extreme_points[n_extreme_points] = p1;
    new_extreme_points[n_extreme_points] = p2;
    new_extreme_points[n_extreme_points] = p3;

    //copy the boxes to place removing the box placed right now
    box* new_boxes_to_place = get_copy_boxes_except_one(boxes_to_place, n_boxes_to_place, i_btp);

    //now compute the dual bound, and if it is worse than primal bound, don't even open this node
    dual_bound_all_liquid(new_boxes_placed, n_boxes_placed+1,
                            new_boxes_to_place, n_boxes_to_place-1);

    

}


int dual_bound_all_liquid(box* boxes_placed, int n_boxes_placed,
                         box* boxes_to_place, int n_boxes_to_place){
    //in general, to compute this (weak) dual bound, we have to check
    //the boxes_to_place, from the one with the most volume to the one with
    //the least volume
    int* boxes_to_place_volumes = malloc(n_boxes_to_place * sizeof(int));
    for(int i = 0; i < n_boxes_to_place; i++){
        boxes_to_place_volumes[i] = boxes_to_place[i].xlen * boxes_to_place[i].ylen * boxes_to_place[i].zlen;
    }
    qsort(boxes_to_place_volumes, n_boxes_to_place, sizeof(int), comp1);

    int total_volume = cont_x*cont_y*cont_z;
    //first, let's remove the volume of the boxes already placed.
    for(int i = 0; i < n_boxes_placed; i++){
        total_volume -= boxes_placed[i].xlen*boxes_placed[i].ylen*boxes_placed[i].zlen;
    }
    //then, the volume of the boxes to place
    for(int i = 0; i < n_boxes_to_place; i++){
        if(boxes_to_place_volumes[i] <= total_volume){
            total_volume -= boxes_to_place_volumes[i];
        }
    }
    printf("Total volume = %d\n", total_volume);
    return total_volume;






}


int comp1(const void * a, const void * b){
    int a1 = *(const int*) a;
    int b1 = *(const int*) b;
    return b1 - a1;
}

