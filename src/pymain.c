#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "royal_graphs.h"

typedef struct {
    PyObject_HEAD
    Royal_Graph graph;
} RoyalGraphObject;
/**
 * Allows custom C functions to create new graphs without invoking new or init.
 */
extern PyTypeObject RoyalGraphType;

static void
RoyalGraph_dealloc(RoyalGraphObject *self)
{
    Royal_Graph_deinit(&(self->graph));
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
RoyalGraph_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RoyalGraphObject *self;
    self = (RoyalGraphObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->graph.data = NULL;
        self->graph.cap = 0;
        self->graph.grow = 0;
        self->graph.field = 0;
    }
    return (PyObject *) self;
}

static int
RoyalGraph_init(RoyalGraphObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"field", "capacity", "growth", NULL};
    unsigned int field_size = 0;
    Py_ssize_t capacity = 0;
    Py_ssize_t growth = 0;
    size_t* capacity_p = NULL;
    size_t* growth_p = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I|nn", kwlist,
                                     &field_size, &capacity,
                                     &growth))
        return -1;

    if (capacity)
        capacity_p = &capacity;
    if (growth)
        growth_p = &growth;
    if(!Royal_Graph_init(&(self->graph), field_size, capacity_p, growth_p)) {
        PyErr_SetString(PyExc_Exception, "RoyalGraph allocation failed");
        return -1;
    }
    return 0;
}

static PyObject *
RoyalGraph_repr(RoyalGraphObject *self)
{
    char* bufptr;
    PyObject* printed;
    int print_res;
    if (self->graph.data == NULL) {
        PyErr_SetString(PyExc_Exception, "RoyalGraph allocation failed");
        return NULL;
    }
    print_res = Royal_Graph_str(&(self->graph), &bufptr);
    if (!print_res) {
        PyErr_SetString(PyExc_Exception, "RoyalGraph representation failed");
        return NULL;
    }
    else if (print_res == -1) {
        printed = PyUnicode_FromString("/0/"); // the empty graph
    } else {
        printed = PyUnicode_FromString(bufptr);
        _Royal_free(bufptr);
    }
    return printed;
}

static PyObject *
RoyalGraph_size(RoyalGraphObject *self, PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromSize_t(self->graph.len);
}

static PyObject*
RoyalGraph_append(RoyalGraphObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"edge1", "vert", "edge2", "enforce",  NULL};
    const char* edge1;
    const char* vert;
    const char* edge2;
    int enforce = 0;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "sss|p", kwlist,
                                    &edge1, &vert, &edge2, &enforce)) {
        return NULL;
    }
    if(NULL == Royal_Graph_append(&(self->graph), edge1, vert, edge2, enforce)) {
        PyErr_SetString(PyExc_Exception, "The size of an element exceeds the field size");
        return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject*
RoyalGraph_trim(RoyalGraphObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"count", NULL};
    Py_ssize_t amount = 0;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "n", kwlist,
                                    &amount)) {
        return NULL;
    }
    if(!Royal_Graph_trim(&(self->graph), amount)) {
        PyErr_Format(PyExc_Exception, "Cannot trim more than %zu connections", self->graph.len);
        return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
RoyalGraph_copy(RoyalGraphObject* self, PyObject *args, PyObject *kwargs)
{
    RoyalGraphObject *copied;
    copied = (RoyalGraphObject *) RoyalGraphType.tp_alloc(&RoyalGraphType, 0);
    if (copied == NULL) {
        PyErr_SetString(PyExc_Exception, "RoyalGraph copy failed");
        return NULL;
    }

    if (!Royal_Graph_copy(&(copied->graph), &(self->graph))) {
        PyErr_SetString(PyExc_Exception, "RoyalGraph copy attempted on NULL graph");
        RoyalGraph_dealloc(copied);
        return NULL;
    }

    return (PyObject *) copied;
}

static PyMethodDef RoyalGraph_methods[] = {
    {"size", (PyCFunction) RoyalGraph_size, METH_NOARGS,
     "Returns the amount of connections"
    },
    {"append", (PyCFunction) RoyalGraph_append, METH_VARARGS | METH_KEYWORDS,
    "Adds a connection to the end of the graph"
    },
    {"trim", (PyCFunction) RoyalGraph_trim, METH_VARARGS | METH_KEYWORDS,
    "Erases connections from the end of the graph"
    },
    {"copy", (PyCFunction) RoyalGraph_copy, METH_VARARGS | METH_KEYWORDS,
    "Creates a full copy of the graph"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject RoyalGraphType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "royal.RoyalGraph",
    .tp_doc = "A container of graph connections",
    .tp_basicsize = sizeof(RoyalGraphObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = RoyalGraph_new,
    .tp_init = (initproc) RoyalGraph_init,
    .tp_dealloc = (destructor) RoyalGraph_dealloc,
    .tp_repr = (reprfunc) RoyalGraph_repr,
    .tp_methods = RoyalGraph_methods,
};

static PyObject* print_message(PyObject* self, PyObject* args)
{
    const char* str_arg;
    if(!PyArg_ParseTuple(args, "s", &str_arg)) {
        puts("Could not parse the python arg!");
        return NULL;
    }
#ifdef USE_PRINTER
    printf("royal %s\n", str_arg);
#else
    printf("royal %s\n", str_arg);
#endif
    // This can also be done with Py_RETURN_NONE
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef myMethods[] = {
    { "print_message", print_message, METH_VARARGS, "Prints a called string" },
    { NULL, NULL, 0, NULL }
};

// Our Module Definition struct
static struct PyModuleDef royalmodule = {
    PyModuleDef_HEAD_INIT,
    "royal",
    "A graph data format library",
    -1,
    myMethods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_royal(void)
{
    PyObject *m;
    if (PyType_Ready(&RoyalGraphType) < 0)
        return NULL;

    m = PyModule_Create(&royalmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&RoyalGraphType);
    if (PyModule_AddObject(m, "RoyalGraph", (PyObject *) &RoyalGraphType) < 0) {
        Py_DECREF(&RoyalGraphType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}