#include "extreme_points.h"
#include <stdlib.h>
#include "boxes.h"
#include <stdio.h>

extern int cont_x;
extern int cont_y;
extern int cont_z;

int is_box_right_of_point(box b, point p){
    return b.x0 >= p.x;
}
int is_box_above_of_point(box b, point p){
    return b.y0 >= p.y;
}
int is_box_front_of_point(box b, point p){
    return b.z0 >= p.z;
}

//assuming that a reference to a point "p" is given, and that it has already defined
//p.x, p.y and p.z, this function adds simply the initial width, height and depth to that point
void create_point_dimensions(point* p){
    (*p).width = cont_x - (*p).x;
    (*p).height = cont_y - (*p).y;
    (*p).depth = cont_z - (*p).z;

}

//function that, given the boxes already placed (boxes) and the extreme point p,
//computes how much free space is there to the right, above and front of p
void update_point_dimensions(point* p, box* boxes, int n_boxes){
    for(int i = 0; i < n_boxes; i++){
        box b = boxes[i];
        
        if(is_box_right_of_point(b, *p) && b.y0 <= (*p).y && (*p).y < b.y0 + b.ylen && 
                                            b.z0 <= (*p).z && (*p).z < b.z0 + b.zlen){
            p->width = min(p->width, b.x0 - p->x);
        }

        if(is_box_above_of_point(b, *p) && b.x0 <= (*p).x && (*p).x < b.x0 + b.xlen && 
                                            b.z0 <= (*p).z && (*p).z < b.z0 + b.zlen){
            p->height = min(p->height, b.y0 - p->y);
        }

        if(is_box_front_of_point(b, *p) && b.y0 <= (*p).y && (*p).y < b.y0 + b.ylen && 
                                            b.x0 <= (*p).x && (*p).x < b.x0 + b.xlen){
            p->depth = min(p->depth, b.z0 - p->z);
        }
    }
}

void copy_point(point* dst, point src){
    (*dst).x = src.x;
    (*dst).y = src.y;
    (*dst).z = src.z;
    (*dst).width = src.width;
    (*dst).height = src.height;
    (*dst).depth = src.depth;
    (*dst).spawnpoint = src.spawnpoint;
}

point* get_copy_points_except_one(point* src, int n, int i, int n_new_points){
    point* dst = malloc((n-1+n_new_points)*sizeof(point)); //remove a point, add (up to) three more. 
    //This happens when a box is placed
    for(int j = 0; j < n; j++){
        if(j < i){
            copy_point(&dst[j], src[j]);
        }
        if(j > i){
            copy_point(&dst[j-1], src[j]);
        }
    }
    return dst;
}

//function that, given the extreme points and the boxes still to place, tells if some
//points are such that NO box can be placed there (to be as general as possible, we
//will return the indexes, of the extreme_points array passed, of those points that
//CANNOT CONTAIN ANY BOX, of those to be placed)
//returns NULL if there is no point that is unavailable. Otherwise, returns an integer
//array with:
//-in the first position, the length of this array -1 (the effective number of unavailable points)
//-the indexes, on "points", of the points unavailable.
int* find_unavailable_points(point* points, int p_len, box* boxes_to_place, int btp_len){
    int tot_unavailable = 0;
    int* points_unavailable = malloc(p_len * sizeof(int));
    for(int i = 0; i < p_len; i++){
        point p = points[i];
        int unavailable = 1;
        for(int j = 0; j < btp_len; j++){
            box b = boxes_to_place[j];
            int min_box_dim = min(b.xlen, min(b.ylen, b.zlen));
            int max_point_dim = max(p.width, max(p.height, p.depth));
            //if the smallest dimension of a box is bigger than the greatest dimension
            //of a point, that box can't fit in that point. Otherwise, we have to investigate
            //further that box
            if(min_box_dim <= max_point_dim){
                int** rotations = rotations_of_box(b);
                for(int k = 0; k < 6; k++){
                    //let's see if this box, with this rotation, fits in that point.
                    //if it does, the point is available (there is at least a box that can stay there)
                    if(rotations[k][0] <= p.width && 
                        rotations[k][1] <= p.height && 
                        rotations[k][2] <= p.depth){
                            unavailable = 0;
                            break;
                        }
                }
                for(int i = 0; i < 6; i++){
                    free(rotations[i]);
                }
                free(rotations);
            }
            if(!unavailable){
                //no need to check other boxes: at least one of them can stay in this point
                break;
            }
        }
        if(unavailable){
            points_unavailable[tot_unavailable] = i;
            tot_unavailable += 1;
        }
    }

    

    int* ret = NULL;
    if(tot_unavailable > 0){
        printf("tot_unavailable = %d\n", tot_unavailable);
        ret = malloc((tot_unavailable+1) * sizeof(int));
        ret[0] = tot_unavailable;
        for(int i = 1; i < tot_unavailable + 1; i++){
            ret[i] = points_unavailable[i-1];
        }
    }
    free(points_unavailable);
    return ret;


}


//assumes that "indexes_to_exclude" contains values ordered from smallest to biggest
void exclude_unavailable_points(point** all_points, int all_points_len, 
                                int* indexes_to_exclude, int indexes_to_exclude_len){
    if(all_points_len - indexes_to_exclude_len == 0){
        //all points must be removed
        if(*all_points) free(*all_points);
        *all_points = NULL;
        return;
    }

    printf("all_points_len = %d, indexes_to_exclude_len = %d\n", all_points_len, indexes_to_exclude_len);
    point* new_set_of_points = malloc((all_points_len - indexes_to_exclude_len) * sizeof(point));
    int a = 0;
    int b = 0;
    int c = 0;
    //I check all points in "all_points".
    //If the current point must not be removed, I add it to "new_set_of_points".
    //Otherwise, i don't add it and increment the index "b" relative to "indexes_to_exclude"
    for(a = 0; a < all_points_len; a++){
        if(b < indexes_to_exclude_len){
            if(a < indexes_to_exclude[b]){
                copy_point(&new_set_of_points[c], (*all_points)[a]);
                c++;
            }else{
                b++;
            }
        }else{
            copy_point(&new_set_of_points[c], (*all_points)[a]);
            c++;
        }
    }

    if(*all_points) free(*all_points);
    *all_points = new_set_of_points;

    /*for(int i = 0; i < all_points_len - indexes_to_exclude_len; i++){
        printf("i = %d, %d %d %d %d %d %d\n", i, (*all_points)[i].x, (*all_points)[i].y, (*all_points)[i].z,
            (*all_points)[i].width, (*all_points)[i].height, (*all_points)[i].depth);
    }*/

    printf("points updated\n");

}



