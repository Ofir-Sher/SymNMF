#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "symnmf.h"
#define DENOM_EPS 1e-12

/*
Function: Calculates the squared Euclidean distance between two vectors.
Input: Includes two vectors of doubles and the dimention of said vectors.
Output: A double representing the squared distance between them.
*/
double squared_euclidean_distance(double* vec1, double* vec2, int d) {
    double sum;
    double diff;
    int i;

    sum = 0.0;

    for (i = 0; i < d; i++) {
        diff = vec1[i] - vec2[i];
        sum += diff * diff;
    }
    return sum;
}


/*
Function: Calculates the similarity matrix A.
Input: Includes a 2D array of doubles representing n data points (X),
the number of data points (n)
and the dimension of each data point (d).
Output: A 2D array of doubles representing the similarity matrix A.
if a cell is in the diagonal - automatically set to 0.0
*/
double** get_similarity_matrix(double** X, int n, int d) {
    int i, j;
    double dist;
    double** A;
    
    A = (double**)malloc(n * sizeof(double*));
    
    for (i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        for (j = 0; j < n; j++) {
            if (i == j) {
                A[i][j] = 0.0;
            } else {
                dist = squared_euclidean_distance(X[i], X[j], d);
                A[i][j] = exp(-dist / 2.0);
            }
        }
    }
    return A;
}


/*
Function: Calculates the degree vector (diagonal of D).
Input: Similarity matrix A and its size n.
Output: Returns a 1D array for memory efficiency,
as all non-diagonal cells are set to 0.
*/
double* get_degree_vector(double** A, int n) {
    int i, j;
    double* D;
    
    D = (double*)malloc(n * sizeof(double));
    
    for (i = 0; i < n; i++) {
        D[i] = 0.0;
        for (j = 0; j < n; j++) {
            D[i] += A[i][j];
        }
    }
    return D;
}


/*
Function: Calculates the normalized similarity matrix W.
Input: Similarity matrix A, degree vector D and their size n.
Output: A 2D array of doubles representing the normalized similarity matrix W.
-Each cell can be calculated as W_ij = A_ij / sqrt(D_i * D_j).
-We check for division by 0 specifically.
*/
double** get_normalized_similarity_matrix(double** A, double* D, int n) {
    int i, j;
    double** W;
    
    W = (double**)malloc(n * sizeof(double*));
    
    for (i = 0; i < n; i++) {
        W[i] = (double*)malloc(n * sizeof(double));
        for (j = 0; j < n; j++) {
            if (D[i] == 0 || D[j] == 0) {
                W[i][j] = 0.0;
            } else {
                W[i][j] = A[i][j] / sqrt(D[i] * D[j]);
            }
        }
    }
    return W;
}


