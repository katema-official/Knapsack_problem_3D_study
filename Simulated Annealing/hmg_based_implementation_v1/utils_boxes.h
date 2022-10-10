typedef struct {
	//char* name;
	int xlen;
    int ylen;
    int zlen;
	int x0;
    int y0;
    int z0;
}box;


#define DEBUG_0 0

double get_random();
void copy_boxes(box** dst, box* src, int len);
void copy_sequence(int** dst, int* src, int len);
void progression_print(int n_boxes, box* boxes, int* a, int* b, int*c, char* filename);
void debug_print(int n_boxes, box* boxes, int* a, int* b, int* c);
int* random_permutation_1_to_n(int n);
int comparator_boxes_volume(const void* a, const void* b);
void permute_box_dimensions(box* this_box, int perm);
void swap_in_array(int** sequence, int i, int j);
void get_neighbour(box** boxes_neighbour, int n_boxes, int** current_a, int** current_b, int** current_c);
int give_X_ij(int* seq, int len, int i, int j);
int enforce_vertical_stability(int x_i, int y_i, int z_i, int xlen, int ylen, int zlen, int* P_y, int P_y_len, box* boxes, int n_boxes);
void place_boxes_sequence_triples(int* a, int* b, int* c, box** boxes, int n_boxes);
void sa_make_a_step(box** boxes, int n_boxes, float* temperature, float alpha, float beta, 
                    int** current_a, int** current_b, int** current_c, 
                    int** best_a, int** best_b, int** best_c, int* best_volume, box** boxes_neighbour,
                    int cont_x, int cont_y, int cont_z);
box* simulated_annealing_knapsack_3D(int* a, int* b, int* c, box* boxes_input, int n_boxes, 
                                    int md, int secs,int cont_x, int cont_y, int cont_z, char** boxes_names);
int volume_occupied(box* boxes, int n_boxes, int cont_x, int cont_y, int cont_z, int mode);
