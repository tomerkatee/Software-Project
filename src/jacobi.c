#include "spkmeans.h"


/* jacobi diagonalization algorithm */

int piv_i, piv_j; /* index of the current pivot */

Matrix rotationMatrix(int i, int j, double c, double s)
{
    int a, b;
    Matrix mat = squareMatrix();
    for(a = 0; a < N; a++)
    {
        for(b = 0; b < N; b++)
        {
            mat[a][b] = a == b ? 1 : 0;
        }
    }
    mat[i][i] = c;
    mat[j][j] = c;
    mat[i][j] = s;
    mat[j][i] = -s;
    return mat;
}

double off_squared(Matrix A)
{
    int i, j;
    double sum = 0;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            sum += i == j ? 0 : pow(A[i][j], 2);
        }
    }
    return sum;
}

void pivot(Matrix A)
{
    double max_el = 0, cur_el;
    int i, j;
    piv_i = 0;
    piv_j = 0;
    
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            cur_el = i == j ? 0 : fabs(A[i][j]);
            if(cur_el > max_el)
            {
                max_el = cur_el;
                piv_i = i;
                piv_j = j;
            }
        }
    }
}

double obtain_theta(Matrix A)
{
    pivot(A);
    return (A[piv_j][piv_j] - A[piv_i][piv_i]) / (2 * A[piv_i][piv_j]);
}

double sign(double theta)
{
    return theta < 0 ? -1 : 1;
}

double root_operation(double theta)
{
    return sqrt(pow(theta, 2) + 1);
}

double obtain_t(Matrix A)
{
    double theta = obtain_theta(A);
    return sign(theta) / (fabs(theta) + root_operation(theta));
}

Datapoint diagonal(Matrix A)
{
    int i;
    Datapoint diag = (Datapoint)calloc_and_check(N, sizeof(double));
    for(i = 0; i < N; i++)
    {
        diag[i] = A[i][i];
    }
    return diag;
}

Matrix transform(Matrix A, Diagonalization* diag)
{
    int r, k;
    double t, c, s;
    Matrix A_tag = squareMatrix();

    t = obtain_t(A);
    c = 1 / root_operation(t);
    s = t * c;
    diag->eigenvectors = matMultiplication(diag->eigenvectors, rotationMatrix(piv_i, piv_j, c, s));

    for(r = 0; r < N; r++)
    {
        for(k = 0; k < N; k++)
        {
            A_tag[r][k] = k == piv_i ? c * A[r][piv_i] - s * A[r][piv_j] : (k == piv_j ? c * A[r][piv_j] + s * A[r][piv_i] : A[r][k]);
        }
    }

    A_tag[piv_i][piv_i] = c * c * A[piv_i][piv_i] + s * s * A[piv_j][piv_j] - 2 * s * c * A[piv_i][piv_j];
    A_tag[piv_j][piv_j] = s * s * A[piv_i][piv_i] + c * c * A[piv_j][piv_j] + 2 * s * c * A[piv_i][piv_j];
    A_tag[piv_i][piv_j] = 0;
    
    return A_tag;
}

Diagonalization jacobi(Matrix A)
{
    double prev_off, cur_off = 0;
    int iter_count = 0;
    Diagonalization* diag = (Diagonalization*)calloc_and_check(1, sizeof(Diagonalization));
    diag->eigenvectors = rotationMatrix(0, 1, 1, 0);

    while (true)
    {
        printf("\n");
        print_matrix(A);
        printf("\n");
        prev_off = cur_off;
        A = transform(A, diag);
        cur_off = off_squared(A);
        if((iter_count++ >= ITER) || (fabs(prev_off - cur_off) <= EPS)){ break; }
    }

    diag->eigenvalues = diagonal(A);
    free_matrix(A);
    return *diag;
}