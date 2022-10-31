#ifndef UTILS
#define UTILS

#include "structs.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define DEBUG_PROGRESS 1


//int comparator_boxes_volume(const void* a, const void* b);
double get_random();
//void copy_boxes(box** dst, box* src, int len);

int capacity_minus_placed_boxes(int capacity, box* boxes_placed, int bp_len);
void reset_progress_file();
void append_progress_file(box* boxes_placed, int n_boxes);

#endif


