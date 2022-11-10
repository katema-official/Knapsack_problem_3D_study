#include "investigate_points_with_boxes_around.h"
#include "boxes.h"
#include "extreme_points.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>


extern int cont_x;
extern int cont_y;
extern int cont_z;

//function to check if a box is EXACTLY on the left wrt a point
int is_box_left_touching___point(point p, box b){
    if(b.x0 + b.xlen == p.x && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            printf("yes left\n");
            return 1;
    }
    return 0;
}

//function to check if a box is EXACTLY behind wrt a point
int is_box_behind_touching___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 + b.zlen == p.z){
            printf("yes behind\n");
            return 1;
    }
    return 0;
}

//function to check if a box is EXACTLY below wrt a point
int is_box_below_touching___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 + b.ylen == p.y && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            printf("yes below\n");
            return 1;
    }
    return 0;
}



//function to check if a box is to the right wrt a point
int is_box_right_of___point(point p, box b){
    if(b.x0 >= p.x && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            return 1;
    }
    return 0;
}

//function to check if a box is above wrt a point
int is_box_front_of___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 <= p.y && p.y < b.y0 + b.ylen && 
        b.z0 >= p.z){
            return 1;
    }
    return 0;
}

//function to check if a box is above wrt a point
int is_box_above_of___point(point p, box b){
    if(b.x0 <= p.x && p.x < b.x0 + b.xlen && 
        b.y0 >= p.y && 
        b.z0 <= p.z && p.z < b.z0 + b.zlen){
            return 1;
    }
    return 0;
}



//function that returns an array of six boxes, given an array:
//1) the one on its left
//2) the one behind
//3) the one below
//4) the one on the right
//5) the one in front
//6) the one above
//if one of this boxes does not exist, its x0 = -1 /////////////and its dimensions will be the container dimensions + 1
box* boxes_around_point(point p, box* boxes, int n_boxes){
    box box_left;
    box box_behind;
    box box_below;
    box_left.x0 = -1;
    box_behind.x0 = -1;
    box_below.x0 = -1;
    box_left.y0 = -1;
    box_behind.y0 = -1;
    box_below.y0 = -1;
    box_left.z0 = -1;
    box_behind.z0 = -1;
    box_below.z0 = -1;
    box_left.xlen = cont_x + 1;
    box_left.ylen = cont_y + 1;
    box_left.zlen = cont_z + 1;
    box_behind.xlen = cont_x + 1;
    box_behind.ylen = cont_y + 1;
    box_behind.zlen = cont_z + 1;
    box_below.xlen = cont_x + 1;
    box_below.ylen = cont_y + 1;
    box_below.zlen = cont_z + 1;

    box box_right;
    box box_front;
    box box_above;
    box_right.x0 = -1;
    box_front.x0 = -1;
    box_above.x0 = -1;
    box_right.y0 = -1;
    box_front.y0 = -1;
    box_above.y0 = -1;
    box_right.z0 = -1;
    box_front.z0 = -1;
    box_above.z0 = -1;
    box_right.xlen = cont_x + 1;
    box_right.ylen = cont_y + 1;
    box_right.zlen = cont_z + 1;
    box_front.xlen = cont_x + 1;
    box_front.ylen = cont_y + 1;
    box_front.zlen = cont_z + 1;
    box_above.xlen = cont_x + 1;
    box_above.ylen = cont_y + 1;
    box_above.zlen = cont_z + 1;

    for(int i = 0; i < n_boxes; i++){
        box b = boxes[i];
        if(is_box_left_touching___point(p, b)){
            box_left = b;
        }else if(is_box_behind_touching___point(p, b)){
            box_behind = b;
        }else if(is_box_below_touching___point(p, b)){
            box_below = b;
        }else if(is_box_right_of___point(p, b)){
            box_right = b;
        }else if(is_box_front_of___point(p, b)){
            box_front = b;
        }else if(is_box_above_of___point(p, b)){
            box_above = b;
        }
    }

    box* ret = malloc(6*sizeof(box));
    copy_box(&ret[0], box_left);
    copy_box(&ret[1], box_behind);
    copy_box(&ret[2], box_below);
    copy_box(&ret[3], box_right);
    copy_box(&ret[4], box_front);
    copy_box(&ret[5], box_above);

    printf("################ SCATOLE ATTORNO AL PUNTO %d %d %d ##############\n", p.x, p.y, p.z);
    for(int i = 0; i < 6; i++){
        printf("%d %d %d %d %d %d\n", ret[i].xlen, ret[i].ylen, ret[i].zlen, ret[i].x0, ret[i].y0, ret[i].z0);
    }
    printf("#################################################################\n");

    return ret;
}



