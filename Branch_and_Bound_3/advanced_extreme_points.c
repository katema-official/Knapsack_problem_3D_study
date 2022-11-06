#include "advanced_extreme_points.h"
#include "structs.h"
#include "extreme_points.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_ADVANCED_POINTS 0


//function that, given a point, if it is a point spawned on the right or in front of box,
//projects it on the first available surface below.
void project_point_down(point* p, box* placed_boxes, int n){
    if(p->spawnpoint == top_of_box) return;
    int point_height = 0;
    for(int i = 0; i < n; i++){
        box b = placed_boxes[i];
        if(b.x0 <= p->x && p->x < b.x0 + b.xlen &&
            b.z0 <= p->z && p->z < b.z0 + b.zlen &&
            b.y0 + b.ylen <= p->y){
                //the upper face of the box is below the point
                point_height = max(point_height, b.y0 + b.ylen);
        }
    }
    p->y = point_height;
    update_point_dimensions(p, placed_boxes, n);
    
}





int comp_left(const void * a, const void * b){
    box a1 = *(const box*) a;
    box a2 = *(const box*) b;
    return a1.z0 - a1.z0; 
}

//despite the name, it returns the (minimum, certain) "box" whose volume can't be occupied given that point
box find_box_touching_point_left(point p, box* boxes_placed, int n_boxes_placed){
    box box_left;
    box box_behind;
    box box_below;
    box_left.x0 = -1;
    box_behind.x0 = -1;
    box_below.x0 = -1;
    
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];

        //if this box is the one EXACTLY behind wrt the point p...
        if(b.z0 + b.zlen == p.z && 
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
                b.x0 <= p.x && p.x < b.x0 + b.xlen){
                    box_behind = b;
                    if(DEBUG_ADVANCED_POINTS) printf("box behind (1) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_behind.x0, box_behind.y0, box_behind.z0);
        }

        //if this box is the one EXACTLY on the left wrt the point p...
        if(b.x0 + b.xlen == p.x &&
            b.y0 == p.y && b.z0 == p.z){
                box_left = b;
                if(DEBUG_ADVANCED_POINTS) printf("box on the left (1) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_left.x0, box_left.y0, box_left.z0);
        }
        
        //if this box is the one EXACTLY below wrt point p...
        if(b.y0 + b.ylen == p.y &&
            b.z0 <= p.z && p.z < b.z0 + b.zlen &&
                b.x0 <= p.x && p.x < b.x0 + b.xlen){
                    box_below = b;
                    if(DEBUG_ADVANCED_POINTS) printf("box below (1) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_left.x0, box_left.y0, box_left.z0);
        }

        //SALVATELO
        //POI LA Z DIVENTA IL MINIMO TRA LA Z DI QUELLO A SINISTRA E LA Z DI QUELLO SOTTO
        //E   LA X DIVENTA IL MINIMO TRA LA X DI QUELLO DIETRO E LA X DI QUELLO SOTTO
    }

    box b_res;
    b_res.x0 = -1;

    int z;
    if(box_left.x0 != -1){
        z = box_left.zlen;
        if(box_below.x0 != -1){
            z = min(z, box_below.z0 + box_below.zlen - p.z);
        }
    }else{
        //since there is no face (of another box, because of projection) on the left, we can't say anything
        return b_res;
    }

    int x;
    if(box_behind.x0 != -1){
        x = box_behind.x0 + box_behind.xlen - p.x;
        if(box_below.x0 != -1){
            x = min(x, box_below.x0 + box_below.xlen - p.x);
        }
    }else if(p.z == 0){
        x = p.width;    //works if all the points were updated before calling this function
        if(box_below.x0 != -1){
            x = min(x, box_below.x0 + box_below.xlen - p.x);
        }
    }else{
        //since there is no face (of another box or of the wall) behind, we can't say anything
        return b_res;
    }

    int y;
    if(box_behind.x0 != -1){
        y = min(box_behind.ylen, box_left.ylen);
    }else{
        y = box_left.ylen;
    }

    b_res.x0 = p.x;
    b_res.y0 = p.y;
    b_res.z0 = p.z;
    b_res.xlen = x;
    b_res.ylen = y;
    b_res.zlen = z;

    if(x == 0 || y == 0 || z == 0){
        b_res.x0 = -1;
    }
    
    return b_res;
}

box find_box_touching_point_below(point p, box* boxes_placed, int n_boxes_placed){
    box box_below;
    box box_left;
    box box_behind;
    box_below.x0 = -1;
    box_left.x0 = -1;
    box_behind.x0 = -1;

    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];
        //point p was "born" above a box. First, let's find it.
        if(b.y0 + b.ylen == p.y && b.x0 == p.x && b.z0 == p.z){
            box_below = b;
            if(DEBUG_ADVANCED_POINTS) printf("box below (3) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_below.x0, box_below.y0, box_below.z0);
        }

        //then we need to find the two boxes on the left...
        if(b.x0 + b.xlen == p.x &&
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
            b.z0 <= p.z && p.z < b.z0 + b.zlen){
                box_left = b;
                if(DEBUG_ADVANCED_POINTS) printf("box on the left (3) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_left.x0, box_left.y0, box_left.z0);
        }
        //...and behind
        if(b.z0 + b.zlen == p.z &&
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
            b.x0 <= p.x && p.x < b.x0 + b.xlen){
                box_behind = b;
                if(DEBUG_ADVANCED_POINTS) printf("box behind (3) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_behind.x0, box_behind.y0, box_behind.z0);
        }
    }

    box b_res;
    b_res.x0 = -1;

    if(box_behind.x0 == -1){
        //no box behind was found
        if(p.z > 0){
            return b_res;
        }
    }
    if(box_left.x0 == -1){
        //no box on the left was found
        if(p.x > 0){
            return b_res;
        }
    }


    
    int x;
    int y;
    int z;

    if(box_behind.x0 != -1){
        x = min(box_below.xlen, box_behind.x0 + box_behind.xlen - p.x);
    }else{
        x = box_below.xlen;
    }

    if(box_left.x0 != -1){
        z = min(z = box_below.zlen, box_left.z0 + box_left.zlen - p.z);
    }else{
        z = box_below.zlen;
    }

    if(box_behind.x0 != -1 && box_left.x0 != -1){
        y = min(box_behind.y0 + box_behind.ylen - p.y, box_left.y0 + box_left.ylen - p.y);
    }else if(box_behind.x0 == -1 && box_left.x0 == -1){
        y = p.height;   //the point has p.x = p.z = 0
    }else if(box_behind.x0 == -1 && p.z == 0){
        y = box_left.y0 + box_left.ylen - p.y;
    }else if(box_left.x0 == -1 && p.x == 0){
        y = box_behind.y0 + box_behind.ylen - p.y;
    }

    b_res.x0 = p.x;
    b_res.y0 = p.y;
    b_res.z0 = p.z;
    b_res.xlen = x;
    b_res.ylen = y;
    b_res.zlen = z;

    if(x == 0 || y == 0 || z == 0){
        b_res.x0 = -1;
    }

    return b_res;

}

box find_box_touching_point_behind(point p, box* boxes_placed, int n_boxes_placed){
    box box_left;
    box box_behind;
    box box_below;
    box_left.x0 = -1;
    box_behind.x0 = -1;
    box_below.x0 = -1;
    
    for(int i = 0; i < n_boxes_placed; i++){
        box b = boxes_placed[i];

        //if this box is the one EXACTLY on the left wrt the point p...
        if(b.x0 + b.xlen == p.x && 
            b.y0 <= p.y && p.y < b.y0 + b.ylen &&
                b.z0 <= p.z && p.z < b.z0 + b.zlen){
                    box_left = b;
                    if(DEBUG_ADVANCED_POINTS) printf("box on the left (2) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_left.x0, box_left.y0, box_left.z0);
        }

        //if this box is the one EXACTLY behind wrt the point p...
        if(b.z0 + b.zlen == p.z &&
            b.x0 == p.x && b.y0 == p.y){
                box_behind = b;
                if(DEBUG_ADVANCED_POINTS) printf("box behind (2) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_behind.x0, box_behind.y0, box_behind.z0);
        }

        //if this box is the one EXACTLY below wrt point p...
        if(b.y0 + b.ylen == p.y &&
            b.z0 <= p.z && p.z < b.z0 + b.zlen &&
                b.x0 <= p.x && p.x < b.x0 + b.xlen){
                    box_below = b;
                    if(DEBUG_ADVANCED_POINTS) printf("box below (2) found.\
                        (x0, y0, z0) of box = (%d,%d,%d). This message should not appear twice in a row\n", 
                        box_left.x0, box_left.y0, box_left.z0);
        }

        //if this box is the one EXACTLY below wrt point p...
        //SALVATELO
        //POI LA Z DIVENTA IL MINIMO TRA LA Z DI QUELLO A SINISTRA E LA Z DI QUELLO SOTTO
        //E   LA X DIVENTA IL MINIMO TRA LA X DI QUELLO DIETRO E LA X DI QUELLO SOTTO
    }

    box b_res;
    b_res.x0 = -1;

    int x;
    if(box_behind.x0 != -1){
        x = box_behind.xlen;
        if(box_below.x0 != -1){
            x = min(x, box_below.x0 + box_below.xlen - p.x);
        }
    }else{
        //since there is no face (of another box, because of projection) behind, we can't say anything
        return b_res;
    }
    
    int z;
    if(box_left.x0 != -1){
        z = box_left.z0 + box_left.zlen - p.z;
        if(box_below.x0 != -1){
            z = min(z, box_below.z0 + box_below.zlen - p.z);
        }
    }else if(p.x == 0){
        z = p.depth;    //works if all the points were updated before calling this function
        if(box_below.x0 != -1){
            z = min(z, box_below.z0 + box_below.zlen - p.z);
        }
    }else{
        //Since there is no face (of another box or of the wall) on the left, we can't say anything
        return b_res;
    }

    int y;
    if(box_left.x0 != -1){
        y = min(box_behind.ylen, box_left.ylen);
    }else{
        y = box_behind.ylen;
    }

    b_res.x0 = p.x;
    b_res.y0 = p.y;
    b_res.z0 = p.z;
    b_res.xlen = x;
    b_res.ylen = y;
    b_res.zlen = z;

    if(x == 0 || y == 0 || z == 0){
        b_res.x0 = -1;
    }

    return b_res;

}













//function that, given the points that have to be excluded, computes the volumes to be excluded
//induced by them. The points to be excluded
//can have their dimensions of two kind:
//1) they touch one of the container's walls.
//2) they touch an already placed box.
//in the first case, the actual length that contributes to the volume to be removed is
//smaller than the total length of the point.
box* get_unavailable_points_volume(box* boxes_placed, int n_boxes_placed,
                    point* points_to_exclude, int n_points_to_exclude){
    box* tmp = malloc(n_points_to_exclude*sizeof(box));
    int tmp_len = 0;
    for(int i = 0; i < n_points_to_exclude; i++){
        point p = points_to_exclude[i];
        box b_found;
        if(DEBUG_1) printf("p to remove from capacity = %d %d %d - %d\n", p.x, p.y, p.z, p.spawnpoint);
        switch(p.spawnpoint){
            case right_of_box:
                b_found = find_box_touching_point_left(p, boxes_placed, n_boxes_placed);
                if(b_found.x0 != -1){
                    tmp[tmp_len] = b_found;
                    tmp_len++;
                }
            break;
            case top_of_box:
                b_found = find_box_touching_point_below(p, boxes_placed, n_boxes_placed);
                if(b_found.x0 != -1){
                    tmp[tmp_len] = b_found;
                    tmp_len++;
                }
            break;
            case front_of_box:
                b_found = find_box_touching_point_behind(p, boxes_placed, n_boxes_placed);
                if(b_found.x0 != -1){
                    tmp[tmp_len] = b_found;
                    tmp_len++;
                }
            break;
            default:
            break;
        }
    }

    box* res = malloc((1 + tmp_len) * sizeof(box));
    res[0].x0 = tmp_len;    //yeahhhh I know this is a bit of acrobatics, but if I don't do this it ould get even messier
    for(int i = 1; i < 1 + tmp_len; i++){
        copy_box(&(res[i]), tmp[i-1]);
    }
    free(tmp);
    return res;
}


//function to check if a point we'd like to add to a list of points is already present
int is_point_not_redundant(point new_p, point* points, int n_points){
    for(int i = 0; i < n_points; i++){
        if(new_p.x == points[i].x && new_p.y == points[i].y && new_p.z == points[i].z){
            return 0;
        }
    }
    return 1;
}







//function that takes the current array of boxes representing unavailable volumes and updates it by adding new boxes
//of this kind.
void add_new_boxes_volumes_unavailable(box** current_boxes_volumes_unavailable, int* n_current_boxes_volumes_unavailable,
                                    box* new_boxes_volumes_unavailable, int n_new_boxes_volumes_unavailable){
    box* res = malloc(((*n_current_boxes_volumes_unavailable) + n_new_boxes_volumes_unavailable) * sizeof(box));
    for(int i = 0; i < (*n_current_boxes_volumes_unavailable); i++){       //if the current_... is NULL, n_current_... is 0, so no problem!
        copy_box(&(res[i]), (*current_boxes_volumes_unavailable)[i]);
    }
    for(int i = (*n_current_boxes_volumes_unavailable); 
            i < (*n_current_boxes_volumes_unavailable) + n_new_boxes_volumes_unavailable; 
                i++){
                    copy_box(&(res[i]), new_boxes_volumes_unavailable[i - (*n_current_boxes_volumes_unavailable)]);
    }
    free(*current_boxes_volumes_unavailable);
    *current_boxes_volumes_unavailable = res;
    *n_current_boxes_volumes_unavailable = (*n_current_boxes_volumes_unavailable) + n_new_boxes_volumes_unavailable;



}