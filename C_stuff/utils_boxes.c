#include "utils_boxes.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#define max(a,b) (((a) > (b)) ? (a) : (b))

void debug_print(int n_boxes, box* boxes, int* a, int* b, int* c){
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
}

int* random_permutation_1_to_n(int n) {
    int* r = malloc(n * sizeof(int));
    // initial range of numbers
    for(int i=0;i<n;++i){
        r[i]=i+1;
    }
    // shuffle
    for (int i = n-1; i >= 0; --i){
        //generate a random number [0, n-1]
        int j = rand() % (i+1);

        //swap the last element with element at random index
        int temp = r[i];
        r[i] = r[j];
        r[j] = temp;
    }
    return r;
}

int comparator_boxes_volume(const void* a, const void* b){
    box b1 = *(const box*) a;
    box b2 = *(const box*) b;
    int vol_1 = b1.xlen*b1.ylen*b1.zlen;
    int vol_2 = b2.xlen*b2.ylen*b2.zlen;
    if(vol_1 > vol_2){
        return -1;
    }else if(vol_1 < vol_2){
        return 1;
    }
    return 0;
}

void permute_box_dimensions(box* this_box, int perm){
    //perm dictates the new permutation of the box.
    //If -1 is given, a random one is generated
    if(perm == -1){
        perm = rand() % 6;
    }
    int _xlen = (*this_box).xlen;
    int _ylen = (*this_box).ylen;
    int _zlen = (*this_box).zlen;
    switch(perm){
        case 0:
            //no change
        break;
        case 1:
            (*this_box).xlen = _xlen;
            (*this_box).ylen = _zlen;
            (*this_box).zlen = _ylen;
        break;
        case 2:
            (*this_box).xlen = _ylen;
            (*this_box).ylen = _xlen;
            (*this_box).zlen = _zlen;
        break;
        case 3:
            (*this_box).xlen = _ylen;
            (*this_box).ylen = _zlen;
            (*this_box).zlen = _xlen;
        break;
        case 4:
            (*this_box).xlen = _xlen;
            (*this_box).ylen = _zlen;
            (*this_box).zlen = _ylen;
        break;
        case 5:
            (*this_box).xlen = _zlen;
            (*this_box).ylen = _ylen;
            (*this_box).zlen = _xlen;
        break;
        default:
        break;
    }
}

void swap_in_array(int** sequence, int i, int j){
    int tmp = (*sequence)[i];
    (*sequence)[i] = (*sequence)[j];
    (*sequence)[j] = tmp;
}

void get_neighbour(box** boxes_neighbour, int n_boxes, int** current_a, int** current_b, int** current_c){

    printf("before neighbour:\n");
    debug_print(n_boxes, *boxes_neighbour, *current_a, *current_b, *current_c);


    //there are several ways to generate neighbours for the current solution, but I'll try to stick
    //as much as possible to Hanan Mostaghimi Ghomi's article. Also, as I interpreted, the "dimensions swap"
    //can happen only on none, one or both of the boxes that are being swapped in the sequences.
    //The chosen boxes are the i-th and the j-th that we'll randomly generate. 
    //in order to generate them, I'll use a procedure that is a bit more costly but for which I'm sure
    //that always a fixed number of steps are necessary to generate i and j (I'm already thinking to
    //possible divergence in CUDA)
    int* perm = random_permutation_1_to_n(n_boxes);
    int i = perm[0];
    int j = perm[1];

    int index_i_a;
    int index_j_a;
    int index_i_b;
    int index_j_b;
    int index_i_c;
    int index_j_c;
    for(int k = 0; k < n_boxes; k++){
        if((*current_a)[k] == i){
            index_i_a = k;
        }else if((*current_a)[k] == j){
            index_j_a = k;
        }

        if((*current_b)[k] == i){
            index_i_b = k;
        }else if((*current_b)[k] == j){
            index_j_b = k;
        }

        if((*current_c)[k] == i){
            index_i_c = k;
        }else if((*current_c)[k] == j){
            index_j_c = k;
        }
    }

    //The neighbourhood of each solution is defined as one of these five permutations:
    //either exchange two boxes from one of the sequences; exchange two boxes in
    //sequences A and B; exchange two boxes in sequences A and C; exchange two boxes
    //in sequences C and B; or exchange two boxes in all sequences.
    int choice = rand() % 5;
    switch(choice){
        case 0:
            //exchange two boxes from one of the sequences.
            int list_chosen = rand() % 3;
            switch(list_chosen){
                case 0:
                    swap_in_array(current_a, index_i_a, index_j_a);
                break;
                case 1:
                    swap_in_array(current_b, index_i_b, index_j_b);
                break;
                case 2:
                    swap_in_array(current_c, index_i_c, index_j_c);
                break;
                default:
                break;
            }
        break;
        case 1:
            swap_in_array(current_a, index_i_a, index_j_a);
            swap_in_array(current_b, index_i_b, index_j_b);
        break;
        case 2:
            swap_in_array(current_a, index_i_a, index_j_a);
            swap_in_array(current_c, index_i_c, index_j_c);
        break;
        case 3:
            swap_in_array(current_b, index_i_b, index_j_b);
            swap_in_array(current_c, index_i_c, index_j_c);
        break;
        case 4:
            swap_in_array(current_a, index_i_a, index_j_a);
            swap_in_array(current_b, index_i_b, index_j_b);
            swap_in_array(current_c, index_i_c, index_j_c);
        break;
        default:
        break;
    }

    //let's also consider that some boxes might rotate. In this implementation, only the two boxes chosen
    //before CAN rotate, and:
    //none of them rotates OR one of the two rotates OR bot of them rotate
    int rotation_chosen = rand() % 4;
    switch(rotation_chosen){
        case 1:
            permute_box_dimensions(&(*boxes_neighbour)[i], -1);
        break;
        case 2:
            permute_box_dimensions(&(*boxes_neighbour)[j], -1);
        break;
        case 3:
            permute_box_dimensions(&(*boxes_neighbour)[i], -1);
            permute_box_dimensions(&(*boxes_neighbour)[j], -1);
        break;
        default:
        break;
    }

    printf("neighbour generated:\n");
    debug_print(n_boxes, *boxes_neighbour, *current_a, *current_b, *current_c);

}

