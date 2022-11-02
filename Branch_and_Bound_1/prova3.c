#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "knapsack_0_1_solver.h"
int* volumes;
int n_volumes;

int main(){
    char** ciao = malloc(3*sizeof(char*));
    //ciao[0] = malloc(5*sizeof(char));
    ciao[0] = "ciao";
    //ciao[1] = malloc(5*sizeof(char));
    ciao[1] = "come";
    //ciao[2] = malloc(5*sizeof(char));
    ciao[2] = "stai";

    for(int i = 0; i < 3; i++){
        printf("%s ", ciao[i]);
    }

    char** ciao2 = malloc(3*sizeof(char*));
    for(int i = 0; i < 3; i++){
        int x = strlen((ciao[(i + 1) % 3]))+1;
        printf("len x = %d\n", x);
        ciao2[i] = malloc(x * sizeof(char));
    }
    strcpy(ciao2[0], ciao[1]);
    strcpy(ciao2[1], ciao[2]);
    strcpy(ciao2[2], ciao[0]);
    free(ciao);
    ciao = ciao2;
    for(int i = 0; i < 3; i++){
        printf("%s ", ciao[i]);
        free(ciao[i]);
    }
    free(ciao);

    //reset_progress_file();


    FILE* f = fopen("./results/primal_bound_start.txt", "r");
    char* line = NULL;
    size_t len = 0;
    size_t read;
    char delim[] = " ";
    int box_index = 0;

    read = getline(&line, &len, f);
    printf("%d\n", atoi(line));

    while ((read = getline(&line, &len, f)) != -1) {
        //protocol of a line: xlen ylen zlen x0 y0 z0 name\n
        char** info_of_a_box = (char**) malloc(7*sizeof(char*));
        line[read-1] = delim[0];
        line[read-2] = delim[0];
        char* token = strtok(line, delim);
        int i = 0;
        while(token != NULL){
            int len = strlen(token);
            info_of_a_box[i] = token;
            i++;
            token = strtok(NULL, delim);
        }
        box b;
        b.xlen = atoi(info_of_a_box[0]);
        b.ylen = atoi(info_of_a_box[1]);
        b.zlen = atoi(info_of_a_box[2]);
        //b.name = (char*) malloc((strlen(info_of_a_box[3])+1)*sizeof(char));
        //strcpy(b.name, info_of_a_box[3]);
        //boxes_names[box_index] = (char*) malloc((strlen(info_of_a_box[3])+1)*sizeof(char));
        //strcpy(boxes_names[box_index], info_of_a_box[3]);
        b.x0 = atoi(info_of_a_box[3]);
        b.y0 = atoi(info_of_a_box[4]);
        b.z0 = atoi(info_of_a_box[5]);
        //boxes[box_index] = b;
        box_index++;
        free(info_of_a_box);
    }
    fclose(f);
    if(line){free(line);}


    int capacity = 10000;
    int volumes[10] = {1100, 1300, 1500, 1501, 1670, 1890, 2100, 2343, 2478, 2891};
    printf("%d\n", solve_knapsack_0_1(volumes, 10, capacity));
    printf("%d\n", solve_knapsack_0_1_v2(volumes, 10, capacity));
    printf("%d\n", solve_knapsack_0_1_v3(volumes, 10, capacity));
    printf("%d\n", solve_knapsack_0_1_v4(volumes, 10, capacity));
    printf("%d\n", solve_knapsack_0_1_v5(volumes, 10, capacity));
}