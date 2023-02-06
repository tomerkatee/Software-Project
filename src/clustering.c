# include "spkmeans.h"


/* cluster operations */

/**
 * Computes the euclidian distance between two datapoints
 * @param dp1 a datapoint
 * @param dp2 a datapoint
 * @return The distance between dp1 and dp2
*/
double euclidean_distance(Datapoint dp1, Datapoint dp2)
{
    double sum_squares = 0;
    int i;
    for(i = 0; i < dp_size; i++)
    {
        sum_squares += pow(dp1[i] - dp2[i], 2);
    }
    return sqrt(sum_squares);
}


/**
 * @return The distance from a cluster's centroid to its previous value
*/
double distance_from_prev(Cluster* cl)
{
    return euclidean_distance(cl->centroid, cl->prev);
}


/**
 * @return The distance from a cluster's centroid to a datapoint
*/
double distance_to_centroid(Datapoint dp, Cluster* cl)
{
    return euclidean_distance(dp, cl->centroid);
}


/**
 * Adds a datapoint to a cluster, assuming it isn't there
 * @param cl a cluster
 * @param dp a datapoint node
*/
void add_datapoint(Cluster* cl, DPNode* dp)
{
    DPNode* first = cl->datapoints;
    cl->datapoints = dp;
    dp->next = first;
    if(first != NULL)
    {
        first->prev = dp;
    }
    cl->size++;
}

/**
 * Removes a datapoint from its linked list.
 * If the datapoint is the head of the given cluster, removes it.
 * @param cl a cluster
 * @param dp a datapoint node
*/
void remove_datapoint(Cluster* cl, DPNode* dp)
{
    DPNode *next = dp->next, *prev = dp->prev;
    if(next != NULL)
    {
        next->prev = prev;
    }
    if(prev != NULL)
    {
        prev->next = next;
        cl->size--;
    }
    else if (cl->datapoints == dp)
    {
        cl->datapoints = next;
        cl->size--;
    }
    dp->next = NULL;
    dp->prev = NULL;
}


/**
 * Transfers a datapoint from the source cluster to the target cluster
 * @param source the source cluster
 * @param target the target cluster
 * @param dp the datapoint to transfer
*/
void transfer_datapoint(Cluster* source, Cluster* target, DPNode* dp)
{
    remove_datapoint(source, dp);
    add_datapoint(target, dp);
}


/**
 * Updates a cluster's cenroid and prev fields
 * @param cl a cluster to update
*/
void update_cluster(Cluster* cl)
{
    int i;
    Datapoint centroid = cl->centroid;
    DPNode *dp = cl->datapoints;

    for (i = 0; i < dp_size; i++)
    {
        (cl->prev)[i] = (cl->centroid)[i];
        centroid[i] = 0;
    }

    while(dp != NULL)
    {
        int j;
        for (j = 0; j < dp_size; j++)
        {
            centroid[j] += (dp->value)[j] / cl->size;
        }
        dp = dp->next;
    }
}


/**
 * Initializes a cluster from a datapoint
 * @param dp the datapoint to init with
*/
Cluster* init_cluster(DPNode* dp)
{
    int i;
    Cluster *cl = (Cluster*)calloc(1, sizeof(Cluster));
    cl->centroid = (Datapoint)calloc(dp_size, sizeof(double)); 
    cl->prev = (Datapoint)calloc(dp_size, sizeof(double));
    for (i = 0; i < dp_size; i++)
    {
        cl->centroid[i] = dp->value[i];
    }
    cl->datapoints = NULL;
    cl->size = 0;
    return cl;
}


/* kmeans algorithm */


/**
 * Finds the cluster with the closest centroid to a datapoint
 * @param clusters a list of clusters
 * @param dp the datapoint to compare to
 * @return Index of the closest cluster
*/
int get_closest_cluster(Cluster* clusters[], Datapoint dp)
{
    int j = 0, i = 1;
    double distance = distance_to_centroid(dp, clusters[j]), min_distance = distance;
    for(; i < K; ++i)
    {
        distance = distance_to_centroid(dp, clusters[i]);
        if(distance < min_distance)
        {
            min_distance = distance;
            j = i;
        }
    }
    return j;
}


/**
 * returns whether all centroid changes are less than epsilon
 * @param clusters a list of clusters
 * @return 0 iff exists a cluster with delta >= epsilon
*/
int convergence(Cluster* clusters[])
{
    int i = 0;
    for(; i < K; ++i)
    {
        if(distance_from_prev(clusters[i]) >= eps)
        {
            return 0;
        }
    }
    return 1;
}


Cluster** kmeans_clustering(DPNode *datapoints[], long initial_centroids[])
{
    int i, j, iteration_number = 0;
    int *datapoints_to_clusters = (int*)calloc(N, sizeof(int));
    Cluster **clusters = (Cluster**)calloc(K, sizeof(Cluster*));

    /* initialize centroids */
    for(i = 0; i < K; ++i) { clusters[i] = init_cluster(datapoints[initial_centroids[i]]); }
    
    while(1)
    {
        /* assign each datapoint to the closest cluster */
        for(i = 0; i < N; ++i)
        {
            j = get_closest_cluster(clusters, datapoints[i]->value);
            transfer_datapoint(clusters[datapoints_to_clusters[i]], clusters[j], datapoints[i]);
            datapoints_to_clusters[i] = j;
        }

        /* update the centroids */ 
        for(i = 0; i < K; ++i) { update_cluster(clusters[i]); }
        
        /* check for convergence */
        if((iteration_number++ >= iter) || convergence(clusters)){ break; }
    }

    free(datapoints_to_clusters);
    return clusters;
}