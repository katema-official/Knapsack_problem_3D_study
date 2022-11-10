#ifndef INVESTIGATE_POINTS_WITH_BOXES_AROUND
#define INVESTIGATE_POINTS_WITH_BOXES_AROUND

#include "extreme_points.h"
#include "boxes.h"

box* boxes_around_point(point p, box* boxes, int n_boxes);
box compute_volume_coverable_from_point(point p, box* boxes, int n_boxes);
void project_unavailable_point_down(point* p, box* placed_boxes, int n_placed_boxes, box* unavailable_volumes, int n_unavailable_volumes);


#endif