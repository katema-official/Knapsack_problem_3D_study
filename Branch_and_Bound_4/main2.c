#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "utils.h"
#include "boxes.h"
#include "extreme_points.h"
#include "knapsack_0_1_solver.h"
#include "advanced_extreme_points.h"
#include "advanced_extreme_points_plus.h"


//define container dimensions
int cont_x = 20;//600;//860;//100;//800;
int cont_y = 10;//500;//322;//100;//700;
int cont_z = 15;//700;//590;//100;//1000;

node* head = NULL;

int* volumes;   //array that keeps the volumes of the boxes. If, in position i of the boxes array,
//there is the i-th box, then in this array, in position i, there is its volume.
int n_volumes;



int primal_bound = 0;   //volume occupied by the best solution found so far
box* optimal_feasible_solution_found;    //the optimal solution found so far. It's an array of boxes
//placed fully inside the container (the primal_bound above is the total volume occupied by
//these boxes)
int n_optimal_feasible_solution_found = 0;

void print_results(box* all_boxes, int n_boxes, char** boxes_names);

node* generate_new_node(point* extreme_points, int n_extreme_points, 
                        box* boxes_placed, int n_boxes_placed, 
                        box* boxes_to_place, int n_boxes_to_place,
                        box* volumes_to_exclude, int n_volumes_to_exclude,
                        box new_box, int i_ep, int i_btp);

void check_then_update_primal_bound(box* boxes, int n, int total_volume_of_boxes);
void initialize_dual_bound();

void explore_node();

int comp1(const void * a, const void * b);


