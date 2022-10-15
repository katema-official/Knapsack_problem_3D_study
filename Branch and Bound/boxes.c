#include "boxes.h"
#include "structs.h"

#include <stdlib.h>

extern int primal_bound;
extern int cont_x;
extern int cont_y;
extern int cont_z;
extern node* head;


int is_overlapping_1D(int min1, int max1, int min2, int max2){
    return max1 > min2 && max2 > min1;
}

int do_boxes_overlap(box b1, box b2){
    return is_overlapping_1D(b1.x0, b1.x0 + b1.xlen, b2.x0, b2.x0 + b2.xlen) &&
            is_overlapping_1D(b1.y0, b1.y0 + b1.ylen, b2.y0, b2.y0 + b2.ylen) &&
            is_overlapping_1D(b1.z0, b1.z0 + b1.zlen, b2.z0, b2.z0 + b2.zlen);
}

int is_box_outside_container(box b){
    if(b.x0 + b.xlen > cont_x && b.y0 + b.ylen > cont_y && b.z0 + b.zlen > cont_z){
        return 1;
    }
    return 0;
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

void copy_box(box* dst, box src){
    (*dst).xlen = src.xlen;
    (*dst).ylen = src.ylen;
    (*dst).zlen = src.zlen;
    (*dst).x0 = src.x0;
    (*dst).y0 = src.y0;
    (*dst).z0 = src.z0;
}

void copy_boxes(box** dst, box* src, int len){
    for(int i = 0; i < len; i++){
        (*dst)[i].xlen = src[i].xlen;
        (*dst)[i].ylen = src[i].ylen;
        (*dst)[i].zlen = src[i].zlen;
        (*dst)[i].x0 = src[i].x0;
        (*dst)[i].y0 = src[i].y0;
        (*dst)[i].z0 = src[i].z0;
    }
}

int** rotations_of_box(box b){
    int** rots = malloc(6*sizeof(int*));

    for(int i=0; i < 6; i++){
        rots[i] = malloc(3*sizeof(int));
    }

    int xlen = b.xlen;
    int ylen = b.ylen;
    int zlen = b.zlen;

    rots[0][0] = xlen;
    rots[0][1] = ylen;
    rots[0][2] = zlen;

    rots[1][0] = xlen;
    rots[1][1] = zlen;
    rots[1][2] = ylen;

    rots[2][0] = ylen;
    rots[2][1] = xlen;
    rots[2][2] = zlen;

    rots[3][0] = ylen;
    rots[3][1] = zlen;
    rots[3][2] = xlen;

    rots[4][0] = zlen;
    rots[4][1] = xlen;
    rots[4][2] = ylen;

    rots[5][0] = zlen;
    rots[5][1] = ylen;
    rots[5][2] = xlen;

    return rots;

}



box* get_copy_boxes_except_one(box* src, int n, int i){
    box* dst = malloc((n-1)*sizeof(box));
    for(int j = 0; j < n; j++){
        if(j < i){
            dst[j].x0 = src[j].x0;
            dst[j].y0 = src[j].y0;
            dst[j].z0 = src[j].z0;
            dst[j].xlen = src[j].xlen;
            dst[j].ylen = src[j].ylen;
            dst[j].zlen = src[j].zlen;
        }

        if(j > i){
            dst[j-1].x0 = src[j].x0;
            dst[j-1].y0 = src[j].y0;
            dst[j-1].z0 = src[j].z0;
            dst[j-1].xlen = src[j].xlen;
            dst[j-1].ylen = src[j].ylen;
            dst[j-1].zlen = src[j].zlen;
        }
    }
    return dst;
}



