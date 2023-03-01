FLAGS = -g -c -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o clustering.o jacobi.o matrix.o
	gcc spkmeans.o clustering.o jacobi.o matrix.o -lm -o spkmeans

spkmeans.o: spkmeans.c spkmeans.h
	gcc $(FLAGS) spkmeans.c

clustering.o: clustering.c spkmeans.h
	gcc $(FLAGS) clustering.c

matrix.o: matrix.c spkmeans.h 
	gcc $(FLAGS) matrix.c

jacobi.o: jacobi.c spkmeans.h
	gcc $(FLAGS) jacobi.c