//function that, given a sequence (A, B or C), returns:
//1: if i is before j in seq
//0: if i is after j in seq
//Note that i, j are to considered boxes: So, if we have our array of boxes, built 
//at the beginning of the problem, in which the boxes have a fixed position, this 
//means that they have a fixed and unique identifier, that is the index of the array 
//that contains them (box 1 is the first in this fixed array, box 2 is the second 
//in this fixed array...)
int give_X_ij(int* seq, int len, int i, int j){
    if(i == j){exit(-1);} //can't happen
    int index_i = 0;
    int index_j = 0;
    for(int k = 0; k < len; k++){
        if(seq[k] == i){
            index_i = k;
        }else if(seq[k] == j){
            index_j = k;
        }
    }
    if(index_i<index_j){
        return 1;
    }else{
        return 0;
    }
}

int enforce_vertical_stability(int x_i, int y_i, int z_i, int xlen, int ylen, int zlen, int* P_y, int P_y_len, box* boxes, int n_boxes){
    int max_h = 0;
    int k;
    for(int i = 0; i < P_y_len; i++){
        k = P_y[i];
        int k_x0 = boxes[k].x0;
        int k_z0 = boxes[k].z0;
        int k_x1 = boxes[k].x0 + boxes[k].xlen;
        int k_z1 = boxes[k].z0 + boxes[k].zlen;
        //the box k is below the box we have to place if one of its four points
        //falls under the lower surface of the box to be placed
        if ((k_x0 >= x_i && k_x0 < x_i + xlen && k_z0 >= z_i && k_z0 < z_i + zlen) ||
            (k_x1 > x_i && k_x1 <= x_i + xlen && k_z0 >= z_i && k_z0 < z_i + zlen) ||
            (k_x0 >= x_i && k_x0 < x_i + xlen && k_z1 > z_i && k_z1 <= z_i + zlen) ||
            (k_x1 > x_i && k_x1 <= x_i + xlen && k_z1 > z_i && k_z1 <= z_i + zlen)){
                if(boxes[k].y0 + boxes[k].ylen > max_h){
                    max_h = boxes[k].y0 + boxes[k].ylen;
                }
        }else{
            //at the same time, the box k is below the box we have to place if one of the
            //four points of the box we have to place falls above the box k
            int btp_x0 = x_i;    //btp = box to place
            int btp_z0 = z_i;
            int btp_x1 = x_i + xlen;
            int btp_z1 = z_i + zlen;
            if((btp_x0 >= k_x0 && btp_x0 < k_x1 && btp_z0 >= k_z0 && btp_z0 < k_z1) ||
                    (btp_x1 > k_x0 && btp_x1 <= k_x1 && btp_z0 >= k_z0 && btp_z0 < k_z1) ||
                    (btp_x0 >= k_x0 && btp_x0 < k_x1 && btp_z1 > k_z0 && btp_z1 <= k_z1) ||
                    (btp_x1 > k_x0 && btp_x1 <= k_x1 && btp_z1 > k_z0 && btp_z1 <= k_z1)){
                        if(boxes[k].y0 + boxes[k].ylen > max_h){
                            max_h = boxes[k].y0 + boxes[k].ylen;
                        }
            }
        }
    }

    return max_h;

}

