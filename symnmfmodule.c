#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

/*
Function: Transforms Python List of Lists to a C Matrix.
Input: A Python List of Lists.
Output: A C matrix i.e double**.
-We make sure to transform python float to C double per the requirements. 
*/
double** py_list_to_c_matrix(PyObject* py_matrix, int rows, int cols) {
    int i, j;
    double** c_matrix;
    PyObject *row, *item;
    
    c_matrix = (double**)malloc(rows * sizeof(double*));
    if (c_matrix == NULL) return NULL;
    
    for (i = 0; i < rows; i++) {
        c_matrix[i] = (double*)malloc(cols * sizeof(double));
        if (c_matrix[i] == NULL) return NULL;
        
        row = PyList_GetItem(py_matrix, i);
        for (j = 0; j < cols; j++) {
            item = PyList_GetItem(row, j);
            c_matrix[i][j] = PyFloat_AsDouble(item);
        }
    }
    return c_matrix;
}


/*
Function: Transforms C Matrix to a Python List of Lists.
Input: A C matrix i.e double**.
Output: A Python List of Lists.
-We make sure to transform C double to python float per the requirements. 
*/
PyObject* c_matrix_to_py_list(double** c_matrix, int rows, int cols) {
    int i, j;
    PyObject *py_matrix, *row, *item;
    
    py_matrix = PyList_New(rows);
    for (i = 0; i < rows; i++) {
        row = PyList_New(cols);
        for (j = 0; j < cols; j++) {
            item = PyFloat_FromDouble(c_matrix[i][j]);
            PyList_SetItem(row, j, item);
        }
        PyList_SetItem(py_matrix, i, row);
    }
    return py_matrix;
}


/*
Function: A Python to C and back wrapper for the similarity matrix.
*/
static PyObject* sym_wrapper(PyObject* self, PyObject* args) {
    PyObject* py_X;
    int n, d;
    double **X, **A;
    PyObject* py_A;
    (void)self;
    
    if (!PyArg_ParseTuple(args, "O", &py_X)) {
        return NULL;
    }
    n = (int)PyList_Size(py_X);
    d = (int)PyList_Size(PyList_GetItem(py_X, 0));
    
    X = py_list_to_c_matrix(py_X, n, d);
    if (X == NULL) return NULL;
    
    A = get_similarity_matrix(X, n, d);
    py_A = c_matrix_to_py_list(A, n, n);
    
    free_matrix(X, n); free_matrix(A, n);
    
    return py_A;
}


/*
Function: A Python to C and back wrapper for the diaginal degree matrix.
-Once again, for efficiency we manually put 0.0 in each cell that is not in the diagonal.
*/
static PyObject* ddg_wrapper(PyObject* self, PyObject* args) {
    PyObject* py_X;
    int n, d, i, j;
    double **X, **A, *D;
    PyObject *py_D, *row, *item;
    (void)self;
    
    if (!PyArg_ParseTuple(args, "O", &py_X)) return NULL;
    n = (int)PyList_Size(py_X);
    d = (int)PyList_Size(PyList_GetItem(py_X, 0));

    X = py_list_to_c_matrix(py_X, n, d);
    if (X == NULL) return NULL;
    
    A = get_similarity_matrix(X, n, d);
    D = get_degree_vector(A, n);
    
    py_D = PyList_New(n);
    for (i = 0; i < n; i++) {
        row = PyList_New(n);
        for (j = 0; j < n; j++) {
            item = PyFloat_FromDouble((i == j) ? D[i] : 0.0);
            PyList_SetItem(row, j, item);
        }
        PyList_SetItem(py_D, i, row);
    }
    
    free_matrix(X, n); free_matrix(A, n); free(D);
    return py_D;
}


/*
Function: A Python to C and back wrapper for the normalized similarity matrix.
*/
static PyObject* norm_wrapper(PyObject* self, PyObject* args) {
    PyObject* py_X;
    int n, d;
    double **X, **A, **W, *D;
    PyObject* py_W;
    (void)self;
    
    if (!PyArg_ParseTuple(args, "O", &py_X)) return NULL;
    n = (int)PyList_Size(py_X);
    d = (int)PyList_Size(PyList_GetItem(py_X, 0));

    X = py_list_to_c_matrix(py_X, n, d);
    if (X == NULL) return NULL;
    
    A = get_similarity_matrix(X, n, d);
    D = get_degree_vector(A, n);
    W = get_normalized_similarity_matrix(A, D, n);
    py_W = c_matrix_to_py_list(W, n, n);
    
    free_matrix(X, n); free_matrix(A, n); free(D); free_matrix(W, n);
    return py_W;
}


/*
Function: A Python to C and back wrapper for the SymNMF matrix.
*/
static PyObject* symnmf_wrapper(PyObject* self, PyObject* args) {
    PyObject *py_W, *py_H;
    int n, k;
    double **W, **H, **final_H;
    PyObject* py_final_H;
    (void)self;
    
    if (!PyArg_ParseTuple(args, "OOii", &py_W, &py_H, &n, &k)) return NULL;
    
    W = py_list_to_c_matrix(py_W, n, n);
    H = py_list_to_c_matrix(py_H, n, k);
    if (W == NULL || H == NULL) return NULL;
    
    final_H = optimize_h(W, H, n, k);
    py_final_H = c_matrix_to_py_list(final_H, n, k);
    
    free_matrix(W, n);
    free_matrix(final_H, n); 
    
    return py_final_H;
}


/*
Default Python module definition
*/
static PyMethodDef symnmfMethods[] = {
    {"sym", (PyCFunction)sym_wrapper, METH_VARARGS, NULL},
    {"ddg", (PyCFunction)ddg_wrapper, METH_VARARGS, NULL},
    {"norm", (PyCFunction)norm_wrapper, METH_VARARGS, NULL},
    {"symnmf", (PyCFunction)symnmf_wrapper, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule",
    NULL,
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_symnmfmodule(void) {
    return PyModule_Create(&symnmfmodule);
}