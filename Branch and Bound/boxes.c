#include "boxes.h"
#include "main.c"


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