//function that places the boxes according to the three sequences
void place_boxes_sequence_triples(int* a, int* b, int* c, box** boxes, int n_boxes){
    int first = b[0];
    (*boxes)[first].x0 = 0;
    (*boxes)[first].y0 = 0;
    (*boxes)[first].z0 = 0;

    //array for remembering the boxes already placed (that is, the boxes for which we
    //have chosen x0, y0 and z0)
    int* placed_boxes = malloc(n_boxes*sizeof(int));
    int counted_boxes = 1;  //effective length of "placed_boxes". Useful also for P_x,y,z
    /*for(int i = 0; i < n_boxes; i++){
        placed_boxes[i] = -1;
    }*/
    placed_boxes[0] = first;

    int* P_x = malloc(n_boxes*sizeof(int));
    int* P_y = malloc(n_boxes*sizeof(int));
    int* P_z = malloc(n_boxes*sizeof(int));
    int P_x_counter = 0;
    int P_y_counter = 0;
    int P_z_counter = 0;

    for(int k = 1; k < n_boxes; k++){
        int current = b[k]; //the box we want to place right now
        for(int q = 0; q < counted_boxes; q++){
            int old = placed_boxes[q];
            //In P_x we put the boxes (already placed) that are to the left than the new one
            if(give_X_ij(a, n_boxes, old, current) && 
                give_X_ij(b, n_boxes, old, current) && 
                !give_X_ij(c, n_boxes, old, current)){
                    P_x[P_x_counter] = old;
                    P_x_counter++;
            }
            //In P_y we put the boxes (already placed) that are below the new one
            if(!give_X_ij(a, n_boxes, old, current) && 
                give_X_ij(b, n_boxes, old, current) && 
                give_X_ij(c, n_boxes, old, current)){
                    P_y[P_y_counter] = old;
                    P_y_counter++;
            }
            //In P_z we put the boxes (already placed) that are behind the new one
            if((!give_X_ij(a, n_boxes, old, current) && 
                give_X_ij(b, n_boxes, old, current) && 
                !give_X_ij(c, n_boxes, old, current)) || 
                (give_X_ij(a, n_boxes, old, current) && 
                give_X_ij(b, n_boxes, old, current) && 
                give_X_ij(c, n_boxes, old, current))){
                    P_z[P_z_counter] = old;
                    P_z_counter++;
            }
        }

        int x_i = 0;
        int y_i = 0;
        int z_i = 0;

        for(int kk = 0; kk < P_x_counter; kk++){
            int index_of_box = P_x[kk];
            box actual_box = (*boxes)[index_of_box];
            if(actual_box.x0 + actual_box.xlen > x_i){
                x_i = actual_box.x0 + actual_box.xlen;
            }
        }
        for(int kk = 0; kk < P_y_counter; kk++){
            int index_of_box = P_y[kk];
            box actual_box = (*boxes)[index_of_box];
            if(actual_box.y0 + actual_box.ylen > y_i){
                y_i = actual_box.y0 + actual_box.ylen;
            }
        }
        for(int kk = 0; kk < P_z_counter; kk++){
            int index_of_box = P_z[kk];
            box actual_box = (*boxes)[index_of_box];
            if(actual_box.z0 + actual_box.zlen > z_i){
                z_i = actual_box.z0 + actual_box.zlen;
            }
        }

        (*boxes)[current].x0 = x_i;
        (*boxes)[current].z0 = z_i;

        //now for the vertical stability: let's "push" the boxes below as possible as we can
        y_i = enforce_vertical_stability(x_i, y_i, z_i,
                                         (*boxes)[current].xlen, (*boxes)[current].ylen, (*boxes)[current].zlen, 
                                         P_y, P_y_counter, *boxes, n_boxes);

        (*boxes)[current].y0 = y_i;

        P_x_counter = 0;
        P_y_counter = 0;
        P_z_counter = 0;

        placed_boxes[counted_boxes] = current;
        counted_boxes++;

    }


}

