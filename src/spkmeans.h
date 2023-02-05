#ifndef SPKMEANS_H
#define SPKMEANS_H

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* globals */
int n; // number of datapoints

/* typedefs and structs */

typedef double* Datapoint;
typedef Datapoint* Matrix;

typedef struct dp_l{
    Datapoint value;
    struct dp_l* next;
    struct dp_l* prev;
} DPNode;

typedef struct{
    DPNode* datapoints;
    int size;
    Datapoint centroid;
    Datapoint prev;
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
Cluster** kmeans_clustering(Datapoint datapoints[], long initial_centroids[]);

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
Diagonalization* jacobi(Matrix A);

/**
 * Initialize a square matrix
 * @return an allocated square matrix of size n
*/
Matrix squareMatrix();

/**
 * Get the squared euclidian distance of two datapoints
 * @param dp1 a datapoint
 * @param dp2 a datapoint
 * @return the squared euclidian distance
*/
double distance(Datapoint dp1, Datapoint dp2);

/**
 * Get the degree of a node in a graph
 * @param A an adjacency matrix
 * @param i index of a node
 * @return the degree of i in A
*/
double degree(Matrix A, int i);

/**
 * Builds a rotation matrix
 * @param c cosine of rotation angle
 * @param s sine of rotation angle
*/
Matrix rotationMatrix(int i, int j, double c, double s);

/**
 * Matrix multiplication
 * @param A a matrix
 * @param B a matrix
 * @return A * B
*/
Matrix matMultiplication(Matrix A, Matrix B);

/**
 * @param A a matrix
 * @return sum of squares of all off-diagonal elements of A
*/
double off(Matrix A);

#endif