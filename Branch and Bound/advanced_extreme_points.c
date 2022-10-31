#include "advanced_extreme_points.h"
#include "structs.h"
#include "extreme_points.h"
#include <stdlib.h>
#include <stdio.h>




//function that, given a point, if it is a point spawned on the right or in front of box,
//projects it on the first available surface below.
void project_point_down(point* p, box* placed_boxes, int n){
    if(p->spawnpoint == top_of_box) return;
    int point_height = 0;
    for(int i = 0; i < n; i++){
        box b = placed_boxes[i];
        if(b.x0 <= p->x && p->x < b.x0 + b.xlen &&
            b.z0 <= p->z && p->z < b.z0 + b.zlen &&
            b.y0 + b.ylen <= p->y){
                //the upper face of the box is below the point
                point_height = max(point_height, b.y0 + b.ylen);
        }
    }
    p->y = point_height;
    update_point_dimensions(p, placed_boxes, n);
    
}





int comp_left(const void * a, const void * b){
    box a1 = *(const box*) a;
    box a2 = *(const box*) b;
    return a1.z0 - a1.z0; 
}

//despite the name, it returns the (minimum, certain) volume that can't be occupied in that point
int find_box_touching_point_left(point p, box* boxes_placed, int n_boxes_placed){
    box minimum_box_on_the_left_but_above;
    minimum_box_on_the_left_but_above.y0 = -1;
    
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];
        if(b.x0 + b.xlen == p.x && b.z0 == p.z && b.y0 == p.y){
            return b.ylen * b.zlen * p.width;
        }
        if(b.x0 + b.xlen == p.x && b.z0 == p.z && b.y0 > p.y){
            if(minimum_box_on_the_left_but_above.y0 == -1){
                minimum_box_on_the_left_but_above = b;
            }else{
                if(b.y0 < minimum_box_on_the_left_but_above.y0){
                    minimum_box_on_the_left_but_above = b;
                }
            }
        }
    }

    //no point might be found, if the point is the result of a projection. In that case,
    //let's do something simple: among all the boxes that are touching on the left with respect
    //to the point, let's take the one with minimum height.
    return (minimum_box_on_the_left_but_above.y0 + minimum_box_on_the_left_but_above.ylen - p.y)*
            minimum_box_on_the_left_but_above.zlen * p.width;

    /*

    //first: find all the boxes that are exactly confining, on the left, with this point
    //second: consider only the ones in front of the point
    //(actually, we are doing the two things at the same time)
    int count_1 = 0;
    int* indexes = malloc(n_boxes_placed*sizeof(int));
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];
        if(b.x0 + b.xlen == p.x && b.z0 >= p.z){
            indexes[count_1] = i;
            count_1++;
        }
    }

    //third: among these, consider only:
    //1) the box that is exactly on the left with respect to "p" (of course)
    //2) all the following boxes that touch the face of the preceeding one (along the z axis)
    //2.1) but only if their height is greater than the one of the first box
    box* boxes_important = malloc(count_1*sizeof(box));
    for(int i = 0; i < count_1; i++){
        boxes_important[i] = boxes_placed[indexes[i]];
    }
    free(indexes);
    qsort(boxes_important, count_1, sizeof(box), comp_left);

    assert(boxes_important[0].z0, p.z);
    int z_current = boxes_important[0].z0 + boxes_important[0].zlen;
    int count_2 = 1;
    for(int i = 1; i < count_1; i++){
        if(z_current == boxes_important[i].z0){
            z_current += boxes_important[i].zlen;
            count_2++;
        }else{
            break;
        }
    }

    //we found the boxes we were looking for
    //I do a little trick: the first entry of the array is a false box, that will contain,
    //in x0, the length of this array
    box* ret = malloc((count_2+1)*sizeof(box));
    box fake;
    fake.x0 = count_2;
    for(int i = 1; i < count_2+1; i++){
        ret[i] = boxes_important[i-1];
    }
    free(boxes_important);
    return ret;
    */

}

int find_box_touching_point_below(point p, box* boxes_placed, int n_boxes_placed){
    //here it's even easier: "p" is for sure a point above a box, so let's find that box!
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];
        if(b.y0 + b.ylen == p.y && b.x0 == p.x && b.z0 == p.z){
            return b.xlen * b.zlen * p.height;
        }
    }
}

int find_box_touching_point_behind(point p, box* boxes_placed, int n_boxes_placed){
    box minimum_box_behind_but_above;
    minimum_box_behind_but_above.y0 = -1;
    
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];
        if(b.z0 + b.zlen == p.z && b.x0 == p.x && b.y0 == p.y){
            return b.ylen * b.xlen * p.depth;
        }
        if(b.z0 + b.zlen == p.z && b.x0 == p.x && b.y0 > p.y){
            if(minimum_box_behind_but_above.y0 == -1){
                minimum_box_behind_but_above = b;
            }else{
                if(b.y0 < minimum_box_behind_but_above.y0){
                    minimum_box_behind_but_above = b;
                }
            }
        }
    }

    //no point might be found, if the point is the result of a projection. In that case,
    //let's do something simple: among all the boxes that are touching on the back with respect
    //to the point, let's take the one with minimum height.
    return (minimum_box_behind_but_above.y0 + minimum_box_behind_but_above.ylen - p.y)*
            minimum_box_behind_but_above.xlen * p.depth;

}













//function that, given the current (free) capacity of the container and the points
//that have to be excluded, computes the new capacity. The points to be excluded
//can have their dimensions of two kind:
//1) they touch one of the container's walls.
//2) they touch an already placed box.
//in the first case, the actual length that contributes to the volume to be removed is
//smaller than the total length of the point.
int capacity_minus_unavailable_points_volume(int capacity, box* boxes_placed, int n_boxes_placed,
                    point* points_to_exclude, int n_points_to_exclude){
    for(int i = 0; i < n_points_to_exclude; i++){
        point p = points_to_exclude[i];
        printf("p to remove from capacity = %d %d %d - %d\n", p.x, p.y, p.z, p.spawnpoint);
        switch(p.spawnpoint){
            case right_of_box:
                capacity -= find_box_touching_point_left(p, boxes_placed, n_boxes_placed);
            break;
            case top_of_box:
                capacity -= find_box_touching_point_below(p, boxes_placed, n_boxes_placed);
            break;
            case front_of_box:
                capacity -= find_box_touching_point_behind(p, boxes_placed, n_boxes_placed);
            break;
            default:
            break;
        }
    }
    return capacity;
}


//function to check if a point we'd like to add to a list of points is already present
int is_point_not_redundant(point new_p, point* points, int n_points){
    for(int i = 0; i < n_points; i++){
        if(new_p.x == points[i].x && new_p.y == points[i].y && new_p.z == points[i].z){
            return 0;
        }
    }
    return 1;
}

