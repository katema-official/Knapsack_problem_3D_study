#ifndef BOXES
#define BOXES

#include "structs.h"

int is_overlapping_1D(int min1, int max1, int min2, int max2);
int do_boxes_overlap(box b1, box b2);
int is_box_outside_container(box b);
int comparator_boxes_volume(const void* a, const void* b);
void copy_box(box* dst, box src);
void copy_boxes(box** dst, box* src, int len);
int** rotations_of_box(box b);


#endif





