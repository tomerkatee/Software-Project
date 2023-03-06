import sys, numpy as np
import matplotlib.pyplot as plt

import mykmeanssp


global k, goal, file_name


def read_args():

    global k, goal, file_name

    k_mentioned = sys.argv[1].isdigit()

    k = int(sys.argv[1]) if k_mentioned else -1

    goal = sys.argv[1+k_mentioned]

    file_name = sys.argv[2+k_mentioned]




def distance(dp1, dp2):

    return sum([(a - b)**2 for a, b in np.column_stack((dp1, dp2))]) ** 0.5



def kmeans_pp_algorithm(datapoints, n, k):

    dp_np = np.array(datapoints)

    np.random.seed(0)

    centroids = []

    distances = [-1 for i in range(n)]
    indices = list(range(n))

    while len(centroids) < k:

        normalized = np.array(distances, copy=False) / sum(distances) if len(centroids) else None

        new_center_index = int(np.random.choice(a=indices, p=normalized))

        centroids.append(new_center_index)

        new_centroid = dp_np[new_center_index, :]

        for i in range(n):

            dis_to_new = distance(dp_np[i, :], new_centroid)

            distances[i] = min(distances[i], dis_to_new) if len(centroids) > 1 else dis_to_new

    return centroids



def eigengap(eigenvals):

    sorted_vals = sorted(eigenvals)

    return np.argmax([sorted_vals[i+1]-sorted_vals[i] for i in range(len(sorted_vals)//2)]) + 1



def print_matrix(rows):
    for row in rows:
        print(','.join(["{:.4f}".format(v) for v in row]))



def main():
    global k   
    read_args()
    datapoints = mykmeanssp.read_datapoints(file_name)
    dp_size = len(datapoints[0])
    dp_count = len(datapoints)
    if goal == "jacobi":

        eigenvals, eigenvecs = mykmeanssp.jacobi(datapoints)

        print(','.join(["{:.4f}".format(v) for v in eigenvals]))

        print_matrix(eigenvecs)

    elif goal == "spk":     

        gl = mykmeanssp.gl(datapoints, dp_size)

        eigenvals, eigenvecs = mykmeanssp.jacobi(gl)

        if k == -1:
            k = eigengap(eigenvals)

        eigendatapoints = [[eigenvecs[j][i] for j in range(k)] for i in range(dp_count)]

        initial_centroids = kmeans_pp_algorithm(eigendatapoints, dp_count, k)
        print(','.join([str(c) for c in initial_centroids]))

        centroids = mykmeanssp.spk(eigendatapoints, initial_centroids, k)

        print_matrix(centroids)

    elif goal == "wam":

        print_matrix(mykmeanssp.wam(datapoints, dp_size))

    elif goal == "ddg":

        print_matrix(mykmeanssp.ddg(datapoints, dp_size))

    elif goal == "gl":

        print_matrix(mykmeanssp.gl(datapoints, dp_size))



if __name__ == '__main__':
    main()

