#include "structs.h"
#include "extreme_points.h"
#include "advanced_extreme_points.h"
#include "advanced_extreme_points_plus.h"
#include "investigate_points_with_boxes_around.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_ADVANCED_EXTREME_POINTS_PLUS 0 

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
    int n_upper_bound_final_volumes = 500 * (*n_volumes_to_exclude);   //nobody says this is always enough: I'm just guessing it is
    box* final_volumes_to_exclude = malloc(n_upper_bound_final_volumes * sizeof(box));  //array containing all the volumes to exclude

    int n_final_volumes_to_exclude = *n_volumes_to_exclude; //index of the first free position of the array containing the final volumes to exclude

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
            printf("b.xlen AAA 4 = %d\n", b.xlen);
            add_points_from_volume_to_exclude(b, &points_to_consider, &n_points_to_consider,
                    final_volumes_to_exclude, n_final_volumes_to_exclude, boxes_placed, n_boxes_placed);

        }

        printf("n_point_to_consider = %d\n", n_points_to_consider);

        //3) for each of those points, see if they induce new volumes to exclude
        for(int i = 0; i < n_points_to_consider; i++){

            //box volume_induced_1 = compute_volume_coverable_from_point(points_to_consider[i], boxes_placed, n_boxes_placed);
            //box volume_induced_2 = compute_volume_coverable_from_point(points_to_consider[i], volumes_to_exclude, n_volumes_to_exclude);

            box volume_induced = volume_induced_by_point_to_consider(points_to_consider[i], 
                                            boxes_placed, n_boxes_placed, final_volumes_to_exclude, n_final_volumes_to_exclude);
            if(volume_induced.x0 != -1){
                //4) add the new volume to exclude to the array of volumes to exclude
                printf("NEW volume induced: %d %d %d %d %d %d\n", volume_induced.xlen, volume_induced.ylen, volume_induced.zlen, 
                    volume_induced.x0, volume_induced.y0, volume_induced.z0);
                final_volumes_to_exclude[n_final_volumes_to_exclude] = volume_induced;
                n_final_volumes_to_exclude++;
            }
        }

        free(points_to_consider);

        printf("current_n_final_volumes_to_exclude = %d, n_final_volumes_to_exclude = %d\n", current_n_final_volumes_to_exclude, n_final_volumes_to_exclude);

    }while (current_n_final_volumes_to_exclude < n_final_volumes_to_exclude);
    
    free(*volumes_to_exclude);
    *volumes_to_exclude = final_volumes_to_exclude;
    *n_volumes_to_exclude = n_final_volumes_to_exclude;



}



//###########################################################################################################

