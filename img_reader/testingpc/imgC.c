#include "Python.h"

#include <stdlib.h> 

#include <numpy/arrayobject.h>

static PyObject* metodo1(PyObject* self, PyObject* args){

    PyArrayObject *np_array = NULL;


    if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &np_array)) {
        return NULL; 
    }
    uint8_t* printar = np_array;
    npy_intp size = PyArray_SIZE(np_array);
    for (npy_intp i = 0; i < size; i++) {
        printf("%d ", printar[i]);
    }
    printf("\n");
    Py_RETURN_NONE;
};

static struct PyMethodDef methods[] = {
    {"metodo1", (PyCFunction) metodo1,METH_VARARGS, "Testando metodo simples"},
    {NULL, NULL}
};

static struct PyModuleDef modulo1 ={
    PyModuleDef_HEAD_INIT,
    "tester",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_modulo1(void) {
    import_array();  // Necessary for NumPy initialization
    return PyModule_Create(&modulo1);
}