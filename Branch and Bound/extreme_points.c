#include "extreme_points.h"
#include <stdlib.h>


int is_box_right_of_point(box b, point p){
    return b.x0 >= p.x;
}
int is_box_above_of_point(box b, point p){
    return b.y0 >= p.y;
}
int is_box_front_of_point(box b, point p){
    return b.z0 >= p.z;
}


//function that, given the boxes already placed (boxes) and the extreme point p,
//computes how much free space is there to the right, above and front of p
void update_point_dimensions(point* p, box* boxes, int n_boxes){
    for(int i = 0; i < n_boxes; i++){
        box b = boxes[i];
        
        if(is_box_right_of_point(b, *p) && b.y0 <= (*p).y && (*p).y < b.y0 + b.ylen && 
                                            b.z0 <= (*p).z && (*p).z < b.z0 + b.zlen){
            p->width = min(p->width, b.x0);
        }

        if(is_box_above_of_point(b, *p) && b.x0 <= (*p).x && (*p).x < b.x0 + b.xlen && 
                                            b.z0 <= (*p).z && (*p).z < b.z0 + b.zlen){
            p->height = min(p->height, b.y0);
        }

        if(is_box_front_of_point(b, *p) && b.y0 <= (*p).y && (*p).y < b.y0 + b.ylen && 
                                            b.x0 <= (*p).x && (*p).x < b.x0 + b.xlen){
            p->depth = min(p->depth, b.z0);
        }
    }
}

point* get_copy_points_except_one(point* src, int n, int i){
    point* dst = malloc((n-1+2)*sizeof(point)); //remove a point, add two more. This happens when a box is placed
    for(int j = 0; j < n; j++){
        if(j < i){
            dst[j].x = src[j].x;
            dst[j].y = src[j].y;
            dst[j].z = src[j].z;
            dst[j].width = src[j].width;
            dst[j].height = src[j].height;
            dst[j].depth = src[j].depth;
        }

        if(j > i){
            dst[j-1].x = src[j].x;
            dst[j-1].y = src[j].y;
            dst[j-1].z = src[j].z;
            dst[j-1].width = src[j].width;
            dst[j-1].height = src[j].height;
            dst[j-1].depth = src[j].depth;
        }
    }
    return dst;
}

//function that, given the extreme points and the boxes still to place, tells if some
//points are such that NO box can be placed there (to be as general as possible, we
//will return the indexes, of the extreme_points array passed, of those points that
//CANNOT CONTAIN ANY BOX, of those to be placed)
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
            }
            if(!unavailable){
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
        ret = malloc(tot_unavailable * sizeof(int));
        for(int i = 0; i < tot_unavailable; i++){
            ret[i] = points_unavailable[i];
        }
        free(points_unavailable);
    }

    return ret;


}

