
typedef struct {
	char* name;
	int xlen;
    int ylen;
    int zlen;
	int x0;
    int y0;
    int z0;
}box;

void debug_print(int n_boxes, box* boxes, int* a, int* b, int* c);
int* random_permutation_1_to_n(int n);
int comparator_boxes_volume(const void* a, const void* b);
void permute_box_dimensions(box* this_box, int perm);
void swap_in_array(int** sequence, int i, int j);
void get_neighbour(box** boxes_neighbour, int n_boxes, int** current_a, int** current_b, int** current_c);
void sa_make_a_step(box** boxes, int n_boxes, int* temperature, int alpha, int beta, 
                    int** current_a, int** current_b, int** current_c, 
                    int** best_a, int** best_b, int** best_c, int* best_volume, box** boxes_neighbour);
box* simulated_annealing_knapsack_3D(int* a, int* b, int* c, box* boxes_input, int n_boxes, int md, int secs);
int volume_occupied(box* boxes, int n_boxes, int cont_x, int cont_y, int cont_z, int mode);
