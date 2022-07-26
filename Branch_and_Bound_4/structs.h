#ifndef STRUCTS
#define STRUCTS

typedef struct {
	int xlen;
    int ylen;
    int zlen;
	int x0;
    int y0;
    int z0;
}box;

typedef enum{
    right_of_box,
    top_of_box,
    front_of_box
}point_spawnpoint;

typedef struct{
    int x;
    int y;
    int z;
    int width;
    int height;
    int depth;
    point_spawnpoint spawnpoint;
}point;

typedef struct node{
    point* extreme_points;
    int ep_len;
    box* boxes_to_place;
    int btp_len;
    box* boxes_placed;
    int bp_len;
    box* volumes_to_exclude; //they are not actual boxes: they are the volumes (that have the shape of a box) induced by unavailable points
    int vte_len;
    struct node* succ;
}node;

//struct that keeps the best solution of a knapsack problem.
typedef struct kp_sol_node{
    int* placed_boxes_volumes;  
    int n_placed_boxes_volumes;
    int max_capacity;
    int best_solution;
    struct kp_sol_node* succ;
}kp_sol_node;



#endif