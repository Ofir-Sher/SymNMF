# Symmetric Non-Negative Matrix Factorization (SymNMF)

A high-performance hybrid C and Python implementation of a clustering algorithm based on Symmetric Non-Negative Matrix Factorization (SymNMF) for graph clustering. This project was developed as part of the Software Project course at Tel Aviv University.

The project leverages C for computationally heavy matrix operations and Python for the user interface and analytical comparisons, bridged together using the Python C API.

## 🚀 Features

- **Algorithmic Graph Clustering:** Groups data points by calculating similarities and finding a lower-dimensional non-negative matrix $H$ that minimizes the squared Frobenius norm $||W - HH^T||_F^2$.
- **Hybrid Architecture:** 
  * Core matrix transformations (Similarity, Degree, and Normalized Similarity matrices) and the $H$-matrix optimization loop are implemented in pure C for maximum performance.
  * A Python wrapper (`symnmfmodule.c`) exposes the C library to Python.
- **Comparative Analysis:** Includes a benchmarking tool (`analysis.py`) that compares SymNMF against K-Means clustering using the Silhouette Score and Adjusted Rand Index (ARI) from `sklearn.metrics`.

## 🛠️ Tech Stack & File Structure
- **C:** `symnmf.c`, `symnmf.h` (Core mathematical operations)
- **Python:** `symnmf.py`, `analysis.py` (CLI interface and algorithmic analysis)
- **Python C-API:** `symnmfmodule.c`, `setup.py` (Integration layer)
- **Build Tools:** `Makefile`, `gcc`

## ⚙️ Compilation & Installation

### 1. Build the Python C-Extension
To allow Python to import and use the C operations, build the extension module:
```bash
python3 setup.py build_ext --inplace
```

### 2. Build the Standalone C Executable
To run the matrix operations directly via the C command-line interface, compile the project using the provided Makefile:
```bash
make
```

## 💻 Usage

### Python Interface (`symnmf.py`)
Run the full SymNMF algorithm or output intermediate matrix transformations.
```bash
python3 symnmf.py <k> <goal> <file_name.txt>
```
* `k`: The number of required clusters.
* `goal`: 
  * `symnmf`: Performs full SymNMF and outputs the optimized matrix $H$.
  * `sym`: Outputs the Similarity Matrix $A$.
  * `ddg`: Outputs the Diagonal Degree Matrix $D$.
  * `norm`: Outputs the Normalized Similarity Matrix $W$.
* `file_name.txt`: Path to the input dataset.

### C Interface (`symnmf`)
Run the intermediate matrix operations directly in C.
```bash
./symnmf <goal> <file_name.txt>
```
*(Note: The C interface supports the `sym`, `ddg`, and `norm` goals).*

### Analysis & Benchmarking (`analysis.py`)
Compare the clustering quality of SymNMF against standard K-Means.
```bash
python3 analysis.py <k> <file_name.txt>
```
**Output Example:**
```text
nmf: 0.1162
kmeans: 0.1147
ari: 0.8243
```
