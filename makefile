all: main 

	gcc -Wall -fno-tree-vectorize -fopenmp main.c -o kolorowanie_wierzcholkow


