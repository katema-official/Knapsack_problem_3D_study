#ifndef EXTREME_POINTS
#define EXTREME_POINTS


#include "boxes.h"
#include "utils.h"
#include "structs.h"


int is_box_right_of_point(box b, point p);
int is_box_above_of_point(box b, point p);
int is_box_front_of_point(box b, point p);
void update_point_dimensions(point* p, box* boxes, int n_boxes);
point* get_copy_points_except_one(point* src, int n, int i);

#endif


