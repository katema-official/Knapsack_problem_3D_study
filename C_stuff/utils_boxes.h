
typedef struct {
	char* name;
	int xlen;
    int ylen;
    int zlen;
	int x0;
    int y0;
    int z0;
}box;

int* random_permutation_1_to_n(int n);
int comparator_boxes_volume(const void* a, const void* b);
