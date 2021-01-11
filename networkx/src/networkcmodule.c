#include "Python.h"
#include <stdio.h>

static PyObject *nwc_add_edges_from(PyObject *self, PyObject *args) {
    printf("in nwc_add_edges_from\n");
    return Py_None;
}

static PyMethodDef networkc_methods[] = {
    {"nwc_add_edges_from", nwc_add_edges_from, METH_VARARGS, "Add egdes from a list"},
    {NULL, NULL, 0, NULL}
};

// module Definition struct
static struct PyModuleDef networkc = {
    PyModuleDef_HEAD_INIT,
    "networkc",
    "C extensions of some networkx functions",
    -1,
    networkc_methods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_networkc(void)
{
    return PyModule_Create(&networkc);
}