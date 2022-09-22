#include "utils_boxes.h"
#include <stdlib.h>


int* random_permutation_1_to_n(int n) {
    int* r = malloc(n * sizeof(int));
    // initial range of numbers
    for(int i=0;i<n;++i){
        r[i]=i+1;
    }
    // shuffle
    for (int i = n-1; i >= 0; --i){
        //generate a random number [0, n-1]
        int j = rand() % (i+1);

        //swap the last element with element at random index
        int temp = r[i];
        r[i] = r[j];
        r[j] = temp;
    }
    return r;
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
