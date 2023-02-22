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

"""
def validate_args():
    if len(sys.argv) != 5 and len(sys.argv) != 6:
        print(err_msg)
        return False
    global k, iter, eps, file_name_1, file_name_2
    k = sys.argv[1]
    offset_iter_mentioned = 1 if len(sys.argv) == 6 else 0
    iter = sys.argv[2] if offset_iter_mentioned else 300
    eps = sys.argv[2 + offset_iter_mentioned]
    file_name_1 = sys.argv[3 + offset_iter_mentioned]
    file_name_2 = sys.argv[4 + offset_iter_mentioned]

    if offset_iter_mentioned and not iter.isdigit():
        print(iter_err_msg)
        return False
    else:
        iter = int(iter)
        if not 1 < iter < 1000:
            print(iter_err_msg)
            return False
    if k.isdigit():
        k = int(k)
    else:
        print(k_err_msg)
        return False
    if eps.replace('.', '', 1).isdigit():
        eps = float(eps)
    else:
        print(err_msg)
        return False
    return True
"""


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

    """
    if not validate_args():
        exit()
    datapoints = read_datapoints()
    if not 1 < k < n:
        print(k_err_msg)
        exit()
    print_items = lambda l, f: print(','.join(map(f, l)))
    initial_centroids = kmeans_pp_algorithm(datapoints.to_numpy(copy=True), n, k)
    print_items(initial_centroids, str)
    datapoints_list = datapoints.to_numpy()[:, 1:].tolist()
    final_centroids = mykmeanssp.fit(datapoints_list, initial_centroids, len(datapoints.columns)-1, iter, eps)
    for p in final_centroids:
        print_items(p, lambda x: '{:.4f}'.format(x))
    """


if __name__ == '__main__':
    main()