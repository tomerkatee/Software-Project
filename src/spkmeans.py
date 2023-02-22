import sys, numpy as np, pandas as pd
import mykmeanssp

global k, iter, eps, file_name_1, file_name_2, n

err_msg = "An Error Has Occurred"
iter_err_msg = "Invalid maximum iteration!"
k_err_msg = "Invalid number of clusters!"


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


def read_datapoints():
    global n
    dp1 = pd.read_csv(file_name_1, sep=',', header=None)
    dp2 = pd.read_csv(file_name_2, sep=',', header=None)
    datapoint_size = len(dp1.columns) - 1
    dp1.columns = ["key"] + [str(i) for i in range(datapoint_size)]
    dp2.columns = ["key"] + [str(i) for i in range(datapoint_size)]
    dp = dp1.merge(dp2, on="key", how="inner").sort_values(by="key")
    n = len(dp)
    return dp


def distance(dp1, dp2):
    return sum([(a - b)**2 for a, b in np.column_stack((dp1, dp2))]) ** 0.5


def kmeans_pp_algorithm(dp_np: np.ndarray, n, k):
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


def main():
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


if __name__ == '__main__':
    main()