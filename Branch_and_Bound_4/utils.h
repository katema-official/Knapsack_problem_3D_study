#ifndef UTILS
#define UTILS

#include "structs.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define DEBUG_PROGRESS 1
#define DEBUG_1 0
#define DEBUG_2 0
#define DEBUG_3 0
#define DEBUG_MISC 0


//int comparator_boxes_volume(const void* a, const void* b);
double get_random();
//void copy_boxes(box** dst, box* src, int len);

int capacity_minus_placed_boxes(int capacity, box* boxes_placed, int bp_len);
void reset_progress_file();
void append_progress_file_partial_solution(box* boxes_placed, int n_boxes, 
                                    box* volumes_to_exclude, int n_volumes_to_exclude);
void append_progress_file_solution_separator(int fate, int dual, int primal);

#endif


