# include "spkmeans.h"


/* cluster operations */

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
    Cluster *cl = (Cluster*)calloc_and_check(1, sizeof(Cluster));
    cl->centroid = (Datapoint)calloc_and_check(dp_size, sizeof(double)); 
    cl->prev = (Datapoint)calloc_and_check(dp_size, sizeof(double));
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
    double distance = euclidian_distance(dp, clusters[j]->centroid), min_distance = distance;
    for(; i < K; ++i)
    {
        distance = euclidian_distance(dp, clusters[i]->centroid);
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
        if(euclidian_distance(clusters[i]->centroid, clusters[i]->prev) >= EPS)
        {
            return false;
        }
    }
    return true;
}

Cluster** kmeans_clustering(DPNode *datapoints[], long initial_centroids[])
{
    int i, j, iteration_number = 0;
    Cluster **clusters = (Cluster**)calloc_and_check(K, sizeof(Cluster*));

    /* initialize centroids */
    for(i = 0; i < K; ++i) { clusters[i] = init_cluster(datapoints[initial_centroids[i]]); }
    
    while(true)
    {
        /* assign each datapoint to the closest cluster */
        for(i = 0; i < N; ++i)
        {
            j = get_closest_cluster(clusters, datapoints[i]->value);
            remove_datapoint(clusters[datapoints[i]->cl_i], datapoints[i]);
            add_datapoint(clusters[j], datapoints[i]);
            datapoints[i]->cl_i = j;
        }

        /* update the centroids */ 
        for(i = 0; i < K; ++i) { update_cluster(clusters[i]); }
        
        /* check for convergence */
        if((iteration_number++ >= ITER) || convergence(clusters)){ break; }
    }

    return clusters;
}