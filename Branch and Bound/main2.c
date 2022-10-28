#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "utils.h"
#include "boxes.h"
#include "extreme_points.h"
#include "knapsack_0_1_solver.h"

int primal_bound = 0;
//define container dimensions
int cont_x = 100;//800;
int cont_y = 1000;//700;
int cont_z = 1000;//1000;

node* head;

int* volumes;   //array that keeps the volumes of the boxes. If, in position i of the boxes array,
//there is the i-th box, then in this array, in position i, there is its volume.
int n_volumes;


void generate_new_node(point* extreme_points, int n_extreme_points, 
                        box* boxes_placed, int n_boxes_placed, 
                        box* boxes_to_place, int n_boxes_to_place,
                        box new_box, int i_ep, int i_btp);

int comp1(const void * a, const void * b);


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

    //---------------------------------------------------------------------
    //-------------------------BFS INITIALIZATION--------------------------
    //---------------------------------------------------------------------
    //we have to initialize the list of subproblems by creating 6*n_boxes problems:
    //each of them represents the fact that we are placing a box in the first
    //extreme point, in any of the 6 possible configurations
    for(int i = 0; i < n_boxes; i++){
        int** rotations = rotations_of_box(boxes[i]);
        for(int j = 0; j < 6; j++){
            box this_box;
            copy_box(&this_box, boxes[i]);
            this_box.x0 = 0;
            this_box.y0 = 0;
            this_box.z0 = 0;
            this_box.xlen = rotations[j][0];
            this_box.ylen = rotations[j][1];
            this_box.zlen = rotations[j][2];

            node* this_node = malloc(sizeof(node));

            //initialize boxes placed given the placed box
            this_node->bp_len = 1;
            this_node->boxes_placed = (box*) malloc(this_node->bp_len * sizeof(box));
            this_node->boxes_placed[0] = this_box;

            //initialize boxes to place given that we already placed a box
            this_node->btp_len = n_boxes-1;
            this_node->boxes_to_place = (box*) malloc(this_node->btp_len * sizeof(box));
            this_node->boxes_to_place = get_copy_boxes_except_one(boxes, n_boxes, i);

            //initialize extreme points given the placed box
            this_node->ep_len = 3;
            this_node->extreme_points = (point*) malloc(this_node->ep_len * sizeof(point));

            this_node->extreme_points[0].x = this_box.x0 + this_box.xlen;
            this_node->extreme_points[0].y = 0;
            this_node->extreme_points[0].z = 0;
            this_node->extreme_points[0].width = cont_x - this_node->extreme_points[0].x;
            this_node->extreme_points[0].height = cont_y;
            this_node->extreme_points[0].depth = cont_z;

            this_node->extreme_points[1].x = 0;
            this_node->extreme_points[1].y = this_box.y0 + this_box.ylen;
            this_node->extreme_points[1].z = 0;
            this_node->extreme_points[1].width = cont_x;
            this_node->extreme_points[1].height = cont_y - this_node->extreme_points[1].y;
            this_node->extreme_points[1].depth = cont_z;

            this_node->extreme_points[2].x = 0;
            this_node->extreme_points[2].y = 0;
            this_node->extreme_points[2].z = this_box.z0 + this_box.zlen;
            this_node->extreme_points[2].width = cont_x;
            this_node->extreme_points[2].height = cont_y;
            this_node->extreme_points[2].depth = cont_z - this_node->extreme_points[2].z;

            //initialize the successor
            this_node->succ = head;

            //this is a LIFO ordering
            head = this_node;

        }
    }

    

    //----------------------------------------------------------
    //------------INITIALIZATION OF VOLUMES ARRAY---------------
    volumes = (int*) malloc(n_boxes*sizeof(int));
    for(int i = 0; i < n_boxes; i++){
        volumes[i] = boxes[i].xlen * boxes[i].ylen * boxes[i].zlen;
    }

    n_volumes = n_boxes;


    //TEST to see if it works up until now (it does)
    /*while(head!=NULL){
        printf("this volume = %d\n", head->boxes_placed[0].xlen * 
                                    head->boxes_placed[0].ylen * 
                                    head->boxes_placed[0].zlen);
        head = head->succ;
    }*/
    //----------------------------------------------------------

    //exhaustive B&B exploration begins: all nodes in the list represent open nodes
    //of the tree, that therfore must be explored.
    while(head != NULL){
        explore_node(head);
    }





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
                            /*generate_new_node(first->extreme_points, first->ep_len,
                                                first->boxes_placed, first->bp_len,
                                                first->boxes_to_place, first->btp_len,
                                                new_box_copy, k, i);*/
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

    free(volumes);

}
















