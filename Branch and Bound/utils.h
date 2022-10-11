#include "boxes.h"
#include "extreme_points.h"

typedef struct{
    point* extreme_points;
    box* boxes_to_place;
    box* boxes_placed;
    node* succ;
}node;

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


int comparator_boxes_volume(const void* a, const void* b);
double get_random();
void copy_boxes(box** dst, box* src, int len);





