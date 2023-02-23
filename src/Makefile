FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o clustering.o matrix.o jacobi.o
	gcc -o spkmeans spkmeans.o clustering.o matrix.o jacobi.o -lm

spkmeans.o: spkmeans.c spkmeans.h
	gcc -c $(FLAGS) spkmeans.c

clustering.o: clustering.c spkmeans.h
	gcc -c $(FLAGS) clustering.c

matrix.o: matrix.c spkmeans.h 
	gcc -c $(FLAGS) matrix.c

jacobi.o: jacobi.c spkmeans.h
	gcc -c $(FLAGS) jacobi.c