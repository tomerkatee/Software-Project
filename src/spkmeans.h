#ifndef SPKMEANS_H
#define SPKMEANS_H

#ifndef _GNU_SOURCE
#define  _GNU_SOURCE
#endif

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define true 1
#define false 0

#define KMEANS_ITER 300
#define KMEANS_EPS 0
#define ITER 100
#define EPS 0.00001

/* globals */
extern int N;          /* number of datapoints */
extern int K;          /* number of clusters */
extern int dp_size;    /* number of elements in each datapoint */
/*  extern double eps;  */    /* error of convergence */
/*  extern int iter;  */   /* number of iterations for convergence */
/* actual declaration in matrix.c */

/* typedefs and structs */

typedef double* Datapoint;
typedef Datapoint* Matrix;

typedef struct dp_node{
    Datapoint value;
    struct dp_node* next;
    struct dp_node* prev;
    int cluster;
} DPNode;

typedef struct{
    DPNode* datapoints;
    Datapoint centroid;
    Datapoint prev;
    int size;
} Cluster;

typedef struct{
    Datapoint eigenvalues;
    Matrix eigenvectors;
} Diagonalization;


/* function prototypes */

/**
 * The kmeans-clustering algorithm
 * @param datapoints an array of datapoint pointers
 * @param initial_centroids array with the indexes of the initial centroids in datapoints
 * @return list of clusters
*/
Cluster** kmeans_clustering(DPNode *datapoints[], long initial_centroids[]);

/**
 * Get the Weighted Adjacency Matrix for a set of datapoints
 * @param datapoints a list of datapoints
 * @return the weighted adjacency matrix
*/
Matrix wam(Datapoint datapoints[]);

/**
 * Get the Diagonal Degree Matrix for a set of datapoints
 * @param datapoints a list of datapoints
 * @return the diagonal degree matrix
*/
Matrix ddg(Datapoint datapoints[]);

/**
 * Get the Graph Laplacian for a set of datapoints
 * @param datapoints a list of datapoints
 * @return the graph laplacian
*/
Matrix gl(Datapoint datapoints[]);

/**
 * Get the eigenvectors and eigenvalues of a matrix
 * @param A a real symmetric full rank matrix
 * @return all the eigenvectors and eigenvalues of the matrix
*/
Diagonalization jacobi(Matrix A);

/**
 * Initialize a square matrix
 * @return an allocated square matrix of size n
*/
Matrix squareMatrix();

/**
 * frees the memory allocated for a matrix
 * @param M a matrix to free
*/
void free_matrix(Matrix M);

Matrix wam_ddg_gl(Datapoint datapoints[], int ddg_gl);

/**
 * Get the squared euclidian distance of two datapoints
 * @param dp1 a datapoint
 * @param dp2 a datapoint
 * @return the squared euclidian distance
*/
double sq_distance(Datapoint dp1, Datapoint dp2);

/**
 * Get the euclidian distance of two datapoints
 * @param dp1 a datapoint
 * @param dp2 a datapoint
 * @return the euclidian distance
*/
double euclidian_distance(Datapoint dp1, Datapoint dp2);

/**
 * Matrix multiplication
 * @param A a matrix
 * @param B a matrix
 * @return A * B
*/
Matrix mat_multiplication(Matrix A, Matrix B);

/**
 * Prints a matrix's rows. The values of each row are separated by commas
 * @param A a matrix
*/
void print_matrix(Matrix A);

/**
 * Prints a datapoint's values separated by commas
 * @param dp a datapoint
 * @param size the datapoint's size
*/
void print_datapoint(Datapoint dp, int size);

/**
 * Reads the datapoints from a file into a Datapoint array
 * @param file_name .txt input file name that contains the datapoints
 * @return array of datapoints from the lines in the file
*/
Datapoint* read_datapoints(char* file_name);

/**
 * Allocates memory with calloc() and terminates the program if unsuccessful
 * @param nitems number of blocks to allocate
 * @param size size of each block
*/
void *calloc_and_check(size_t nitems, size_t size);

#endif
