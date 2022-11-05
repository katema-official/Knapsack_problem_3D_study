#ifndef ADVANCED_POINTS
#define ADVANCED_POINTS

#include "structs.h"


void project_point_down(point* p, box* placed_boxes, int n);
box* get_unavailable_points_volume(box* boxes_placed, int n_boxes_placed,
                    point* points_to_exclude, int n_points_to_exclude);
int is_point_not_redundant(point new_p, point* points, int n_points);
void add_new_boxes_volumes_unavailable(box** current_boxes_volumes_unavailable, int* n_current_boxes_volumes_unavailable,
                                    box* new_boxes_volumes_unavailable, int n_new_boxes_volumes_unavailable);

#endif