void add_points_from_volume_to_exclude(box b, point** points_to_consider, int* n_points_to_consider,
        box* volumes_to_exclude, int n_volumes_to_exclude, box* boxes_placed, int n_boxes_placed){
    point* new_points = malloc(3*sizeof(point));

    printf("b.xlen AAA 5 = %d\n", b.xlen);

    new_points[0].x = b.x0 + b.xlen;
    new_points[0].y = b.y0;
    new_points[0].z = b.z0;
    new_points[0].spawnpoint = right_of_box;
    project_unavailable_point_down(&(new_points[0]), boxes_placed, n_boxes_placed, volumes_to_exclude, n_volumes_to_exclude);

    new_points[1].x = b.x0;
    new_points[1].y = b.y0 + b.ylen;
    new_points[1].z = b.z0;
    new_points[1].spawnpoint = top_of_box;

    new_points[2].x = b.x0;
    new_points[2].y = b.y0;
    new_points[2].z = b.z0 + b.zlen;
    new_points[2].spawnpoint = front_of_box;
    project_unavailable_point_down(&(new_points[2]), boxes_placed, n_boxes_placed, volumes_to_exclude, n_volumes_to_exclude);

    if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("new candidate point is: %d %d %d\n", new_points[0].x, new_points[0].y, new_points[0].z);
    if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("new candidate point is: %d %d %d\n", new_points[1].x, new_points[1].y, new_points[1].z);
    if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("new candidate point is: %d %d %d\n", new_points[2].x, new_points[2].y, new_points[2].z);

    //a point of this kind can only be added if:
    //1) it's not already present in the array
    //2) it does not go outside the container nor is inside another box (real or volume to exclude)
    for(int i = 0; i < 3; i++){
        point point_to_test = new_points[i];
        if(point_to_test.x < cont_x && point_to_test.y < cont_y && point_to_test.z < cont_z){
            if(is_point_not_redundant(point_to_test, *points_to_consider, *n_points_to_consider)){
                if(!is_point_inside_some_box(point_to_test, volumes_to_exclude, n_volumes_to_exclude, boxes_placed, n_boxes_placed)){
                    if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("new point to consider! is: %d %d %d\n", point_to_test.x, point_to_test.y, point_to_test.z);
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
        /*if(p.x == 17 && p.y == 0 && p.z == 3) printf("ABC box here is: %d %d %d %d %d %d\n", boxes1[i].xlen, boxes1[i].ylen, boxes1[i].zlen,
                boxes1[i].x0, boxes1[i].y0, boxes1[i].z0);*/
        if(is_point_inside_box(p, boxes1[i])){
            /*if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("sorry, point %d %d %d is inside %d %d %d %d %d %d\n", p.x, p.y, p.z, 
                boxes1[i].xlen, boxes1[i].ylen, boxes1[i].zlen, boxes1[i].x0, boxes1[i].y0, boxes1[i].z0);*/
            return 1;
        }
    }
    for(int i = 0; i < n_boxes2; i++){
        
        if(is_point_inside_box(p, boxes2[i])){
            /*if(1 || DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("sorry, point %d %d %d is inside %d %d %d %d %d %d\n", p.x, p.y, p.z, 
                boxes2[i].xlen, boxes2[i].ylen, boxes2[i].zlen, boxes2[i].x0, boxes2[i].y0, boxes2[i].z0);*/
            return 1;
        }
    }
    return 0;
}

//very simple function used to check if a point falls inside a box (of whatever kind)
int is_point_inside_box(point p, box b){
    /*if(p.x == 17 && p.y == 0 && p.z == 3) printf("DEF box here is: %d %d %d %d %d %d\n", b.xlen, b.ylen, b.zlen,
                b.x0, b.y0, b.z0);*/
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            /*if(p.x == 17 && p.y == 0 && p.z == 3) printf("YES");*/
            return 1;
    }
    /*if(p.x == 17 && p.y == 0 && p.z == 3) printf("NO\n");*/
    return 0;
}



//###########################################################################################################

//function that returns the volume induced by point p (if this volume exists)
box volume_induced_by_point_to_consider(point p, box* placed_boxes, int n_placed_boxes, 
                                                box* volumes_to_exclude, int n_volumes_to_exclude){
    box ret;
    ret.x0 = -1;

    box* boxes_to_exclude_around = boxes_around_point(p, volumes_to_exclude, n_volumes_to_exclude);
    //if there are less than 2 unavailable volumes in the positions left, behind and below wrt this point, don't go on anymore.
    
    int is_there_box_left = boxes_to_exclude_around[0].x0 == -1 ? 0 : 1;
    int is_there_box_behind = boxes_to_exclude_around[1].x0 == -1 ? 0 : 1;
    int is_there_box_below = boxes_to_exclude_around[2].x0 == -1 ? 0 : 1;

    if(is_there_box_left + is_there_box_behind + is_there_box_below < 2){
        printf("there are no >=2 boxes around");
        free(boxes_to_exclude_around);
        return ret;
    }

    printf("ma qui ci arrivo\n");

    int left_behind = (is_there_box_left && is_there_box_behind);
    int left_below = (is_there_box_left && is_there_box_below);
    int behind_below = (is_there_box_behind && is_there_box_below);
    int left_behind_below = (is_there_box_left && is_there_box_behind && is_there_box_below);

    //if instead there are, we have to check also for the normal boxes around the point
    box* boxes_around = boxes_around_point(p, placed_boxes, n_placed_boxes);

    //if at this point we don't have a box (of whatever kind) on the left, behind and belowe wrt p, don't go on anymore
    if(!left_behind_below){
        if((left_behind && (boxes_around[2].x0 == -1 && p.y > 0)) || 
            (left_below && (boxes_around[1].x0 == -1 && p.z > 0)) || 
            (behind_below && (boxes_around[0].x0 == -1 && p.x > 0))){
                printf("qui?");
                free(boxes_to_exclude_around);
                free(boxes_around);
                return ret;
        }
    }

    ret.x0 = p.x;
    ret.y0 = p.y;
    ret.z0 = p.z;
    
    box box_left_u = boxes_to_exclude_around[0];
    box box_behind_u = boxes_to_exclude_around[1];
    box box_below_u = boxes_to_exclude_around[2];
    box box_right_u = boxes_to_exclude_around[3];
    box box_front_u = boxes_to_exclude_around[4];
    box box_above_u = boxes_to_exclude_around[5];

    box box_left = boxes_around[0];
    box box_behind = boxes_around[1];
    box box_below = boxes_around[2];
    box box_right = boxes_around[3];
    box box_front = boxes_around[4];
    box box_above = boxes_around[5];

    int x1 = min(box_behind.x0 + box_behind.xlen - p.x, box_behind_u.x0 + box_behind_u.xlen - p.x);
    int x2 = min(box_front.x0 + box_front.xlen - p.x, box_front_u.x0 + box_front_u.xlen - p.x);
    int x3 = min(box_below.x0 + box_below.xlen - p.x, box_below_u.x0 + box_below_u.xlen - p.x);
    int x4 = min(box_above.x0 + box_above.xlen - p.x, box_above_u.x0 + box_above_u.xlen - p.x);

    int y1 = min(box_behind.y0 + box_behind.ylen - p.y, box_behind_u.y0 + box_behind_u.ylen - p.y);
    int y2 = min(box_front.y0 + box_front.ylen - p.y, box_front_u.y0 + box_front_u.ylen - p.y);
    int y3 = min(box_left.y0 + box_left.ylen - p.y, box_left_u.y0 + box_left_u.ylen - p.y);
    int y4 = min(box_right.y0 + box_right.ylen - p.y, box_right_u.y0 + box_right_u.ylen - p.y);

    int z1 = min(box_left.z0 + box_left.zlen - p.z, box_left_u.z0 + box_left_u.zlen - p.z);
    int z2 = min(box_right.z0 + box_right.zlen - p.z, box_right_u.z0 + box_right_u.zlen - p.z);
    int z3 = min(box_below.z0 + box_below.zlen - p.z, box_below_u.z0 + box_below_u.zlen - p.z);
    int z4 = min(box_above.z0 + box_above.zlen - p.z, box_above_u.z0 + box_above_u.zlen - p.z);

    //int x = min(x1, min(x2, min(x3, x4)));
    //int y = min(y1, min(y2, min(y3, y4)));
    //int z = min(z1, min(z2, min(z3, z4)));

    int x = min(x1, x3);
    int y = min(y1, y3);
    int z = min(z1, z3);
    printf("x, y e z di questo nuovo volume rosso: %d %d %d\n", x, y, z);

    ret.xlen = x;
    ret.ylen = y;
    ret.zlen = z;

    free(boxes_to_exclude_around);
    free(boxes_around);

    if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){
        printf("ma perché scusa?\n");
        ret.x0 = -1;
        return ret;
    }

    printf("tutt'appost\n");

    return ret;












    /*
    if(1 || 0){
        printf("in volume_induced_by_point_to_consider: \n p = %d %d %d\n", p.x, p.y, p.z);
        printf("%d\n", n_placed_boxes);
        for(int i = 0; i < n_placed_boxes; i++){
            printf("%d %d %d %d %d %d\n", placed_boxes[i].xlen, placed_boxes[i].ylen, placed_boxes[i].zlen,
                    placed_boxes[i].x0, placed_boxes[i].y0, placed_boxes[i].z0);
        }
        printf("%d\n", n_volumes_to_exclude);
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
    if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("walls adjacent: %d\n", boxes_adjacent_found);

    for(int i = 0; i < n_placed_boxes; i++){
        //let's see if there are some placed boxes adjacent (left, behind or below) the current point
        box b_to_check = placed_boxes[i];
        if(can_there_be_a_box_left){
            if(is_box_left_touching___point(p, b_to_check)){
                box_left = b_to_check;
                boxes_adjacent_found++;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found normal box left\n");
            }
        }
        if(can_there_be_a_box_behind){
            if(is_box_behind_touching___point(p, b_to_check)){
                box_behind = b_to_check;
                boxes_adjacent_found++;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found normal box behind\n");
            }
        }
        if(can_there_be_a_box_below){
            if(is_box_below_touching___point(p, b_to_check)){
                box_below = b_to_check;
                boxes_adjacent_found++;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found normal box below\n");
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
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("b_to_check (red): %d %d %d %d %d %d\n", b_to_check.xlen, b_to_check.ylen, b_to_check.zlen, b_to_check.x0, b_to_check.y0, b_to_check.z0);
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("p (red new) = %d %d %d\n", p.x, p.y, p.z);
        if(can_there_be_a_box_left){
            if(is_box_left_touching___point(p, b_to_check)){
                box_left = b_to_check;
                boxes_adjacent_found++;
                is_box_left_unavailable_volume = 1;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found red box left\n");
            }
        }
        if(can_there_be_a_box_behind){
            if(is_box_behind_touching___point(p, b_to_check)){
                box_behind = b_to_check;
                boxes_adjacent_found++;
                is_box_behind_unavailable_volume = 1;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found red box behind\n");
            }
        }
        if(can_there_be_a_box_below){
            if(is_box_below_touching___point(p, b_to_check)){
                box_below = b_to_check;
                boxes_adjacent_found++;
                is_box_below_unavailable_volume = 1;
                if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("AAA found red box below\n");
            }
        }
    }

    //if I found, at this point, three boxes adjacent, i can induce a volume. Otherwise, I can't
    if(boxes_adjacent_found < 3){
        return ret;
    }

    ret.x0 = p.x;
    ret.y0 = p.y;
    ret.z0 = p.z;

    //intermediate step: check the space "in line of air" available for this point
    int* lengths1 = get_lengths_air_of_unavailable_point(p, placed_boxes, n_placed_boxes);
    int* lengths2 = get_lengths_air_of_unavailable_point(p, volumes_to_exclude, n_volumes_to_exclude);
    ret.xlen = min(lengths1[0], lengths2[0]);
    ret.ylen = min(lengths1[1], lengths2[1]);
    ret.zlen = min(lengths1[2], lengths2[2]);
    free(lengths1);
    free(lengths2);

    //at this point, we have at least two unvailable boxes volume adjacent. Let's consider four cases:
    //1) all three adjacent boxes are unavailable volumes.
    //2) the one on the left and the one behind are unavailable volumes, the last one is either a placed box or a wall
    //3 and 4) same as before but with the other combinations of unavailable volumes and placed boxes/walls
    if(is_box_left_unavailable_volume && is_box_behind_unavailable_volume && is_box_below_unavailable_volume){
        ret.xlen = min(ret.xlen, min(box_behind.x0 + box_behind.xlen - ret.x0, box_below.x0 + box_below.xlen - ret.x0));
        ret.ylen = min(ret.ylen, min(box_behind.y0 + box_behind.ylen - ret.y0, box_left.y0 + box_left.ylen - ret.y0));
        ret.zlen = min(ret.zlen, min(box_left.z0 + box_left.zlen - ret.z0, box_below.z0 + box_below.zlen - ret.z0));
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("(1) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);

        if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){    //should not be necessary, but just to make sure
            ret.x0 = -1;
        }
        return ret;
    }
    

    if(is_box_left_unavailable_volume && is_box_behind_unavailable_volume){
        ret.xlen = min(ret.xlen, box_behind.x0 + box_behind.xlen - ret.x0);
        ret.zlen = min(ret.zlen, box_left.z0 + box_left.zlen - ret.z0);
        if(ret.y0 > 0){
            ret.xlen = min(ret.xlen, box_below.x0 + box_below.xlen - ret.x0);
            ret.zlen = min(ret.zlen, box_below.z0 + box_below.zlen - ret.z0);
        }
        
        ret.ylen = min(ret.ylen, min(box_behind.y0 + box_behind.ylen - ret.y0, box_left.y0 + box_left.ylen - ret.y0));
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("(2) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);

        if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){
            ret.x0 = -1;
        }
        return ret;
    }
    

    if(is_box_behind_unavailable_volume && is_box_below_unavailable_volume){
        ret.ylen = min(ret.ylen, box_behind.y0 + box_behind.ylen - ret.y0);
        ret.zlen = min(ret.zlen, box_below.z0 + box_below.zlen - ret.z0);
        if(ret.x0 > 0){
            ret.ylen = min(ret.ylen, box_left.y0 + box_left.ylen - ret.y0);
            ret.zlen = min(ret.zlen, box_left.z0 + box_left.zlen - ret.z0);
        }

        ret.xlen = min(ret.xlen, min(box_behind.x0 + box_behind.xlen - ret.x0, box_below.x0 + box_below.xlen - ret.x0));
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("(3) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);

        if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){
            ret.x0 = -1;
        }
        return ret;
    }
    

    if(is_box_left_unavailable_volume && is_box_below_unavailable_volume){
        ret.xlen = min(ret.xlen, box_below.x0 + box_below.xlen - ret.x0);
        ret.ylen = min(ret.ylen, box_left.y0 + box_left.ylen - ret.y0);
        if(ret.z0 > 0){
            ret.xlen = min(ret.xlen, box_behind.x0 + box_behind.xlen - ret.x0);
            ret.ylen = min(ret.ylen, box_behind.y0 + box_behind.ylen - ret.y0);
        }

        ret.zlen = min(ret.zlen, min(box_left.z0 + box_left.zlen - ret.z0, box_below.z0 + box_below.zlen - ret.z0));
        if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("(4) ret.xlen = %d, ret.ylen = %d, ret.zlen = %d\n", ret.xlen, ret.ylen, ret.zlen);

        if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){
            ret.x0 = -1;
        }
        return ret;
    }

    if(DEBUG_ADVANCED_EXTREME_POINTS_PLUS) printf("We have a BIG problem\n");*/

}





//###########################

//function that returns the free space "in line of air" on the right, above and front of p (point that induces an unavailable volume)
int* get_lengths_air_of_unavailable_point(point p, box* boxes, int n_boxes){
    int min_distance_x = cont_x;
    int min_distance_y = cont_y;
    int min_distance_z = cont_z;
    for(int i = 0; i < n_boxes; i++){
        box b = boxes[i];
        
        if(p.x <= b.x0 && 
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
            b.z0 <= p.z && p.z < b.z0 + b.zlen){
                //if this box is on the right wrt point p...
                //I want the box immediately on the right wrt point p
                min_distance_z = min(min_distance_z, b.z0 + b.zlen - p.z);
                min_distance_y = min(min_distance_y, b.y0 + b.ylen - p.y);
        }



        if(p.y <= b.y0 && 
            b.x0 <= p.x && p.x < b.x0 + b.xlen &&
            b.z0 <= p.z && p.z < b.z0 + b.zlen){
                //if this box is above wrt point p...  
                //I want the box immediately above wrt point p
                min_distance_z = min(min_distance_z, b.z0 + b.zlen - p.z);
                min_distance_x = min(min_distance_x, b.x0 + b.xlen - p.x);
        }



        if(p.z <= b.z0 && 
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
            b.x0 <= p.x && p.x < b.x0 + b.xlen){
                //if this box is in front wrt point p...
                //I want the box immediately in front wrt point p
                min_distance_x = min(min_distance_x, b.x0 + b.xlen - p.x);
                min_distance_y = min(min_distance_y, b.y0 + b.ylen - p.y);
        }
    }

    int* ret = malloc(3*sizeof(int));
    ret[0] = min_distance_x;
    ret[1] = min_distance_y;
    ret[2] = min_distance_z;
    return ret;
}