void sa_make_a_step(box** boxes, int n_boxes, int* temperature, int alpha, int beta, 
                    int** current_a, int** current_b, int** current_c, 
                    int** best_a, int** best_b, int** best_c, int* best_volume, box** boxes_neighbour){
    //make a copy of boxes, a, b and c
    memcpy(&(*boxes_neighbour), &(*boxes), sizeof(*boxes));
    memcpy(&(*current_a), &(*best_a), sizeof(*best_a));
    memcpy(&(*current_b), &(*best_b), sizeof(*best_b));
    memcpy(&(*current_c), &(*best_c), sizeof(*best_c));
    
    //get a neighbour. When this function is called, boxes_neighbour, current_a, current_b and current_c are
    //simply copies of boxes, best_a, best_b and best_c. It it this function that will change them to produce
    //a true neighbour.
    get_neighbour(boxes_neighbour, n_boxes, current_a, current_b, current_c);

    place_boxes_sequence_triples(*current_a, *current_b, *current_c, &(*boxes_neighbour), n_boxes);
    

}

box* simulated_annealing_knapsack_3D(int* a, int* b, int* c, box* boxes_input, int n_boxes, int md, int secs){
    int* best_a = a;
    int* best_b = b;
    int* best_c = c;
    box* boxes = boxes_input;
    int temperature = 0.2;
    int beta = 0.2;
    int alpha = 0.002;
    int best_volume = 0;

    //mode = 0: perform the simulated annealing for a max number of iterations
    //mode = 1: perform the simulated annealing for a certain amount of time (seconds)
    int mode = md;

    int max_number_of_iterations = 1;
    int current_iteration = 0;

    int seconds = secs;


    box* boxes_neighbour = (box*) malloc(n_boxes*sizeof(box));
    int* current_a = (int*) malloc(n_boxes*sizeof(int));
    int* current_b = (int*) malloc(n_boxes*sizeof(int));
    int* current_c = (int*) malloc(n_boxes*sizeof(int));
    //now the code for a step of the simulated annealing
    switch(mode){
        case 0:
            for(int i = 0; i < max_number_of_iterations; i++){
                sa_make_a_step(&boxes, n_boxes, &temperature, alpha, beta, &current_a, &current_b, &current_c, 
                                &best_a, &best_b, &best_c, &best_volume, &boxes_neighbour);
            }
        break;
        case 1:
            float seconds_elapsed = 0;
            clock_t start = clock();
            while(seconds_elapsed < seconds){
                //to avoid too much overhead in the ckeck of the time, we perform a step of the
                //simulated annealing multiple times before checking the time.
                for(int i = 0; i < 100; i ++){
                    sa_make_a_step(&boxes, n_boxes, &temperature, alpha, beta, &current_a, &current_b, &current_c, 
                                &best_a, &best_b, &best_c, &best_volume, &boxes_neighbour);
                }
                clock_t end = clock();
                seconds = (float)(end - start) / CLOCKS_PER_SEC;
            }
            
            
        break;
        default:
        break;
    }


    return NULL;
}

int volume_occupied(box* boxes, int n_boxes, int cont_x, int cont_y, int cont_z, int mode){
    //mode = 0: consider only boxes fully contained
    //mode = 1: consider also boxes partially contained
    int volume_summed = 0;
    int x_in_bin;
    int y_in_bin;
    int z_in_bin;
    switch(mode){
        case 0:
            for(int i = 0; i < n_boxes; i++){
                x_in_bin = boxes[i].x0 + boxes[i].xlen <= cont_x ? boxes[i].xlen : 0;
                y_in_bin = boxes[i].y0 + boxes[i].ylen <= cont_y ? boxes[i].ylen : 0;
                z_in_bin = boxes[i].z0 + boxes[i].zlen <= cont_z ? boxes[i].zlen : 0;
            }
            volume_summed += x_in_bin*y_in_bin*z_in_bin;
        break;
        case 1:
            for(int i = 0; i < n_boxes; i++){
                x_in_bin = boxes[i].x0 + boxes[i].xlen <= cont_x ? boxes[i].xlen : max(cont_x - boxes[i].x0, 0);
                y_in_bin = boxes[i].y0 + boxes[i].ylen <= cont_y ? boxes[i].ylen : max(cont_y - boxes[i].y0, 0);
                z_in_bin = boxes[i].z0 + boxes[i].zlen <= cont_z ? boxes[i].zlen : max(cont_z - boxes[i].z0, 0);
            }
            volume_summed += x_in_bin*y_in_bin*z_in_bin;
        break;
        default:
        break;
    }
    return volume_summed;
}