/*
Function: Prints a 2D matrix following the project requirements.
*/
void print_matrix(double** matrix, int rows, int cols) {
    int i, j;
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%.4f", matrix[i][j]);
            if (j < cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}


/*
Function: Prints the 1D degree vector as a full N x N diagonal matrix.
-We expand the 1D vector to a matrix, using our assumption of 0's outside the diagonal.
*/
void print_diagonal_matrix(double* D, int n) {
    int i, j;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                printf("%.4f", D[i]);
            } else {
                printf("0.0000");
            }
            if (j < n - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}


/*
General error printing function
 */
void print_error() {
    printf("An Error Has Occurred\n");
    exit(1);
}


/*
General n x n memory freeing function.
 */
void free_matrix(double **mat, int n) {
    int i;
    if (mat == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}


/*
Function: Reads data from file, updating number of data points (n) and dimension (d).
Input: User inputed text file.
Output: Matrix X of size n x d containing the data points.
*/
double** read_points(const char* file_name, int* n, int* d) {
    FILE* fp;
    double** X;
    double val;
    char c;
    int i, j, ret;
    
    *n = 0; *d = 0;
    fp = fopen(file_name, "r");
    if (fp == NULL) print_error();
    
    while ((ret = fscanf(fp, "%lf%c", &val, &c)) >= 1) {
        if (*n == 0) (*d)++;
        if (c == '\n' || ret == 1 || c == '\r') (*n)++;
    }
    
    rewind(fp);
    X = (double**)malloc((*n) * sizeof(double*));
    if (X == NULL) print_error();
    
    i = 0; j = 0;
    while ((ret = fscanf(fp, "%lf%c", &val, &c)) >= 1) {
        if (j == 0) {
            X[i] = (double*)malloc((*d) * sizeof(double));
            if (X[i] == NULL) print_error();
        }
        X[i][j] = val;
        j++;
        if (c == '\n' || ret == 1 || c == '\r') {
            i++;
            j = 0;
        }
    }
    
    fclose(fp);
    return X;
}


/*
Main funtion for this file, operates based on user input and frees memory at the end.
*/
int main(int argc, char *argv[]) {
    char *goal; char *file_name;
    int n, d;
    double **X, **A, **W; double *D;

    if (argc != 3) print_error();

    goal = argv[1];
    file_name = argv[2];

    X = read_points(file_name, &n, &d);
    A = get_similarity_matrix(X, n, d);

    if (strcmp(goal, "sym") == 0) {
        print_matrix(A, n, n);
    } else if (strcmp(goal, "ddg") == 0) {
        D = get_degree_vector(A, n);
        print_diagonal_matrix(D, n);
        free(D);
    } else if (strcmp(goal, "norm") == 0) {
        D = get_degree_vector(A, n);
        W = get_normalized_similarity_matrix(A, D, n);
        print_matrix(W, n, n);
        free(D);
        free_matrix(W, n);
    } else {
        print_error();
    }

    free_matrix(X, n);
    free_matrix(A, n);
    return 0;
}


/*
Function: Multiply two matrices by row and columns.
Input: 2 Matrices and their row/column size.
Output: new matrix C with a size of rowsA x colsB.
*/
double** matmul(double** A, double** B, int rowsA, int colsA, int colsB) {
    double** C; int i, j, l;
    C = (double**)malloc(rowsA * sizeof(double*));
    if (C == NULL) print_error();
    for (i = 0; i < rowsA; i++) {
        C[i] = (double*)malloc(colsB * sizeof(double));
        if (C[i] == NULL) print_error();
        for (j = 0; j < colsB; j++) {
            C[i][j] = 0.0;
            for (l = 0; l < colsA; l++){
                C[i][j] += A[i][l] * B[l][j];
            } 
        }
    }
    return C;
}


/*
Function: Transposes a matrix.
Input: A matrix and its dimensions
Output: new transposed matrix T with a size of cols x rows.
*/
double** transpose_matrix(double** mat, int rows, int cols) {
    double** T; int i, j;
    T = (double**)malloc(cols * sizeof(double*));
    if (T == NULL) print_error();
    for (i = 0; i < cols; i++) {
        T[i] = (double*)malloc(rows * sizeof(double));
        if (T[i] == NULL) print_error();
        for (j = 0; j < rows; j++) {
            T[i][j] = mat[j][i];
        } 
    }
    return T;
}


/*
Function: Calculates the squared Frobenius norm of (A - B).
Input: Two Matrices and the dimensions
Output: A double representing the squared Frobenius norm.
*/
double get_frobenius_distance(double** A, double** B, int rows, int cols) {
    double sum = 0.0; double diff; int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            diff = A[i][j] - B[i][j];
            sum += diff * diff;
        }
    }
    return sum;
}


/*
Function: Optimizes H matrix until convergence .
Input: Two Matrices (initialized H and normalized similarity matrix W) and their dimensions n,k.
Output: Martix H with optimized values.
*/
double** optimize_h(double** W, double** H, int n, int k) {
    int iter = 0, i, j; double beta = 0.5, dist = 1.0;
    double **Ht, **HtH, **HHtH, **WH, **H_next;
    
    while (iter < 300 && dist >= 1e-4) {
        Ht = transpose_matrix(H, n, k);
        HtH = matmul(Ht, H, k, n, k);
        HHtH = matmul(H, HtH, n, k, k);
        WH = matmul(W, H, n, n, k);
        
        H_next = (double**)malloc(n * sizeof(double*));
        if (H_next == NULL) print_error();
        for (i = 0; i < n; i++) {
            H_next[i] = (double*)malloc(k * sizeof(double));
            if (H_next[i] == NULL) print_error();
            for (j = 0; j < k; j++) {
                H_next[i][j] = H[i][j] * (1.0 - beta + beta * (WH[i][j] / (HHtH[i][j] + DENOM_EPS)));
            }
        }
        
        dist = get_frobenius_distance(H_next, H, n, k);
        
        free_matrix(Ht, k); free_matrix(HtH, k); 
        free_matrix(HHtH, n); free_matrix(WH, n); free_matrix(H, n);
        
        H = H_next;
        iter++;
    }
    return H;
}