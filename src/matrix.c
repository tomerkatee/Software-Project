#include "spkmeans.h"


/* matrix operations */

Matrix wam(){

}

Matrix ddg(){

}

Matrix gl(){


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


