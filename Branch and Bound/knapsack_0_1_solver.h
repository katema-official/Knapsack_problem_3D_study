#ifndef KP_SOLVER_0_1
#define KP_SOLVER_0_1

#include "structs.h"

int solve_knapsack_0_1(int* volumes, int n_items, int capacity);
int solve_knapsack_0_1_v2(int* volumes, int n_items, int capacity);
int get_dual_bound_using_kp_0_1(int* volumes_of_boxes_placed, int n_boxes_placed, int max_capacity);
int are_array_contents_the_same(int* first_array, int* second_array, int n);
kp_sol_node* create_new_kp_sol_node(int max_capacity, int n_boxes_placed, int* volumes_of_boxes_placed);

//BFS stuff
int get_dual_bound_using_kp_0_1_bfs(int* volumes_of_boxes_placed, int n_boxes_placed, int max_capacity,
                    box* boxes_to_place, int n_boxes_to_place);
kp_sol_node* create_new_kp_sol_node_bfs(int max_capacity, 
                    int n_boxes_placed, int* volumes_of_boxes_placed, 
                    int n_boxes_to_place, box* boxes_to_place);


#endif