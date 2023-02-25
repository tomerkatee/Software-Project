FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o clustering.o jacobi.o matrix.o
	gcc -g spkmeans.o clustering.o jacobi.o matrix.o -lm -o spkmeans

spkmeans.o: spkmeans.c spkmeans.h
	gcc -g -c $(FLAGS) spkmeans.c

clustering.o: clustering.c spkmeans.h
	gcc -g -c $(FLAGS) clustering.c

matrix.o: matrix.c spkmeans.h 
	gcc -g -c $(FLAGS) matrix.c

jacobi.o: jacobi.c spkmeans.h
	gcc -g -c $(FLAGS) jacobi.c

