FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: spkmeans.o clustering.o jacobi.o spkmeans.h
	gcc -o spkmeans spkmeans.o jacobi.o

spkmeans.o: spkmeans.c
	gcc -c $(FLAGS) spkmeans.c

clustering.o: clustering.c
	gcc -c $(FLAGS) clustering.c

jacobi.o: jacobi.c
	gcc -c $(FLAGS) jacobi.c