int main(){
    srand(time(NULL));

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if(DEBUG_PROGRESS) reset_progress_file();
    //initialize_dual_bound();
    
    int n_boxes = 0;
    box* boxes;
    char** boxes_names; //I keep the name of the boxes in a separate array.

    int read_from_file = 1;
    if(read_from_file){
        //adapter from https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
        FILE* f = fopen("./input4.txt", "r");
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
    box* original_boxes_copy = malloc(n_boxes*sizeof(box));
    copy_boxes(&original_boxes_copy, boxes, n_boxes);
    if(order_boxes){
        qsort(boxes, n_boxes, sizeof(box), comparator_boxes_volume);
    }

    for(int i = 0; i < n_boxes; i++){
        printf("%d %d %d %d %d %d\n", boxes[i].xlen, boxes[i].ylen, boxes[i].zlen,
                boxes[i].x0, boxes[i].y0, boxes[i].z0);
    }

    char** ordered_boxes_names = (char**) malloc(n_boxes*sizeof(char*));
    for(int i = 0; i < n_boxes; i++){
        int index = -1;
        box b_ordered = boxes[i];
        for(int j = 0; j < n_boxes; j++){
            if(b_ordered.xlen == original_boxes_copy[j].xlen && 
                b_ordered.ylen == original_boxes_copy[j].ylen &&
                b_ordered.zlen == original_boxes_copy[j].zlen){
                    index = j;
                    original_boxes_copy[j].xlen = -1;
                    original_boxes_copy[j].ylen = -1;
                    original_boxes_copy[j].zlen = -1;
                    break;
            }
        }
        int x = strlen(boxes_names[index]) + 1;
        ordered_boxes_names[i] = malloc(x * sizeof(char));
        strcpy(ordered_boxes_names[i], boxes_names[index]);
        free(boxes_names[index]);
    }
    free(boxes_names);
    boxes_names = ordered_boxes_names;
    free(original_boxes_copy);




    //****************************************************
    //**************BRANCH AND BOUND START****************
    //****************************************************
    
    //when a node is detected to be infeasible or its dual bound is lower
    //than the actual primal bound (the super-optimal solution of the subproblem
    //is worse than the best known feasible solution), the node is closed
    /*int max_number_of_boxes_placeable = 0;
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
    }*/

    //---------------------------------------------------------------------
    //-------------------------BFS INITIALIZATION--------------------------
    //---------------------------------------------------------------------
    //we have to initialize the list of subproblems by creating 6*n_boxes problems:
    //each of them represents the fact that we are placing a box in the first
    //extreme point, in any of the 6 possible configurations
    for(int i = n_boxes-1; i >= 0; i--){    //BEFORE THE ORDER WAS REVERSED AND IT REQUIRED A LOT MORE TIME!!! (of course but still cool)
        int** rotations = rotations_of_box(boxes[i]);
        int n_of_rotations = rotations[0][0];
        for(int j = 1; j < n_of_rotations+1; j++){
            //if the box, in that rotation, is inside the container...
            if(rotations[j][0] <= cont_x && rotations[j][1] <= cont_y && rotations[j][2] <= cont_z){
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
                this_node->extreme_points[0].spawnpoint = right_of_box;

                this_node->extreme_points[1].x = 0;
                this_node->extreme_points[1].y = this_box.y0 + this_box.ylen;
                this_node->extreme_points[1].z = 0;
                this_node->extreme_points[1].width = cont_x;
                this_node->extreme_points[1].height = cont_y - this_node->extreme_points[1].y;
                this_node->extreme_points[1].depth = cont_z;
                this_node->extreme_points[1].spawnpoint = top_of_box;

                this_node->extreme_points[2].x = 0;
                this_node->extreme_points[2].y = 0;
                this_node->extreme_points[2].z = this_box.z0 + this_box.zlen;
                this_node->extreme_points[2].width = cont_x;
                this_node->extreme_points[2].height = cont_y;
                this_node->extreme_points[2].depth = cont_z - this_node->extreme_points[2].z;
                this_node->extreme_points[2].spawnpoint = front_of_box;

                //initialize volumes to exclude. Initlially, there are none.
                this_node->volumes_to_exclude = NULL;
                this_node->vte_len = 0;

                //initialize the successor
                this_node->succ = head;

                //this is a LIFO ordering
                head = this_node;
            }

        }
        for(int i = 0; i < n_of_rotations+1; i++){
            free(rotations[i]);
        }
        free(rotations);
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
        explore_node();
    }

    print_results(boxes, n_boxes, boxes_names);

    free(volumes);
    free_kp_sol_node_list();
    for(int i = 0; i < n_boxes; i++){
        free(boxes_names[i]);
    }


    free(boxes_names);
    free(boxes);
    free(optimal_feasible_solution_found);

    end = clock();
    cpu_time_used = (((double) (end-start)) / CLOCKS_PER_SEC);
    int mins = cpu_time_used/60;
    int secs = ((int)cpu_time_used)%60;
    printf("TOTAL TIME: %d.%d\n", mins, secs);

}


void print_results(box* all_boxes, int n_boxes, char** boxes_names){
    FILE* f = fopen("./results/result3.txt", "w");
    fprintf(f, "%d\n", n_optimal_feasible_solution_found);
    for(int i = 0; i < n_optimal_feasible_solution_found; i++){
        int index_name = -1;
        //I want to find the name of the boxes of the optimal solution
        for(int j = 0; j < n_boxes; j++){
            box b = all_boxes[j];
            int** rots = rotations_of_box(b);
            int n_of_rotations = rots[0][0];
            for(int k = 1; k < n_of_rotations+1; k++){
                if(rots[k][0] == optimal_feasible_solution_found[i].xlen &&
                    rots[k][1] == optimal_feasible_solution_found[i].ylen &&
                    rots[k][2] == optimal_feasible_solution_found[i].zlen){
                        //the box is the same
                        index_name = j;
                        b.xlen = -1;
                        b.ylen = -1;
                        b.zlen = -1;
                        break;
                }
            }
            for(int k = 0; k < n_of_rotations+1; k++){
                free(rots[k]);
            }
            free(rots);
            if(index_name != -1){
                break;
            }
        }


        if(boxes_names!=NULL) fprintf(f, "%d %d %d %d %d %d %s\n", 
                                        optimal_feasible_solution_found[i].xlen,
                                        optimal_feasible_solution_found[i].ylen,
                                        optimal_feasible_solution_found[i].zlen,
                                        optimal_feasible_solution_found[i].x0,
                                        optimal_feasible_solution_found[i].y0,
                                        optimal_feasible_solution_found[i].z0,
                                        boxes_names[index_name]);
    }
    fprintf(f, "---\n");
    fclose(f);
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
void explore_node(){

    //head is pointing to this node right now, wich will be freed after this function.
    //So, let's move head ahead (we will if necessary move it if new subproblems will
    //be generated)
    node* current_node = head;
    head = head->succ;


    //0) update, if possible, the primal bound
    int v = 0;
    for(int i = 0; i < current_node->bp_len; i++){
        v += current_node->boxes_placed[i].xlen * 
                current_node->boxes_placed[i].ylen * 
                current_node->boxes_placed[i].zlen;
    }
    check_then_update_primal_bound(current_node->boxes_placed, current_node->bp_len, v);

    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //1) COMPUTE DUAL BOUND
    int dual_bound = 0;
    //1.1) compute the capacity available

    int capacity;
    //1.1.1) compute the total volume of the container
    capacity = cont_x*cont_y*cont_z;
    //1.1.2) remove the volume of the boxes already contained
    capacity = capacity_minus_placed_boxes(capacity, current_node->boxes_placed, current_node->bp_len);

    //1.1.3) find new points that cannot contain any box
    int* points_unavailable = find_unavailable_points(current_node->extreme_points, current_node->ep_len,
                                            current_node->boxes_to_place, current_node->btp_len);
    if(points_unavailable != NULL){
        int n = points_unavailable[0];
        if(n == current_node->ep_len){
            //if the points to remove are all the extreme points of this instance, don't
            //even bother computing anything except the primal bound: this instance can't
            //generate new ones.
            if(DEBUG_PROGRESS) append_progress_file_partial_solution(current_node->boxes_placed, current_node->bp_len,
                                                current_node->volumes_to_exclude, current_node->vte_len);
            if(DEBUG_PROGRESS) append_progress_file_solution_separator(2, dual_bound, primal_bound);
            free(points_unavailable);
            free(current_node->boxes_placed);
            free(current_node->boxes_to_place);
            free(current_node->extreme_points);
            free(current_node->volumes_to_exclude);
            free(current_node);
            //this node can be closed
            if(DEBUG_3) printf("no points!\n");
            return;
        }

        point* pts_tmp = malloc(n*sizeof(point));
        for(int i = 1; i < n+1; i++){
            int index = points_unavailable[i];    //remember that points_unavailable contains INDEXES!
            pts_tmp[i-1] = current_node->extreme_points[index];
        }
        
        box* volumes_induced_by_points_unavailable = get_unavailable_points_volume(current_node->boxes_placed,
                                                            current_node->bp_len, pts_tmp, n, current_node->volumes_to_exclude, current_node->vte_len);
        int n_boxes_volumes_unavailable = volumes_induced_by_points_unavailable[0].x0;  //check the function above to see why this is ok
        
        free(pts_tmp);

        //1.1.4) let's add, to this sub-problem (so that this will be propagated to all subsequent
        //sub-problems), the box volumes to be excluded
        box* new_volumes_to_exclude = malloc(n_boxes_volumes_unavailable * sizeof(box));
        for(int i = 0; i < n_boxes_volumes_unavailable; i++){
            copy_box(&(new_volumes_to_exclude[i]), volumes_induced_by_points_unavailable[i+1]);
        }
        free(volumes_induced_by_points_unavailable);    //i did this step of copy just to have clearer code in the following.
                                                        //It's not gret to have, in an array, both its length AND its content.

        add_new_boxes_volumes_unavailable(&(current_node->volumes_to_exclude), &(current_node->vte_len),
                                            new_volumes_to_exclude, n_boxes_volumes_unavailable);
        free(new_volumes_to_exclude);

        //1.1.5) let's try to remove even more volumes, induced by the current unavailable volumes
        deduce_even_more_unavailable_points_volume(&(current_node->volumes_to_exclude), &(current_node->vte_len), 
                current_node->boxes_placed, current_node->bp_len);

        //1.1.6) since we (maybe) found out some points that CAN'T be occupied by any box, we can
        //remove them from the extreme_points of this subproblem, to make smaller subproblems.
        int* tmp = malloc(n * sizeof(int));
        for(int i = 0; i < n; i++){
            tmp[i] = points_unavailable[i+1];
        }
        exclude_unavailable_points(&(current_node->extreme_points), current_node->ep_len,
                            tmp, n);
        current_node->ep_len = current_node->ep_len - points_unavailable[0];
        free(points_unavailable);
        free(tmp);
    }
    if(capacity < 0){
        capacity = 0;
        printf("NOOOO\n");
    }

    //1.1.7) remove the volume of the boxes volumes unavailable (to exclude)
    for(int i = 0; i < current_node->vte_len; i++){
        capacity -= current_node->volumes_to_exclude[i].xlen * 
                    current_node->volumes_to_exclude[i].ylen * 
                    current_node->volumes_to_exclude[i].zlen; 
    }

    //1.2) solve the knapsack problem
    int* volumes_of_placed_boxes = malloc(current_node->bp_len * sizeof(int));
    for(int i = 0; i < current_node->bp_len; i++){
        volumes_of_placed_boxes[i] = current_node->boxes_placed[i].xlen *
                                    current_node->boxes_placed[i].ylen *
                                    current_node->boxes_placed[i].zlen;
    }
    qsort(volumes_of_placed_boxes, current_node->bp_len, sizeof(int), comp1);
    dual_bound = get_dual_bound_using_kp_0_1_bfs(volumes_of_placed_boxes, current_node->bp_len,
                            capacity, current_node->boxes_to_place, current_node->btp_len);
    if(DEBUG_3) printf("dual bound here = %d, primal bound = %d\n", dual_bound, primal_bound);
    free(volumes_of_placed_boxes);
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------

    //we now have the dual bound. If it is worse (lower, in the knapsack problem)
    //than the volume occupied by the optimal feasible solution found so far (the primal bound),
    //we can close this node and avoid exploring all subsequent sub-nodes (sub-problems)
    if(dual_bound > primal_bound){
        if(DEBUG_PROGRESS) append_progress_file_partial_solution(current_node->boxes_placed, current_node->bp_len,
                                                current_node->volumes_to_exclude, current_node->vte_len);
        if(DEBUG_PROGRESS) append_progress_file_solution_separator(0, dual_bound, primal_bound);
        if(DEBUG_3) printf("ok, explore\n");
        //it is still worth to explore this sub-problems (and so its (feasible) sub-problems)
        //So, we now have to generate all the feasible sub-problems from this.
        //This is done in the following way:
        //-for each extreme_point available
        //--for each box that still needs to be placed
        //---for each rotation of that box
        //---check: can you place this box, in this point, with this rotation, without
        //   overlapping with any previously placed box, and going outside of the container? 
        //   -YES: generate new node
        //   -NO: skip
        for(int p_index = 0; p_index < current_node->ep_len; p_index++){
            point p = current_node->extreme_points[p_index];
            for(int b_index = current_node->btp_len - 1; b_index >= 0; b_index--){   //IN THIS WAY WE PREFER FIRST THE SOLUTION WITH BIGGER BOXES
                box b = current_node->boxes_to_place[b_index];
                int** rots = rotations_of_box(b);
                int n_of_rotations = rots[0][0];
                for(int r_index = 1; r_index < n_of_rotations+1; r_index++){
                    int* current_rotation = rots[r_index];
                    box b_copy;
                    copy_box(&b_copy, b);

                    b_copy.x0 = p.x;
                    b_copy.y0 = p.y;
                    b_copy.z0 = p.z;
                    b_copy.xlen = current_rotation[0];
                    b_copy.ylen = current_rotation[1];
                    b_copy.zlen = current_rotation[2];

                    //first check: does the box go outside of the container?
                    int not_ok = 0;
                    if(!is_box_outside_container(b_copy)){
                        //if not: does it collide with any other previously placed box?
                        for(int i = 0; i < current_node->bp_len; i++){
                            not_ok += do_boxes_overlap(b_copy, current_node->boxes_placed[i]);
                        }
                        if(not_ok == 0){
                            //the box can be placed
                            node* new_node = generate_new_node(current_node->extreme_points, current_node->ep_len,
                                            current_node->boxes_placed, current_node->bp_len,
                                            current_node->boxes_to_place, current_node->btp_len,
                                            current_node->volumes_to_exclude, current_node->vte_len,
                                            b_copy, p_index, b_index);
                            //append new sub-problem to the list of sub-problems to explore
                            new_node->succ = head;
                            head = new_node;
                        }
                    }
                }
                for(int i = 0; i < n_of_rotations+1; i++){
                    free(rots[i]);
                }
                free(rots);
                
            }
        }


    }else{
        //this node can be closed
        if(DEBUG_PROGRESS) append_progress_file_partial_solution(current_node->boxes_placed, current_node->bp_len,
                                                current_node->volumes_to_exclude, current_node->vte_len);
        if(DEBUG_PROGRESS) append_progress_file_solution_separator(1, dual_bound, primal_bound);
        if(DEBUG_3) printf("close!\n");
    }
    free(current_node->boxes_placed);
    free(current_node->boxes_to_place);
    free(current_node->extreme_points);
    free(current_node->volumes_to_exclude);
    free(current_node);
    

}

//i_ep = the index in extreme_points of the point used just now to place the box
//i_btp = index in boxes_to_place of the box just placed
node* generate_new_node(point* extreme_points, int n_extreme_points, 
                        box* boxes_placed, int n_boxes_placed, 
                        box* boxes_to_place, int n_boxes_to_place,
                        box* volumes_to_exclude, int n_volumes_to_exclude,
                        box new_box, int i_ep, int i_btp){


    //copy the boxes placed, adding the new box placed
    box* new_boxes_placed = malloc((n_boxes_placed+1) * sizeof(box));
    copy_boxes(&new_boxes_placed, boxes_placed, n_boxes_placed);
    new_boxes_placed[n_boxes_placed] = new_box;

    //create the three new points, but check if some of them are redundant
    point p1;
    point p2;
    point p3;
    p1.x = new_box.x0 + new_box.xlen;
    p1.y = new_box.y0;
    p1.z = new_box.z0;
    p1.width = cont_x - p1.x;
    p1.height = cont_y - p1.y;
    p1.depth = cont_z - p1.z;
    p1.spawnpoint = right_of_box;

    p2.x = new_box.x0;
    p2.y = new_box.y0 + new_box.ylen;
    p2.z = new_box.z0;
    p2.width = cont_x - p2.x;
    p2.height = cont_y - p2.y;
    p2.depth = cont_z - p2.z;
    p2.spawnpoint = top_of_box;

    p3.x = new_box.x0;
    p3.y = new_box.y0;
    p3.z = new_box.z0 + new_box.zlen;
    p3.width = cont_x - p3.x;
    p3.height = cont_y - p3.y;
    p3.depth = cont_z - p3.z;
    p3.spawnpoint = front_of_box;
    project_point_down(&p1, new_boxes_placed, n_boxes_placed+1);
    //we don't need to project p2 down: it's by definition on top of a box
    project_point_down(&p3, new_boxes_placed, n_boxes_placed+1);

    int p1_valid = is_point_not_redundant(p1, extreme_points, n_extreme_points);
    int p3_valid = is_point_not_redundant(p3, extreme_points, n_extreme_points);


    //copy the extreme points by removing the point in which the box has been placed, 
    //then add (up to) three new points
    point* new_extreme_points = get_copy_points_except_one(extreme_points, n_extreme_points, i_ep, 
                                        1+p1_valid+p3_valid);   //1 because the top point is always added
    
    new_extreme_points[n_extreme_points-1] = p2;
    if(p1_valid && p3_valid){
        new_extreme_points[n_extreme_points] = p1;
        new_extreme_points[n_extreme_points+1] = p3;
        update_point_dimensions(&new_extreme_points[n_extreme_points], new_boxes_placed, n_boxes_placed+1);
        update_point_dimensions(&new_extreme_points[n_extreme_points+1], new_boxes_placed, n_boxes_placed+1);
    }
    if(p1_valid && !p3_valid){
        new_extreme_points[n_extreme_points] = p1;
        update_point_dimensions(&new_extreme_points[n_extreme_points], new_boxes_placed, n_boxes_placed+1);
    }
    if(!p1_valid && p3_valid){
        new_extreme_points[n_extreme_points] = p3;
        update_point_dimensions(&new_extreme_points[n_extreme_points], new_boxes_placed, n_boxes_placed+1);
    }

    for(int i = 0; i < n_extreme_points; i++){
        update_point_dimensions(&new_extreme_points[i], new_boxes_placed, n_boxes_placed+1);
    }



    //copy the boxes to place removing the box placed right now
    box* new_boxes_to_place = get_copy_boxes_except_one(boxes_to_place, n_boxes_to_place, i_btp);


    //copy the boxes volumes unavailable (to exclude). This will propagate the knowledge of the
    //unavailable spaces also to the sub-nodes, improving their lower bounds.
    box* volumes_to_exclude_copy = malloc(n_volumes_to_exclude * sizeof(box));
    copy_boxes(&(volumes_to_exclude_copy), volumes_to_exclude, n_volumes_to_exclude);

    node* new_node = malloc(sizeof(node));
    new_node->bp_len = n_boxes_placed+1;
    new_node->boxes_placed = new_boxes_placed;
    new_node->btp_len = n_boxes_to_place-1;
    new_node->boxes_to_place = new_boxes_to_place;
    new_node->ep_len = n_extreme_points + p1_valid + p3_valid;
    new_node->extreme_points = new_extreme_points;
    new_node->volumes_to_exclude = volumes_to_exclude_copy;
    new_node->vte_len = n_volumes_to_exclude;
    
    return new_node;

}


//function that: updates the best feasible solution found so far with "boxes",
//IF "total_volume_of_boxes" is a better upper_bound than the current upper_bound
//THEN updates the primal_bound with "total_volume_of_boxes" (the volume occupied
//by the "boxes") and the optimal_feasible_solution_found with "boxes"

void check_then_update_primal_bound(box* boxes, int n, int total_volume_of_boxes){
    if(total_volume_of_boxes > primal_bound){
        if(optimal_feasible_solution_found) free(optimal_feasible_solution_found);
        optimal_feasible_solution_found = malloc(n*sizeof(box));
        copy_boxes(&optimal_feasible_solution_found, boxes, n);
        primal_bound = total_volume_of_boxes;
        n_optimal_feasible_solution_found = n;
        if(DEBUG_1) printf("new partial optimal solution! It's:\n");
        if(DEBUG_1) print_results(boxes, n, NULL);
    }
}

//function that initializes the primal bound to a value given by the user
//(it's the user's responsibility to grant that the given primal bound is
//a real primal bound)
void initialize_dual_bound(){
    FILE* f = fopen("./results/primal_bound_start.txt", "r");
    char* line = NULL;
    size_t len = 0;
    size_t read;
    char delim[] = " ";
    int box_index = 0;

    read = getline(&line, &len, f);
    int n = atoi(line);

    box* initial_boxes = malloc(n*sizeof(box));
    int initial_total_volume = 0;

    while ((read = getline(&line, &len, f)) != -1) {
        //protocol of a line: xlen ylen zlen x0 y0 z0 name\n
        char** info_of_a_box = (char**) malloc(7*sizeof(char*));
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
        b.x0 = atoi(info_of_a_box[3]);
        b.y0 = atoi(info_of_a_box[4]);
        b.z0 = atoi(info_of_a_box[5]);
        initial_boxes[box_index] = b;
        initial_total_volume += b.xlen*b.ylen*b.zlen;
        box_index++;
        free(info_of_a_box);
    }
    fclose(f);
    if(line){free(line);}

    check_then_update_primal_bound(initial_boxes, n, initial_total_volume);


}





int comp1(const void * a, const void * b){
    int a1 = *(const int*) a;
    int b1 = *(const int*) b;
    return a1 - b1;
}



