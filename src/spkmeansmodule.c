# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "spkmeans.h"


int get_datapoints_list(PyObject *lst, Datapoint datapoints[])
{
    PyObject *dp;
    PyObject *item;
    int i, j;

    for(i = 0; i < N; ++i)
    {
        dp = PyList_GetItem(lst, i);
        if(!PyList_Check(dp)){
            return false;
        }
        datapoints[i] = calloc_and_check(dp_size, sizeof(double));

        for(j = 0; j < dp_size; ++j)
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

int get_centroid_list(PyObject *lst, long initial_centroids[])
{
    PyObject *item;
    int i;
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

int get_matrix(PyObject *mat, Matrix M)
{
    PyObject *dp;
    PyObject *item;
    int i, j;

    for(i = 0; i < N; ++i)
    {
        dp = PyList_GetItem(lst, i);
        if(!PyList_Check(dp)){
            return false;
        }

        for(j = 0; j < N; ++j)
        {
            item = PyList_GetItem(dp, j);
            if(!PyFloat_Check(item)){
                return false;
            }
            M[i][j] = PyFloat_AsDouble(item);
        }
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

PyObject* build_matrix(Matrix M)
{
    PyObject* mat= PyList_New(N);
    PyObject* lst;
    int i, j;
    for(i = 0; i < N; ++i)
    {
        lst = PyList_New(N);
        for(j = 0; j < N; ++j)
        {
            PyList_SetItem(lst, j, Py_BuildValue("d", M[i][j]));
        }
        PyList_SetItem(mat, i, lst);
    }
    return mat;
}

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
    Datapoint *datapoints[N];
    if(!get_datapoints_list(dp_lst, datapoints)){
        return NULL;
    }

    /* fetch the initial centroids */
    K = PyList_Size(centroid_lst);
    long initial_centroids[K];
    if(!get_centroid_list(centroid_lst, initial_centroids)){
        return NULL;
    }

    /* build the datapoint nodes */
    DPNode *nodes[N];
    for(i = 0; i < N; ++i)
    {
        nodes[i]->value = datapoints[i];
        nodes[i]->next = NULL;
        nodes[i]->prev = NULL;
        nodes[i]->cluster = 0;
    }

    /* call the clustering method and build the return value */
    Cluster** clusters = kmeans_clustering(nodes, initial_centroids);
    centroid_lst = make_centroids(clusters);

    /* free the allocated memory */
    for(i = 0; i < N; ++i){ free(nodes[i]->value); }
    for(i = 0; i < K; ++i){ free(clusters[i]); }

    return Py_BuildValue("O", centroid_lst);
}

static PyObject* wam_ddg_gl_wrapper(PyObject *self, PyObject *args, int ddg_gl)
{
    PyObject* dp_lst;
    if(!PyArg_ParseTuple("Oi", &dp_lst, dp_size)){
        return NULL;
    }

    N = PyList_Size(dp_lst);
    Datapoint datapoints[N];
    if(!get_datapoints_list(dp_lst, datapoints)){
        return NULL;
    }

    Matrix M = wam_ddg_gl(datapoints, ddg_gl);
    PyObject* mat = build_matrix(M);

    free_matrix(M);
    return Py_BuildValue("O", mat);
}

static PyObject* wam_wrapper(yObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 0);
}

static PyObject* ddg_wrapper(yObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 1);
}

static PyObject* gl_wrapper(yObject *self, PyObject *args)
{
    return wam_ddg_gl_wrapper(self, args, 2);
}

static PyObject* jacobi_wrapper(yObject *self, PyObject *args)
{
    PyObject* dp_lst;
    if(!PyArg_ParseTuple("Oi", &dp_lst, dp_size)){
        return NULL;
    }

    N = PyList_Size(dp_lst);
    Matrix M = squareMatrix();
    if(!get_matrix(dp_lst, M)){
        return NULL;
    }

    Diagonalization diag = jacobi(M);
    PyObject* vecs = build_matrix(diag.eigenvectors);
    PyObject* vals = PyList_New(N);
    for(int i = 0; i < N; ++i)
    {
        PyList_SetItem(vals, i, diag.eigenvalues[i]);
    }

    free_matrix(M);
    free_matrix(diag.eigenvectors);
    free_datapoint(diag.eigenvalues);
    return Py_BuildValue("OO", vals, vecs);
}

static PyMethodDef kmeansMethods[] = {
    {
        /* name exposed to Python */
        "spk_wrapper",

        /* C wrapper function */
        spk_wrapper,

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