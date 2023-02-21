FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o clustering.o matrix.o jacobi.o spkmeans.h
	gcc -o spkmeans spkmeans.o matrix.o jacobi.o

spkmeans.o: spkmeans.c
	gcc -c $(FLAGS) spkmeans.c

clustering.o: clustering.c
	gcc -c $(FLAGS) clustering.c

matrix.o: matrix.c
	gcc -c $(FLAGS) matrix.c

jacobi.o: jacobi.c
	gcc -c $(FLAGS) jacobi.c