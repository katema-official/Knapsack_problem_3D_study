#include "structs.h"
#include "extreme_points.h"
#include "advanced_extreme_points.h"
#include "advanced_extreme_points_plus.h"
#include <stdlib.h>
#include <stdio.h>

extern int cont_x;
extern int cont_y;
extern int cont_z;

//function that, given the volumes to exclude found, and the boxes placed,
//searches for even more volumes to exclude.
//What it does is:
//1) computes the extreme_points of the given volumes to exclude.
//2) cleans this list of points from those that can't be used/are redundant
//3) tries to find new unavailable volumes from the given points. If it does:
//4) updates the given array of volumes_to_exclude passed as argument as a reference,
//with the new volumes found.
//5) now, the given volumes to exclude are set to those volumes just found. Go back to step 1

//this repeats untill step 3 is evaluated to false (no more volumes to exclude can be generated)

void deduce_even_more_unavailable_points_volume(box** volumes_to_exclude, int* n_volumes_to_exclude,
                box* boxes_placed, int n_boxes_placed){
    
    //the first thing to ask is: how many volumes will this function generate?
    //We cannot give an exact answer, as this will depend on the specific instance presented.
    //For now, we simply give a "guessed" value that we assume will be larger or equal than
    //the maximum number of volumes this function will generate
    int n_upper_bound_final_volumes = 50 * (*n_volumes_to_exclude);   //nobody says this is always enough: I'm just guessing it is
    box* final_volumes_to_exclude = malloc(n_upper_bound_final_volumes * sizeof(box));  //array containing all the volumes to exclude

    int n_final_volumes_to_exclude = *n_volumes_to_exclude; //index of the first free position of the array containing the final volumes to exclude

    printf("E VA BENE, ancora prima. Volumes to exclude:\n");
    for(int i = 0; i < *n_volumes_to_exclude; i++){
        printf("%d %d %d %d %d %d\n", (*volumes_to_exclude)[i].xlen, (*volumes_to_exclude)[i].ylen, (*volumes_to_exclude)[i].zlen,
                (*volumes_to_exclude)[i].x0, (*volumes_to_exclude)[i].y0, (*volumes_to_exclude)[i].z0);
    }

    copy_boxes(&final_volumes_to_exclude, *volumes_to_exclude, *n_volumes_to_exclude);

    int n_points_to_consider;
    int current_n_final_volumes_to_exclude;

    do{
        current_n_final_volumes_to_exclude = n_final_volumes_to_exclude;
        point* points_to_consider = malloc(3*current_n_final_volumes_to_exclude*sizeof(point));
        n_points_to_consider = 0;
        //1) compute the extreme_points of the given volumes to exclude
        //2) in add_points_from_volume_to_exclude we automatically remove useless points
        for(int i = 0; i < n_final_volumes_to_exclude; i++){
            box b = final_volumes_to_exclude[i];    //the red box (volumes to exclude)
            add_points_from_volume_to_exclude(b, &points_to_consider, &n_points_to_consider,
                    final_volumes_to_exclude, n_final_volumes_to_exclude, boxes_placed, n_boxes_placed);

        }

        //3) for each of those points, sees if they induce new volumes to exclude
        for(int i = 0; i < n_points_to_consider; i++){
            if(points_to_consider[i].x == 7 && points_to_consider[i].y == 0 && points_to_consider[i].z == 9){
                printf("allora, ci riprovo (prima): volumes to exclude:\n");
                for(int i = 0; i < n_final_volumes_to_exclude; i++){
                    printf("%d %d %d %d %d %d\n", final_volumes_to_exclude[i].xlen, final_volumes_to_exclude[i].ylen, final_volumes_to_exclude[i].zlen,
                            final_volumes_to_exclude[i].x0, final_volumes_to_exclude[i].y0, final_volumes_to_exclude[i].z0);
                }
            }
            box volume_induced = volume_induced_by_point_to_consider(points_to_consider[i], 
                                            boxes_placed, n_boxes_placed, final_volumes_to_exclude, n_final_volumes_to_exclude);
            if(volume_induced.x0 != -1){
                //4) add the new volume to exclude to the array of volumes to exclude
                final_volumes_to_exclude[n_final_volumes_to_exclude] = volume_induced;
                n_final_volumes_to_exclude++;
            }
        }

        free(points_to_consider);

    }while (current_n_final_volumes_to_exclude < n_final_volumes_to_exclude);
    
    free(*volumes_to_exclude);
    *volumes_to_exclude = final_volumes_to_exclude;
    *n_volumes_to_exclude = n_final_volumes_to_exclude;



}



