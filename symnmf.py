import sys
import pandas as pd
import numpy as np
import symnmfmodule

#Printing function for python
def print_matrix(matrix):
    """Prints a matrix formatted to 4 decimal places, comma-separated."""
    for row in matrix:
        print(",".join([f"{val:.4f}" for val in row]))


'''Function: Main file function, takes user's input and prints the corresponding matrix.'''
def main():
    if len(sys.argv) != 4:
        print("An Error Has Occurred"); sys.exit(1)

    try:
        k_float = float(sys.argv[1])
        if not k_float.is_integer():
            print("Incorrect number of clusters!"); sys.exit(1)
        k = int(k_float)
    except ValueError:
        print("Incorrect number of clusters!"); sys.exit(1)

    goal, file_name = sys.argv[2], sys.argv[3]

    try:
        data = pd.read_csv(file_name, header=None)
        X, n, d = data.values.tolist(), len(data), len(data.columns)
    except Exception:
        print("An Error Has Occurred"); sys.exit(1)

    if not (1 < k < n):
        print("Incorrect number of clusters!"); sys.exit(1)


    if goal == "sym":
        print_matrix(symnmfmodule.sym(X))
    elif goal == "ddg":
        print_matrix(symnmfmodule.ddg(X))
    elif goal == "norm":
        print_matrix(symnmfmodule.norm(X))
    elif goal == "symnmf":
        W = symnmfmodule.norm(X)
        m = np.mean(np.array(W))
        
        np.random.seed(1234)
        H_np = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k))
        
        print_matrix(symnmfmodule.symnmf(W, H_np.tolist(), n, k))
    else:
        print("An Error Has Occurred"); sys.exit(1)

if __name__ == "__main__":
    main()