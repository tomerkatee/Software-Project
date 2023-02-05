#ifndef SPKMEANS_H
#define SPKMEANS_H

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* globals */

/* typedefs and structs */

typedef double* Datapoint;
typedef Datapoint* Matrix;

typedef struct{
    Datapoint eigenvalues;
    Matrix eigenvectors;
} Diagonalization;

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
 * @param matrix a real symmetric full rank matrix
 * @return all the eigenvectors and eigenvalues of the matrix
*/
Diagonalization* jacobi(Matrix matrix);

#endif