//###########################################################################################################

void add_points_from_volume_to_exclude(box b, point** points_to_consider, int* n_points_to_consider,
        box* volumes_to_exclude, int n_volumes_to_exclude, box* boxes_placed, int n_boxes_placed){
    point* new_points = malloc(3*sizeof(point));

    new_points[0].x = b.x0 + b.xlen;
    new_points[0].y = b.y0;
    new_points[0].z = b.z0;

    new_points[1].x = b.x0;
    new_points[1].y = b.y0 + b.ylen;
    new_points[1].z = b.z0;

    new_points[2].x = b.x0;
    new_points[2].y = b.y0;
    new_points[2].z = b.z0 + b.zlen;

    printf("new candidate point is: %d %d %d\n", new_points[0].x, new_points[0].y, new_points[0].z);
    printf("new candidate point is: %d %d %d\n", new_points[1].x, new_points[1].y, new_points[1].z);
    printf("new candidate point is: %d %d %d\n", new_points[2].x, new_points[2].y, new_points[2].z);

    //a point of this kind can only be added if:
    //1) it's not already present in the array
    //2) it does not go outside the container nor is inside another box (real or volume to exclude)
    for(int i = 0; i < 3; i++){
        point point_to_test = new_points[i];
        if(point_to_test.x < cont_x && point_to_test.y < cont_y && point_to_test.z < cont_z){
            if(is_point_not_redundant(point_to_test, *points_to_consider, *n_points_to_consider)){
                if(!is_point_inside_some_box(point_to_test, volumes_to_exclude, n_volumes_to_exclude, boxes_placed, n_boxes_placed)){
                    printf("new point to consider! is: %d %d %d\n", point_to_test.x, point_to_test.y, point_to_test.z);
                    copy_point(&( (*points_to_consider)[(*n_points_to_consider)] ), point_to_test);
                    (*n_points_to_consider) += 1;
                }
            }
        }
    }

    free(new_points);

}

//ised to check if a new point falls inside a placed box or volume to exclude already computed
int is_point_inside_some_box(point p, box* boxes1, int n_boxes1, box* boxes2, int n_boxes2){
    for(int i = 0; i < n_boxes1; i++){
        if(is_point_inside_box(p, boxes1[i])){
            printf("sorry, point %d %d %d is inside %d %d %d %d %d %d\n", p.x, p.y, p.z, 
                boxes1[i].xlen, boxes1[i].ylen, boxes1[i].zlen, boxes1[i].x0, boxes1[i].y0, boxes1[i].z0);
            return 1;
        }
    }
    for(int i = 0; i < n_boxes2; i++){
        if(is_point_inside_box(p, boxes2[i])){
            printf("sorry, point %d %d %d is inside %d %d %d %d %d %d\n", p.x, p.y, p.z, 
                boxes2[i].xlen, boxes2[i].ylen, boxes2[i].zlen, boxes2[i].x0, boxes2[i].y0, boxes2[i].z0);
            return 1;
        }
    }
    return 0;
}

//very simple function used to check if a point falls inside a box (of whatever kind)
int is_point_inside_box(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            return 1;
    }
    return 0;
}



//###########################################################################################################

