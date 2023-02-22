import sys, numpy as np
#import mykmeanssp

global k, goal, file_name, dp_length, dp_count

err_msg = "An Error Has Occurred"
iter_err_msg = "Invalid maximum iteration!"
k_err_msg = "Invalid number of clusters!"

def read_args():
    global k, goal, file_name
    k_mentioned = sys.argv[1].isdigit()
    k = int(sys.argv[1]) if k_mentioned else -1
    goal = sys.argv[1+k_mentioned]
    file_name = sys.argv[2+k_mentioned]


def read_datapoints():
    global dp_length, dp_count
    with open(file_name) as file:
        datapoints = [line.split(',') for line in file.readlines()]
    dp_length = len(datapoints[0])
    dp_count =  len(datapoints)
    return datapoints


def read_matrix():
    global dp_count
    with open(file_name) as file:
        rows = [line.split(',') for line in file.readlines()]
    dp_count = len(rows)
    return rows


def distance(dp1, dp2):
    return sum([(a - b)**2 for a, b in np.column_stack((dp1, dp2))]) ** 0.5


def kmeans_pp_algorithm(datapoints, n, k):
    dp_np = datapoints.to_numpy()
    np.random.seed(0)
    centroids = []
    distances = [-1 for i in range(n)]
    indices = list(range(n))
    while len(centroids) < k:
        normalized = np.array(distances, copy=False) / sum(distances) if len(centroids) else None
        new_center_index = int(np.random.choice(a=indices, p=normalized))
        centroids.append(new_center_index)
        new_centroid = dp_np[new_center_index, 1:]
        for i in range(n):
            dis_to_new = distance(dp_np[i, 1:], new_centroid)
            distances[i] = min(distances[i], dis_to_new) if len(centroids) > 1 else dis_to_new
    return centroids


def eigengap(eigenvals):
    sorted_vals = sorted(eigenvals)
    return max([sorted_vals[i+1]-sorted_vals[i] for i in range(len(sorted_vals)-1)])


def print_matrix(rows):
    for row in rows:
        print(','.join(row))


def main():
    read_args()
    if goal == "jacobi":
        eigenvals, eigenvecs = mykmeansmodule.jacobi(read_matrix(), dp_count)
        print(','.join(eigenvals))
        print_matrix(eigenvecs)
    else:
        datapoints = read_datapoints()
        if goal == "spk":
            gl = mykmeansmodule.gl(datapoints, dp_length)
            eigenvals, eigenvecs = mykmeansmodule.jacobi(gl, dp_count)
            if k == -1:
                k = eigengap(eigenvals)
            eigendatapoints = [[eigenvecs[j][i] for j in range(k)] for i in range(dp_count)]
            initial_centroids = kmeans_pp_algorithm(eigendatapoints)
            print(initial_centroids)
            centroids = mykmeansmodule.spk(eigendatapoints, initial_centroids)
            print_matrix(centroids)
        if goal == "wam":
            print_matrix(mykmeansmodule.wam(datapoints, dp_length))
        if goal == "ddg":
            print_matrix(mykmeansmodule.ddg(datapoints, dp_length))
        if goal == "gl":
            print_matrix(mykmeansmodule.gl(datapoints, dp_length))


if __name__ == '__main__':
    main()