//what is a node? A node is a subproblem, that contains:
//-the boxes placed in that subproblem
//-the boxes that still need to be placed
//-the extreme points given from the boxes already placed
//now, this subproblem can generate new subproblems. However, since this is a B&B algorithm,
//we can close a node when one of the two following situations occurs:
//-the current subproblem has a dual bound worse (lower, in this case) than the primal bound
//-the current subproblem is infeasible
//now: for each node, we will test the first condition, and if dual_bound < upper_bound, we
//close the node. Otherwise, we generate, from this subproblem, ALL its possible successors.
//BUT! When generating the successors, we check if that successor is feasible. If it is,
//we put it in the list. Otherwise, we discard it.
//In a "pure" B&B algorithm, we should generate all subproblems, and in them check if they
//are feasible. But, in this case, I think it would be more profitable to tentatively craete
//a new subproblem given the current one. If the tentative subproblem is faesible, we add it
//to the list. Otherwise, we discard it.
void explore_node(node* node){
    //1) COMPUTE DUAL BOUND

    //1.1) create an array with the volumes of the boxes to place (and order them)
    int* volumes = malloc(node->btp_len * sizeof(int));
    for(int i = 0; i < node->btp_len; i++){
        volumes[i] = node->boxes_to_place[i].xlen * 
                    node->boxes_to_place[i].ylen * 
                    node->boxes_to_place[i].zlen;
    }
    qsort(volumes, node->btp_len, sizeof(int), comp1);

    //1.2) compute the capacity available
    //1.2.1) compute the total volume of the container
    int capacity = cont_x*cont_y*cont_z;
    //1.2.2) remove the volume of the boxes already contained
    capacity = capacity_minus_placed_boxes(capacity, node->boxes_placed, node->bp_len);
    //1.2.3) remove also the volume of the points that cannot contain any box
    int* points_to_remove = find_unavailable_points(node->extreme_points, node->ep_len,
                                            node->boxes_to_place, node->btp_len);
    if(points_to_remove != NULL){
        int n = points_to_remove[0];
        for(int i = 1; i < n+1; i++){
            int index = points_to_remove[i];    //remember that points_to_remove contains INDEXES!
            capacity -= node->extreme_points[index].width *
                        node->extreme_points[index].height *
                        node->extreme_points[index].depth;
        }

        //1.2.4) since we (maybe) found out some points that CAN'T be occupied by any box, we can
        //remove them from the extreme_points of this subproblem, to make smaller subproblems.
        int* tmp = malloc(points_to_remove[0] * sizeof(int));
        for(int i = 0; i < points_to_remove[0]; i++){
            tmp[i] = points_to_remove[i+1];
        }
        exclude_unavailable_points(&(node->extreme_points), node->ep_len,
                            tmp, points_to_remove[0]);
        node->ep_len = node->ep_len - points_to_remove[0];
        free(points_to_remove);

    }
    


    


    int dual_bound = solve_knapsack_0_1_v3(volumes, node->btp_len, capacity);


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
    int volume_occupied = 0;
    int* volumes_of_boxes_placed = malloc((n_boxes_placed+1)*sizeof(int));
    for(int i = 0; i < n_boxes_placed+1; i++){
        volumes_of_boxes_placed[i] = new_boxes_placed[i].xlen * 
                                    new_boxes_placed[i].ylen * 
                                    new_boxes_placed[i].zlen;
        volume_occupied += volumes_of_boxes_placed[i];
    }

        printf("il volume della scatola messa è %d\n", volumes_of_boxes_placed[0]);


    int dual_bound = get_dual_bound_using_kp_0_1(volumes_of_boxes_placed, n_boxes_placed+1,
                                                (cont_x*cont_y*cont_z)-volume_occupied);

    

}










int comp1(const void * a, const void * b){
    int a1 = *(const int*) a;
    int b1 = *(const int*) b;
    return a1 - b1;
}