//function that returns the volume induced by point p (if this volume exists)
box volume_induced_by_point_to_consider(point p, box* placed_boxes, int n_placed_boxes, 
                                                box* volumes_to_exclude, int n_volumes_to_exclude){
    box ret;
    ret.x0 = -1;

    printf("################################\n");
    if(p.x == 7 && p.y == 0 && p.z == 9){
        printf("point here: %d %d %d\n", p.x, p.y, p.z);
        printf("the red boxes are: \n");
        for(int i = 0; i < n_volumes_to_exclude; i++){
            printf("%d %d %d %d %d %d\n", volumes_to_exclude[i].xlen, volumes_to_exclude[i].ylen, volumes_to_exclude[i].zlen,
                    volumes_to_exclude[i].x0, volumes_to_exclude[i].y0, volumes_to_exclude[i].z0);

        }
    
    }

    //a point of this kind induces a volume to exclude iif:
    //-it is confining with three faces of volumes to exclude
    //-it is confining with two faces of volumes to exclude AND a face of placed boxes/container wall

    box box_left;
    box box_behind;
    box box_below;
    box_left.x0 = -1;
    box_behind.x0 = -1;
    box_below.x0 = -1;

    int can_there_be_a_box_left = (p.x == 0) ? 0 : 1;    //can there be a box on the left of this point?
    int can_there_be_a_box_behind = (p.z == 0) ? 0 : 1;    //can there be a box behind this point?
    int can_there_be_a_box_below = (p.y == 0) ? 0 : 1;    //can there be a box below this point?

    int is_box_left_unavailable_volume = 0;
    int is_box_behind_unavailable_volume = 0;
    int is_box_below_unavailable_volume = 0;

    int boxes_adjacent_found = 0;
    boxes_adjacent_found += (1 - can_there_be_a_box_left) + (1 - can_there_be_a_box_behind) + (1 - can_there_be_a_box_below);
    printf("walls adjacent: %d\n", boxes_adjacent_found);

    for(int i = 0; i < n_placed_boxes; i++){
        //let's see if there are some placed boxes adjacent (left, behind or below) the current point
        box b_to_check = placed_boxes[i];
        if(can_there_be_a_box_left){
            if(is_box_left_touching___point(p, b_to_check)){
                box_left = b_to_check;
                boxes_adjacent_found++;
                printf("AAA found normal box left\n");
            }
        }
        if(can_there_be_a_box_behind){
            if(is_box_behind_touching___point(p, b_to_check)){
                box_behind = b_to_check;
                boxes_adjacent_found++;
                printf("AAA found normal box behind\n");
            }
        }
        if(can_there_be_a_box_below){
            if(is_box_below_touching___point(p, b_to_check)){
                box_below = b_to_check;
                boxes_adjacent_found++;
                printf("AAA found normal box below\n");
            }
        }
    }

    //if, in this way, I found two or more placed boxes/walls adjacent to my point, I'm sure this point cannot induce any volume    
    if(boxes_adjacent_found >= 2){
        return ret;
    }
    

    for(int i = 0; i < n_volumes_to_exclude; i++){
        //let's see if there are some placed boxes adjacent (left, behind or below) the current point
        box b_to_check = volumes_to_exclude[i];
        printf("b_to_check (red): %d %d %d %d %d %d\n", b_to_check.xlen, b_to_check.ylen, b_to_check.zlen, b_to_check.x0, b_to_check.y0, b_to_check.z0);
        printf("p (red new) = %d %d %d\n", p.x, p.y, p.z);
        if(can_there_be_a_box_left){
            if(is_box_left_touching___point(p, b_to_check)){
                box_left = b_to_check;
                boxes_adjacent_found++;
                is_box_left_unavailable_volume = 1;
                printf("AAA found red box left\n");
            }
        }
        if(can_there_be_a_box_behind){
            if(is_box_behind_touching___point(p, b_to_check)){
                box_behind = b_to_check;
                boxes_adjacent_found++;
                is_box_behind_unavailable_volume = 1;
                printf("AAA found red box behind\n");
            }
        }
        if(can_there_be_a_box_below){
            if(is_box_below_touching___point(p, b_to_check)){
                box_below = b_to_check;
                boxes_adjacent_found++;
                is_box_below_unavailable_volume = 1;
                printf("AAA found red box below\n");
            }
        }
    }

    //if I found, at this point, three boxes adjacent, i can induce a volume. Otherwise, I can't
    if(boxes_adjacent_found < 3){
        return ret;
    }

    /*printf("boxes found:\n box_left = %d %d %d %d %d %d\n box_behind = %d %d %d %d %d %d\n box_below = %d %d %d %d %d %d\n",
        box_left.xlen, box_left.ylen, box_left.zlen, box_left.x0, box_left.y0, box_left.z0, 
        box_behind.xlen, box_behind.ylen, box_behind.zlen, box_behind.x0, box_behind.y0, box_behind.z0,
        box_below.xlen, box_below.ylen, box_below.zlen, box_below.x0, box_below.y0, box_below.z0);*/


    ret.x0 = p.x;
    ret.y0 = p.y;
    ret.z0 = p.z;

    //at this point, we have at least two unvailable boxes volume adjacent. Let's consider four cases:
    //1) all three adjacent boxes are unavailable volumes.
    //2) the one on the left and the one behind are unavailable volumes, the last one is either a placed box or a wall
    //3 and 4) same as before but with the other combinations of unavailable volumes and placed boxes/walls
    if(is_box_left_unavailable_volume && is_box_behind_unavailable_volume && is_box_below_unavailable_volume){
        ret.xlen = min(box_behind.x0 + box_behind.xlen - ret.x0, box_below.x0 + box_below.xlen - ret.x0);
        ret.ylen = min(box_behind.y0 + box_behind.ylen - ret.y0, box_left.y0 + box_left.ylen - ret.y0);
        ret.zlen = min(box_left.z0 + box_left.zlen - ret.z0, box_below.z0 + box_below.zlen - ret.z0);
        printf("(1) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);
        return ret;
    }
    

    if(is_box_left_unavailable_volume && is_box_behind_unavailable_volume){
        ret.xlen = box_behind.x0 + box_behind.xlen - ret.x0;
        ret.zlen = box_left.z0 + box_left.zlen - ret.z0;
        ret.ylen = min(box_behind.y0 + box_behind.ylen - ret.y0, box_left.y0 + box_left.ylen - ret.y0);
        printf("(2) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);
        return ret;
    }
    

    if(is_box_behind_unavailable_volume && is_box_below_unavailable_volume){
        ret.ylen = box_behind.y0 + box_behind.ylen - ret.y0;
        ret.zlen = box_below.z0 + box_below.zlen - ret.z0;
        ret.xlen = min(box_behind.x0 + box_behind.xlen - ret.x0, box_below.x0 + box_below.xlen - ret.x0);
        printf("(3) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);
        return ret;
    }
    

    if(is_box_left_unavailable_volume && is_box_below_unavailable_volume){
        ret.xlen = box_below.x0 + box_below.xlen - ret.x0;
        ret.ylen = box_left.y0 + box_left.ylen - ret.y0;
        ret.zlen = min(box_left.z0 + box_left.zlen - ret.z0, box_below.z0 + box_below.zlen - ret.z0);
        printf("(4) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);
        return ret;
    }

    printf("We have a BIG problem\n");

}


//function to check if a box is EXACTLY on the left wrt a point
int is_box_left_touching___point(point p, box b){
    if(b.x0 + b.xlen == p.x && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            return 1;
    }
    return 0;
}

int is_box_behind_touching___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 + b.zlen == p.z){
            return 1;
    }
    return 0;
}

int is_box_below_touching___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 + b.ylen == p.y && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            return 1;
    }
    return 0;
}
