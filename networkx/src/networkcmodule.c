#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <stdio.h>

static PyObject *add_edges_from(PyObject *self, PyObject *args, PyObject *attr) {

    // list of argument names
    static char* keywords[] = {"graph_node", "graph_adj", "ebunch_to_add", "attr", NULL};
    PyObject *dd = NULL;
    PyObject *e = NULL;

    // parse the argument list, attr is optional
    PyObject *graph_node, *graph_adj, *ebunch_to_add;
    if(!PyArg_ParseTupleAndKeywords(args, attr, (char *)"OOO|O", keywords, &graph_node, &graph_adj, &ebunch_to_add, &attr)) {
        return NULL;
    }

    if(!PyDict_Check(graph_node)) {
        PyErr_SetString(PyExc_TypeError, "ERROR add_edges_from: wrong argument #1, node dictionary required");
        return NULL;
    }

    if(!PyDict_Check(graph_adj)) {
        PyErr_SetString(PyExc_TypeError, "ERROR add_edges_from: wrong argument #2, adj dictionary required");
        return NULL;
    }

    PyObject *iter = PyObject_GetIter(ebunch_to_add);

    if(!PyIter_Check(iter)) {
        PyErr_SetString(PyExc_TypeError, "ERROR add_edges_from: wrong argument #3, must be iterable");
        return NULL;
    }

    
    while ((e = PyIter_Next(iter))) {

        Py_ssize_t ne = PySequence_Length(e);
        PyObject *u = PySequence_GetItem(e, 0);
        PyObject *v = PySequence_GetItem(e, 1);
        dd = PyDict_New(); // new reference
        if (ne == 3) {
            PyDict_Update(dd, attr);
        }
        if (!PyDict_Contains(graph_node, u)) {
            // create u node
            PyDict_SetItem(graph_adj, u, PyDict_New());
            PyDict_SetItem(graph_node, u, PyDict_New());
        }
        if (!PyDict_Contains(graph_node, v)) {
            // create v node
            PyDict_SetItem(graph_adj, v, PyDict_New());
            PyDict_SetItem(graph_node, v, PyDict_New());
        }
        // update the attributes
        PyDict_SetItem(PyDict_GetItem(graph_adj, u), v, dd);
        PyDict_SetItem(PyDict_GetItem(graph_adj, v), u, dd);

        Py_DECREF(e);
    }

    Py_DECREF(iter);

    /*
 925         for e in ebunch_to_add:
 926             ne = len(e)
 927             if ne == 3:
 928                 u, v, dd = e
 929             elif ne == 2:
 930                 u, v = e
 931                 dd = {}  # doesn't need edge_attr_dict_factory
 932             else:
 933                 raise NetworkXError(f"Edge tuple {e} must be a 2-tuple or 3-tuple.")
 934             if u not in self._node:
 935                 self._adj[u] = self.adjlist_inner_dict_factory()
 936                 self._node[u] = self.node_attr_dict_factory()
 937             if v not in self._node:
 938                 self._adj[v] = self.adjlist_inner_dict_factory()
 939                 self._node[v] = self.node_attr_dict_factory()
 940             datadict = self._adj[u].get(v, self.edge_attr_dict_factory())
 941             datadict.update(attr)
 942             datadict.update(dd)
 943             self._adj[u][v] = datadict
 944             self._adj[v][u] = datadict
    */
 

    Py_RETURN_NONE;
}

static PyMethodDef networkc_methods[] = {
    {"add_edges_from", (PyCFunction) add_edges_from, METH_VARARGS | METH_KEYWORDS, "Add egdes from a list"},
    {NULL, NULL, 0, NULL}
};

// module Definition struct
static struct PyModuleDef networkcmodule = {
    PyModuleDef_HEAD_INIT,
    "networkc", /* name of the module */
    "C extensions of some networkx functions", /* module doc */
    -1,  /* -1 if module keeps state in global variables */
    networkc_methods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_networkc(void)
{
    return PyModule_Create(&networkcmodule);
}