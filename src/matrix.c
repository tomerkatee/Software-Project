#include "spkmeans.h"

 int N, K, dp_size, iter;
 double eps;

/* matrix operations */

void *calloc_and_check(size_t nitems, size_t size)
{
    void *p = calloc(nitems, size);
    if(p == NULL)
    {
        printf("An Error Has Occurred!\n");
        exit(1);
    }
    return p;
}

double sq_distance(Datapoint dp1, Datapoint dp2)
{
    int i;
    double sum = 0;
    for (i = 0; i < K; i++)
    {
        sum += pow(dp1[i] - dp2[i], 2);
    }
    return sum;
    
}

double euclidian_distance(Datapoint dp1, Datapoint dp2)
{
    return sqrt(sq_distance(dp1, dp2));
}

Matrix wam_ddg_gl(Datapoint datapoints[], int ddg_gl)
{
    Matrix W;
    int i, j, diag;

    W = ddg_gl ? wam(datapoints) : squareMatrix();
    for (i = 0; i < N; i++)
    {
        diag = ddg_gl ? degree(W, i) : 0;
        for (j = 0; j < N; j++)
        {
            if (i == j)
                W[i][j] = diag;
            else
                W[i][j] = ddg_gl ? (ddg_gl==1 ? 0 : -W[i][j]) : exp(-(sq_distance(datapoints[i], datapoints[j]) / 2));
        }
    }
    return W;
}

Matrix wam(Datapoint datapoints[])
{
    return wam_ddg_gl(datapoints, 0);
}

Matrix ddg(Datapoint datapoints[])
{
    return wam_ddg_gl(datapoints, 1);
}

Matrix gl(Datapoint datapoints[])
{
    return wam_ddg_gl(datapoints, 2);
}

Matrix squareMatrix(){
    int i;
    Matrix m;
    m = (Matrix)calloc_and_check(N, sizeof(Datapoint));
    for (i = 0; i < N; i++)
    {
        m[i] = (Datapoint)calloc_and_check(N, sizeof(double));
    }
    return m;
    
}

double degree(Matrix W, int i){
    double sum = 0;
    int j;
    for (j = 0; j < N; j++)
    {
        sum += W[i][j];
    }
    return sum;
}

Matrix matMultiplication(Matrix A, Matrix B)
{
    Matrix C;
    int i, j, k; 
    C = squareMatrix();
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
            
        }
    }
    return C;
    
}

void print_matrix(Matrix A)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for(j = 0; j < N - 1; j++)
        {
            printf("%.4f,", A[i][j]);
        }
        printf("%.4f\n", A[i][j]);
    }
    
}