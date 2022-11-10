#ifndef ADVANCED_POINTS_PLUS
#define ADVANCED_POINTS_PLUS

#include "structs.h"

void deduce_even_more_unavailable_points_volume(box** volumes_to_exclude, int* n_volumes_to_exclude,
                box* boxes_placed, int n_boxes_placed);
void add_points_from_volume_to_exclude(box b, point** points_to_consider, int* n_points_to_consider,
        box* volumes_to_exclude, int n_volumes_to_exclude, box* boxes_placed, int n_boxes_placed);
int is_point_inside_some_box(point p, box* boxes1, int n_boxes1, box* boxes2, int n_boxes2);
int is_point_inside_box(point p, box b);
box volume_induced_by_point_to_consider(point p, box* placed_boxes, int n_placed_boxes, 
                                                box* volumes_to_exclude, int n_volumes_to_exclude);
int is_box_left_touching___point(point p, box b);
int is_box_behind_touching___point(point p, box b);
int is_box_below_touching___point(point p, box b);
int* get_lengths_air_of_unavailable_point(point p, box* boxes, int n_boxes);


#endif