# Symmetric Non-Negative Matrix Factorization (SymNMF)

A high-performance hybrid C and Python implementation of a clustering algorithm based on Symmetric Non-Negative Matrix Factorization (SymNMF) for graph clustering. This project was developed as part of the Software Project course at Tel Aviv University.

The project leverages C for computationally heavy matrix operations and Python for the user interface and analytical comparisons, bridged together using the Python C API.

## 🚀 Features

* **Algorithmic Graph Clustering:** Groups data points by calculating similarities and finding a lower-dimensional non-negative matrix $H$ that minimizes the squared Frobenius norm $\vert{}\vert{}W - HH^T\vert{}\vert{}_F^2$.
* **Hybrid Architecture:** 
  * Core matrix transformations (Similarity, Degree, and Normalized Similarity matrices) and the $H$-matrix optimization loop are implemented in pure C for maximum performance.
  * A Python wrapper (`symnmfmodule.c`) exposes the C library to Python.
* **Comparative Analysis:** Includes a benchmarking tool (`analysis.py`) that compares SymNMF against K-Means clustering using the Silhouette Score and Adjusted Rand Index (ARI) from `sklearn.metrics`.

## 🛠️ Tech Stack & File Structure
* **C:** `symnmf.c`, `symnmf.h` (Core mathematical operations)
* **Python:** `symnmf.py`, `analysis.py` (CLI interface and algorithmic analysis)
* **Python C-API:** `symnmfmodule.c`, `setup.py` (Integration layer)
* **Build Tools:** `Makefile`, `gcc`

## ⚙️ Compilation & Installation

### 1. Build the Python C-Extension
To allow Python to import and use the C operations, build the extension module:
```bash
python3 setup.py build_ext --inplace
