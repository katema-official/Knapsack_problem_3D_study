#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "sa_performance_stuff.h"


int generation_counter;
int solutions_for_this_generation;

void clear_generation_performance_file(){
    FILE* f = fopen("performance_generations_big.txt", "w");
    fclose(f);
}

void initialize(){
    generation_counter = 1;
    solutions_for_this_generation = 1;
}

void print_generation_performance(int changed){
    FILE* f = fopen("performance_generations_big.txt", "a");
    if(changed){
        //a new solution has been accepted
        fprintf(f, "%d° generation - %d solutions\n", generation_counter, solutions_for_this_generation);
        generation_counter++;
        solutions_for_this_generation = 1;
    }else{
        solutions_for_this_generation++;
    }
    fclose(f);
}




