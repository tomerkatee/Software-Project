FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o jacobi.o spkmeans.h
	gcc -o spkmeans spkmeans.o jacobi.o

spkmeans.o: spkmeans.c
	gcc -c $(FLAGS) spkmeans.c

jacobi.o: jacobi.c
	gcc -c $(FLAGS) jacobi.c