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

typedef struct{
    int x;
    int y;
    int z;
    int width;
    int height;
    int depth;
}point;

typedef struct node{
    point* extreme_points;
    int ep_len;
    box* boxes_to_place;
    int btp_len;
    box* boxes_placed;
    int bp_len;
    struct node* succ;
}node;



#endif