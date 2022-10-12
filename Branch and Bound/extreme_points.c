#include "extreme_points.h"



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