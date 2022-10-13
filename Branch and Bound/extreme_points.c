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

