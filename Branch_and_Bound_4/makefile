compile_knapsack_1: main.c utils.c extreme_points.c boxes.c knapsack_0_1_solver.c
	gcc -g main.c utils.c extreme_points.c boxes.c knapsack_0_1_solver.c -lm -o main.out

compile_prova: prova.c utils.c extreme_points.c knapsack_0_1_solver_2.c
	gcc -g prova.c utils.c extreme_points.c knapsack_0_1_solver_2.c -lm -o prova.out

compile_prova_2: prova2.c utils.c boxes.c extreme_points.c knapsack_0_1_solver_2.c
	gcc -g prova2.c utils.c boxes.c extreme_points.c knapsack_0_1_solver_2.c -lm -o prova2.out

compile_knapsack_2: main2.c utils.c extreme_points.c boxes.c knapsack_0_1_solver.c advanced_extreme_points.c advanced_extreme_points_plus.c investigate_points_with_boxes_around.c
	gcc -g main2.c utils.c extreme_points.c boxes.c knapsack_0_1_solver.c advanced_extreme_points.c advanced_extreme_points_plus.c investigate_points_with_boxes_around.c -lm -o main2.out