#ifndef SYMNMF_H
#define SYMNMF_H


/*
General error printing function
*/
void print_error(void);


/*
General n x n memory freeing function.
*/
void free_matrix(double **mat, int rows);


/*
Function: Calculates the similarity matrix A.
Input: Includes a 2D array of doubles representing n data points (X),
the number of data points (n)
and the dimension of each data point (d).
Output: A 2D array of doubles representing the similarity matrix A.
if a cell is in the diagonal - automatically set to 0.0
*/
double** get_similarity_matrix(double** X, int n, int d);


/*
Function: Calculates the degree vector (diagonal of D).
Input: Similarity matrix A and its size n.
Output: Returns a 1D array for memory efficiency,
as all non-diagonal cells are set to 0.
*/
double* get_degree_vector(double** A, int n);


/*
Function: Calculates the normalized similarity matrix W.
Input: Similarity matrix A, degree vector D and their size n.
Output: A 2D array of doubles representing the normalized similarity matrix W.
-Each cell can be calculated as W_ij = A_ij / sqrt(D_i * D_j).
-We check for division by 0 specifically.
*/
double** get_normalized_similarity_matrix(double** A, double* D, int n);


/*
Function: Optimizes H matrix until convergence .
Input: Two Matrices (initialized H and normalized similarity matrix W) and their dimensions n,k.
Output: Martix H with optimized values.
*/
double** optimize_h(double** W, double** H, int n, int k);

#endif