//function that, given a point and some surrounding boxes, finds the minimume volume
//that can be generated from that point without "annoying" any other box.
//In other words: the maximum volume that can be filled without risking to take some
//volume that another box could fill.
//this volume is given as a box.
//
//if the point does not have a face on its left, on its behind and on its below,
//the box returned has x0 = -1
//
//the boxes_around are the boxes:
//-on the left
//-behind
//-below
//-on the right
//-in front
//-above
//wrt point p
//
//returs a box with x0 = -1 if there is no volume that this point can cover
box compute_volume_coverable_from_point(point p, box* boxes, int n_boxes){
    box* boxes_around = boxes_around_point(p, boxes, n_boxes);
    box ret;
    ret.x0 = -1;
    box box_left = boxes_around[0];
    box box_behind = boxes_around[1];
    box box_below = boxes_around[2];
    box box_right = boxes_around[3];
    box box_front = boxes_around[4];
    box box_above = boxes_around[5];

    //if the box we'd create in this way doesn't have a left, behind or lower face, we don't create any volume
    if((box_left.x0 == -1 && p.x > 0) || (box_behind.x0 == -1 && p.z > 0) || (box_below.x0 == -1 && p.y > 0)){
        free(boxes_around);
        return ret;
    }

    int x1 = box_behind.x0 + box_behind.xlen - p.x;
    int x2 = box_front.x0 + box_front.xlen - p.x;
    int x3 = box_below.x0 + box_below.xlen - p.x;
    int x4 = box_above.x0 + box_above.xlen - p.x;

    int y1 = box_behind.y0 + box_behind.ylen - p.y;
    int y2 = box_front.y0 + box_front.ylen - p.y;
    int y3 = box_left.y0 + box_left.ylen - p.y;
    int y4 = box_right.y0 + box_right.ylen - p.y;

    int z1 = box_left.z0 + box_left.zlen - p.z;
    int z2 = box_right.z0 + box_right.zlen - p.z;
    int z3 = box_below.z0 + box_below.zlen - p.z;
    int z4 = box_above.z0 + box_above.zlen - p.z;

    int x = min(x1, x3);
    int y = min(y1, y3);
    int z = min(z1, z3);

    //if there is a box placed above this unavailable volume, and it is above the box behind...
    if(box_above.x0 != -1 && (box_above.y0 == box_behind.y0 + box_behind.ylen)){
        //... we have to check if, because of it, a box was placed in front of it (pulled to the ground)
        if(box_front.z0 == box_above.z0 + box_above.zlen){
            z = min(z, box_front.z0 - p.z);
        }
    }

    //if there is a box placed above this unavailable volume, and it is above the box on the left...
    if(box_above.x0 != -1 && (box_above.y0 == box_left.y0 + box_left.ylen)){
        //... we have to check if, because of it, a box was placed on the right of it (pulled to the ground)
        if(box_right.x0 == box_above.x0 + box_above.xlen){
            x = min(x, box_right.x0 - p.x);
        }
    }

    //int x = min(x1, min(x2, min(x3, x4)));
    //int y = min(y1, min(y2, min(y3, y4)));
    //int z = min(z1, min(z2, min(z3, z4)));
    
    ret.x0 = p.x;
    ret.y0 = p.y;
    ret.z0 = p.z;
    ret.xlen = x;
    ret.ylen = y;
    ret.zlen = z;
    free(boxes_around);

    if(ret.xlen == 0 || ret.ylen == 0 || ret.zlen == 0){
        ret.x0 = -1;
        return ret;
    }


    return ret;

}



//####################################################################
//function that, given an unavailable point, if it is a point spawned on the right or in front of a box,
//projects it on the first available surface below.
void project_unavailable_point_down(point* p, box* placed_boxes, int n_placed_boxes, box* unavailable_volumes, int n_unavailable_volumes){
    if(p->spawnpoint == top_of_box) return;

    printf("Hi| p.x = %d\n", p->x);

    int point_height_1 = 0;
    for(int i = 0; i < n_placed_boxes; i++){
        box b = placed_boxes[i];
        if(b.x0 <= p->x && p->x < b.x0 + b.xlen &&
            b.z0 <= p->z && p->z < b.z0 + b.zlen &&
            b.y0 + b.ylen <= p->y){
                //the upper face of the box is below the point
                point_height_1 = max(point_height_1, b.y0 + b.ylen);
        }
    }

    int point_height_2 = 0;
    for(int i = 0; i < n_unavailable_volumes; i++){
        box b = unavailable_volumes[i];
        if(b.x0 <= p->x && p->x < b.x0 + b.xlen &&
            b.z0 <= p->z && p->z < b.z0 + b.zlen &&
            b.y0 + b.ylen <= p->y){
                //the upper face of the box is below the point
                point_height_2 = max(point_height_2, b.y0 + b.ylen);
        }
    }

    p->y = max(point_height_1, point_height_2);
    
}



