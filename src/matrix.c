#include "spkmeans.h"

double sq_distance(Datapoint dp1, Datapoint dp2)
{
    int i;
    double sum = 0;
    for (int i = 0; i < K; i++)
    {
        sum += pow(dp1[i] - dp2[i], 2);
    }
    return sum;
    
}

/* matrix operations */



Matrix wam_ddg_gl(Datapoint datapoints[], int ddg_gl)
{
    Matrix W;
    W = ddg_gl ? wam(datapoints) : squareMatrix();
    int i, j, diag;
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
    Datapoint row;
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
            printf("%.4lf,", A[i][j]);
        }
        printf("%.4lf\n", A[i][j]);
    }
    
}


