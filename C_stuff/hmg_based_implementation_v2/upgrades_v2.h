



int is_box_outside_container(box actual_box, int cont_x, int cont_y, int cont_z, int mode);
int get_index_of_box_inside_container(box* boxes, int n_boxes, int* b_chain, 
            int cont_x, int cont_y, int cont_z);
void get_indexes_of_boxes_to_swap(int* x, int* y, box* boxes, int n_boxes, int* b_chain, 
            int cont_x, int cont_y, int cont_z);
int random_element_of_array(int* array, int n);



