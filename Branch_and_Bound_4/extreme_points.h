#ifndef EXTREME_POINTS
#define EXTREME_POINTS


#include "boxes.h"
#include "utils.h"
#include "structs.h"


int is_box_right_of_point(box b, point p);
int is_box_above_of_point(box b, point p);
int is_box_front_of_point(box b, point p);
void create_point_dimensions(point* p);
void update_point_dimensions(point* p, box* boxes, int n_boxes);
void copy_point(point* dst, point src);
point* get_copy_points_except_one(point* src, int n, int i, int n_new_points);
int* find_unavailable_points(point* points, int p_len, box* boxes_to_place, int btp_len);
void exclude_unavailable_points(point** all_points, int all_points_len, 
                                int* indexes_to_exclude, int indexes_to_exclude_len);

#endif


