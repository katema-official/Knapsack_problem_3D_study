void initialize_array_local_optimum();
void free_array_local_optimum();
void reset_array_local_optimum();
void evaluate_new_solution(int volume_of_this_solution, box* boxes, int* a, int* b, int* c, int n_boxes);
void save_local_optimum(box* boxes, int* a, int* b, int* c, int n_boxes, int append_or_overwrite);
void result_print_local_optimum_found(int n_boxes, char** boxes_names);
void print_occupancy(int cont_x, int cont_y, int cont_z);
