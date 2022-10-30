#ifndef ADVANCED_POINTS
#define ADVANCED_POINTS

#include "structs.h"


void project_point_down(point* p, box* placed_boxes, int n);
int capacity_minus_unavailable_points_volume(int capacity, box* boxes_placed, int n_boxes_placed,
                    point* points_to_exclude, int n_points_to_exclude);


#endif