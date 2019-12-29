#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "royal_graphs.h"

typedef struct {
    PyObject_HEAD
    Royal_Graph graph;
} RoyalGraphObject;

static void
RoyalGraph_dealloc(RoyalGraphObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
RoyalGraph_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RoyalGraphObject *self;
    self = (RoyalGraphObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->graph->data = NULL;
        self->graph->cap = 0;
        self->graph->grow = 0;
        self->graph->field = 0;
    }
    return (PyObject *) self;
}

static int
RoyalGraph_init(CustomObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"field", "capacity", "growth", NULL};
    unsigned int field_size = 0;
    unsigned int capacity = 0;
    unsigned int growth = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I|II", kwlist,
                                     &field_size, &capacity,
                                     &growth))
        return -1;

    if (capacity) {
        tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        Py_XDECREF(tmp);
    }
    if (growth) {
        tmp = self->last;
        Py_INCREF(last);
        self->last = last;
        Py_XDECREF(tmp);
    }
    return 0;
}

static PyTypeObject RoyalGraphType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "royal.RoyalGraph",
    .tp_doc = "A container of graph connections",
    .tp_basicsize = sizeof(RoyalGraphObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = RoyalGraph_new,
    .tp_init = (initproc) RoyalGraph_init,
    .tp_dealloc = (destructor) RoyalGraph_dealloc,
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