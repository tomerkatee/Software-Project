# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "spkmeans.h"


int get_datapoints(PyObject *lst, Datapoint datapoints[], int size)
{
    PyObject *dp;
    PyObject *item;
    int i, j;

    if(!PyList_Check(lst)){
        return false;
    }
    
    for(i = 0; i < N; ++i)
    {
        dp = PyList_GetItem(lst, i);
        
        if(!PyList_Check(dp)){
            return false;
        }
        datapoints[i] = calloc_and_check(size, sizeof(double));
        
        for(j = 0; j < size; ++j)
        {
            item = PyList_GetItem(dp, j);
            if(!PyFloat_Check(item)){
                return false;
            }
            datapoints[i][j] = PyFloat_AsDouble(item);
        }
    }
    return true;
}

int get_centroids(PyObject *lst, long initial_centroids[])
{
    PyObject *item;
    int i;

    if(!PyList_Check(lst)){
        return false;
    }

    for(i = 0; i < K; ++i)
    {
        item = PyList_GetItem(lst, i);
        if(!PyLong_Check(item)){
            return false;
        }
        initial_centroids[i] = PyLong_AsLong(item);
    }
    return true;
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

PyObject* make_datapoints(Datapoint* datapoints, int size)
{
    PyObject* dp_lst = PyList_New(N);
    PyObject* lst;
    for(int i = 0; i < N; ++i)
    {
        lst = PyList_New(size);
        for(int j = 0; j < size; ++j)
        {
            PyList_SetItem(lst, j, Py_BuildValue("d", datapoints[i][j]));
        }
        PyList_SetItem(dp_lst, i, lst);
    }
    return dp_lst;
}

/* wrapper methods */

static PyObject* spk_wrapper(PyObject *self, PyObject *args)
{
    PyObject *dp_lst;
    PyObject *centroid_lst;
    int i;
    /* take in the raw arguments */
    if(!PyArg_ParseTuple(args, "OOi", &dp_lst, &centroid_lst, &dp_size)){
        return NULL;
    }
    /* fetch the datapoints */
    N = PyList_Size(dp_lst);
    Datapoint *datapoints = (Datapoint*)calloc_and_check(N, sizeof(Datapoint));
    if(!get_datapoints(dp_lst, datapoints, dp_size)){
        return NULL;
    }
    /* fetch the initial centroids */
    K = PyList_Size(centroid_lst);
    long initial_centroids[K];
    if(!get_centroids(centroid_lst, initial_centroids)){
        return NULL;
    }
    
    /* build the datapoint nodes */
    DPNode **nodes_pointers = (DPNode**)calloc_and_check(N, sizeof(DPNode*));
    DPNode *nodes = (DPNode*)calloc_and_check(N, sizeof(DPNode));
    for(i = 0; i < N; ++i){ nodes_pointers[i] = &nodes[i]; }
    for(i = 0; i < N; ++i)
    {
        nodes_pointers[i]->value = datapoints[i];
        nodes_pointers[i]->next = NULL;
        nodes_pointers[i]->prev = NULL;
        nodes_pointers[i]->cluster = 0;
    }
    /* call the clustering method and build the return value */
    Cluster** clusters = kmeans_clustering(nodes_pointers, initial_centroids);
    centroid_lst = make_centroids(clusters);

    /* free the allocated memory */
    free(nodes_pointers);
    free(nodes);
    for(i = 0; i < K; ++i){ free(clusters[i]); }
    for(i = 0; i < N; ++i){ free(datapoints[i]); }
    return Py_BuildValue("O", centroid_lst);
}

static PyObject* wam_ddg_gl_wrapper(PyObject *self, PyObject *args, int ddg_gl)
{
    PyObject* dp_lst = NULL;
    if(!PyArg_ParseTuple(args, "Oi", &dp_lst, &dp_size)){
        return NULL;
    }

    N = PyList_Size(dp_lst);
    Datapoint datapoints[N];
    if(!get_datapoints(dp_lst, datapoints, dp_size)){
        return NULL;
    }

    Matrix M = wam_ddg_gl(datapoints, ddg_gl);
    PyObject* mat = make_datapoints(M, N);

    free_matrix(M);
    for(int i = 0; i < N; ++i){ free(datapoints[i]); }
    return Py_BuildValue("O", mat);
}

static PyObject* wam_wrapper(PyObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 0);
}

static PyObject* ddg_wrapper(PyObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 1);
}

static PyObject* gl_wrapper(PyObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 2);
}

static PyObject* jacobi_wrapper(PyObject *self, PyObject *args)
{
    PyObject* dp_lst = NULL;
    if(!PyArg_ParseTuple(args, "O", &dp_lst)){
        return NULL;
    }
    
    N = PyList_Size(dp_lst);
    Matrix M = (Matrix)calloc_and_check(N, sizeof(Datapoint));

    if(!get_datapoints(dp_lst, M, N)){
        
        return NULL;
    }
    Diagonalization diag = jacobi(M);
    PyObject* vecs = make_datapoints(diag.eigenvectors, N);
    PyObject* vals = PyList_New(N);
    for(int i = 0; i < N; ++i)
    {
        PyList_SetItem(vals, i, Py_BuildValue("d", diag.eigenvalues[i]));
    }
    free_matrix(M);
    free_matrix(diag.eigenvectors);
    free(diag.eigenvalues);
    return Py_BuildValue("OO", vals, vecs);
}

static PyObject* read_datapoints_wrapper(PyObject* self, PyObject* args)
{
    char* filename = NULL;
    if(!PyArg_ParseTuple(args, "s", &filename)){
        return NULL;
    }
    Datapoint* datapoints = read_datapoints(filename);
    PyObject* dp_lst = make_datapoints(datapoints, dp_size);

    free(datapoints);
    return Py_BuildValue("O", dp_lst);
}

/* method and module definition */

static PyMethodDef spkmeansMethods[] = {
    {
        /* name exposed to Python */
        "spk",

        /* C wrapper function */
        spk_wrapper,

        /* receives variable args */
        METH_VARARGS,

        /* docstring */
        PyDoc_STR("Receives list of datapoints, indexes of initial centroids and the datapoint size, returns the final centroids")
    }, {
        "wam",

        wam_wrapper,

        METH_VARARGS,

        PyDoc_STR("Receives list of datapoints and the datapoint size, returns the Weighted Adjacency Matrix")
    }, {
        "ddg",

        ddg_wrapper,

        METH_VARARGS,

        PyDoc_STR("Receives list of datapoints and the datapoint size, returns the Diagonal Degree Matrix")
    }, {
        "gl",

        gl_wrapper,
        
        METH_VARARGS,

        PyDoc_STR("Receives list of datapoints and the datapoint size, returns the Graph Laplacian")
    }, {
        "jacobi",

        jacobi_wrapper,

        METH_VARARGS,

        PyDoc_STR("Receives a real, symmetric, full rank matrix and returns its eigenvalues and eigenvectors")
    }, {
        "read_datapoints",

        read_datapoints_wrapper,

        METH_VARARGS,

        PyDoc_STR("Receives a filename, reads the file and returns the datapoints")
    }, {
        NULL, NULL, 0, NULL
    }
};

static struct PyModuleDef mykmeanssp = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    "C module for the kmeans-clustering algorithm.",
    -1,
    spkmeansMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *module = PyModule_Create(&mykmeanssp);
    return !module ? NULL : module;
}