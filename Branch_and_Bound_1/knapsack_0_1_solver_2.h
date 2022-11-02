#ifndef KP_SOLVER_0_1_v2
#define KP_SOLVER_0_1_v2

int solve_knapsack_0_1_recursive_v1(int* volumes, int len, int max_capacity, int index, int current_volume_occupied);
int solve_knapsack_0_1_recursive_v2(int* volumes, int len, int max_capacity, int max_depth);
void explore_subtree(int* partial_solutions, int start, int end,
    int root_value, int* volumes, int index, int depth, int max_capacity, int pos);
void add_new(int* partial_solutions, int* partial_solutions_index, int val);
int max_in_array(int* a, int len);



#endif