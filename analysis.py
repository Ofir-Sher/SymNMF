import sys
import pandas as pd
import numpy as np
from sklearn.metrics import silhouette_score, adjusted_rand_score
import symnmfmodule


'''Function: Calculates the Euclidean distance between two data points (a list/tuple of floats)'''
def euclidean_distance(p, q):
    return sum((p[i] - q[i]) ** 2 for i in range(len(p))) ** 0.5


'''Function: Performs the K-Means clustering algorithm on a set of data points.
Input: A list of datapoints (floats), int k for the number of clusters and a const int iters for the maximum number of interation
Output: A list of integers representing the final cluster assignment for each datapoint.'''
def run_kmeans(datapoints, k, iters=300):
    eps = 1e-4
    N = len(datapoints)
    dim = len(datapoints[0])

    centroids = [point.copy() for point in datapoints[:k]]
    old_centroids = [point.copy() for point in datapoints[:k]]
    assignments = [-1] * N
    iteration = 0

    while iteration < iters:
        for j in range(N):
            min_index = 0
            index_val = euclidean_distance(centroids[0], datapoints[j])

            for i in range(1, k):
                curr_val = euclidean_distance(centroids[i], datapoints[j])
                if curr_val < index_val:
                    index_val = curr_val
                    min_index = i

            assignments[j] = min_index

        for idx in range(k):
            curr_cent = [0.0] * dim
            size = 0
            for i in range(N):
                if assignments[i] == idx:
                    size += 1
                    for j in range(dim):
                        curr_cent[j] += datapoints[i][j]

            if size > 0:
                centroids[idx] = [x / size for x in curr_cent]

        deltas = [euclidean_distance(centroids[i], old_centroids[i]) for i in range(k)]
        if all(delta < eps for delta in deltas):
            break
            
        old_centroids = [c[:] for c in centroids]
        iteration += 1
        
    return assignments

'''Function: Main file function, takes user's input and prints the corresponding matrix.
Reads data points from a specified CSV file, performs both 
clustering algorithms, and evaluates their performance using the Silhouette Score.
In addition, compares the similarity of the two clusterings using the adjusted_rand_score.'''
def main():

    if len(sys.argv) != 3:
        print("An Error Has Occurred")
        sys.exit(1)

    try:
        k_float = float(sys.argv[1])
        if not k_float.is_integer():
            print("Incorrect number of clusters!"); sys.exit(1)
        k = int(k_float)
    except ValueError:
        print("Incorrect number of clusters!"); sys.exit(1)

    file_name = sys.argv[2]

    data = pd.read_csv(file_name, header=None)
    X = data.values.tolist()
    n = len(X)
    d = len(X[0])

    W = symnmfmodule.norm(X)
    np.random.seed(1234)
    H_np = np.random.uniform(0, 2 * np.sqrt(np.mean(np.array(W)) / k), size=(n, k))
    
    final_H = symnmfmodule.symnmf(W, H_np.tolist(), n, k)
    symnmf_clusters = np.argmax(np.array(final_H), axis=1)
    
    kmeans_clusters = run_kmeans(X, k)

    try:
        nmf_score = silhouette_score(X, symnmf_clusters)
        kmeans_score = silhouette_score(X, kmeans_clusters)
    except ValueError:
        print("An Error Has Occurred")
        sys.exit(1)
    
    print(f"nmf: {nmf_score:.4f}")
    print(f"kmeans: {kmeans_score:.4f}")
    print(f"ari: {adjusted_rand_score(symnmf_clusters, kmeans_clusters):.4f}")

if __name__ == "__main__":
    main()