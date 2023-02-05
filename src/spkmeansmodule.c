# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "spkmeans.h"


/* wrapper function */

int get_datapoints_list(PyObject *lst, Datapoint *datapoints[])
{
    PyObject *dp;
    PyObject *item;
    int i, j;

    for(i = 0; i < dp_list_length; ++i)
    {
        dp = PyList_GetItem(lst, i);
        if(!PyList_Check(dp)){
            return 0;
        }
        datapoints[i]->value = calloc(dp_size, sizeof(double));

        datapoints[i]->next = NULL;
        datapoints[i]->prev = NULL;
        for(j = 0; j < dp_size; ++j)
        {
            item = PyList_GetItem(dp, j);
            if(!PyFloat_Check(item)){
                return 0;
            }
            datapoints[i]->value[j] = PyFloat_AsDouble(item);
        }
    }
    return 1;
}


int get_centroid_list(PyObject *lst, long initial_centroids[])
{
    PyObject *item;
    int i;
    for(i = 0; i < K; ++i)
    {
        item = PyList_GetItem(lst, i);
        if(!PyLong_Check(item)){
            return 0;
        }
        initial_centroids[i] = PyLong_AsLong(item);
    }
    return 1;
}


PyObject* make_centroids(Cluster** clusters)
{
    PyObject* centroids = PyList_New(K);
    PyObject* lst;
    int i, j;
    for(i = 0; i < K; ++i)
    {
        lst = PyList_New(dp_size);
        for(j = 0; j < dp_size; ++j)
        {
            PyList_SetItem(lst, j, Py_BuildValue("d", clusters[i]->centroid[j]));
        }
        PyList_SetItem(centroids, i, lst);
    }
    return centroids;
}


static PyObject* fit(PyObject *self, PyObject *args)
{
    PyObject *dp_lst;
    PyObject *centroid_lst;
    int i;

    if(!PyArg_ParseTuple(args, "OOiid", &dp_lst, &centroid_lst, &dp_size, &iter, &eps)){
        return NULL;
    }

    dp_list_length = PyList_Size(dp_lst);
    if(dp_list_length < 0){
        return NULL;
    }
    Datapoint *datapoints[dp_list_length];
    if(!get_datapoints_list(dp_lst, datapoints)){
        return NULL;
    }

    K = PyList_Size(centroid_lst);
    if(K < 0){
        return NULL;
    }
    long initial_centroids[K];
    if(!get_centroid_list(centroid_lst, initial_centroids)){
        return NULL;
    }

    Cluster** clusters = kmeans_clustering(datapoints, initial_centroids);
    centroid_lst = make_centroids(clusters);

    for(i = 0; i < dp_list_length; ++i){
        free(datapoints[i]->value);
    }
    for(i = 0; i < K; ++i){
        free(clusters[i]);
    }

    return Py_BuildValue("O", centroid_lst);
}


/* method and module def */


static PyMethodDef kmeansMethods[] = {
    {
        /* name exposed to Python */
        "fit",

        /* C wrapper function */
        fit,

        /* receives variable args */
        METH_VARARGS,

        /* docstring */
        "The kmeans-clustering algorithm\n"
        "@param list of datapoint pointers\n"
        "@param list of indexes of the initial centroids as in the kmeans_pp init\n"
        "@param size of each datapoint\n"
        "@param number of K-means iterations\n"
        "@return list of centroids"
    }, {
        NULL, NULL, 0, NULL
    }
};


static struct PyModuleDef mykmeanssp = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    "C module for the kmeans-clustering algorithm.",
    -1,
    kmeansMethods
};


PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *module = PyModule_Create(&mykmeanssp);
    if(!module){
        return NULL;
    }
    return module;
}