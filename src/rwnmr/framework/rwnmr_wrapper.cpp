#include "Python.h"

#include <stdlib.h> 

#include <numpy/arrayobject.h>
#include <iostream>
#include "../configFiles/CpmgConfig.h"

static PyObject* metodo1(PyObject* self, PyObject* args){

    PyArrayObject *np_array;


    if (!PyArg_ParseTuple(args, "O", &np_array)) {
        return NULL; 
    }else{
    uint8_t* printar = (uint8_t*)PyArray_DATA(np_array);
    npy_intp size = PyArray_SIZE(np_array);
    uint8_t total=0;
    for (npy_intp i = 0; i < size; i++) {
        total += printar[i];
    }
    Py_DECREF(np_array);
    return PyLong_FromUnsignedLongLong((unsigned long long)total);
    }
};

static PyObject* metodo2(PyObject* self, PyObject* args){
    PyObject* texto;
    if(!PyArg_ParseTuple(args, "U", &texto)){
        Py_DecRef(texto);
        return NULL;
    }
    PyObject* ret = PyUnicode_FromFormat("Hello from %U!", texto);
    return ret;
};
static PyObject* CPMG(PyObject* self, PyObject* args){
    PyObject* CPMG_object;

    if(!PyArg_ParseTuple(args, "O", &CPMG_object )){
        Py_DecRef(CPMG_object);
        return NULL;
    }
    
    CpmgConfig cpmg_config;

    PyObject* applyBulk = PyObject_GetAttrString(CPMG_object, "apply_bulk");
    PyObject* obsTime = PyObject_GetAttrString(CPMG_object, "obs_time");
    PyObject* method = PyObject_GetAttrString(CPMG_object, "method");
    PyObject* timeVerbose = PyObject_GetAttrString(CPMG_object, "time_verbose");
    PyObject* residualField = PyObject_GetAttrString(CPMG_object, "residual_field");
    PyObject* gradientValue = PyObject_GetAttrString(CPMG_object, "gradient_value");
    PyObject* gradientDirection = PyObject_GetAttrString(CPMG_object, "gradient_direction");
    PyObject* interpolateField = PyObject_GetAttrString(CPMG_object, "interpolate_field");
    PyObject* minT2 = PyObject_GetAttrString(CPMG_object, "min_t2");
    PyObject* maxT2 = PyObject_GetAttrString(CPMG_object, "max_t2");
    PyObject* useT2Logspace = PyObject_GetAttrString(CPMG_object, "use_t2_logspace");
    PyObject* numT2Bins = PyObject_GetAttrString(CPMG_object, "num_t2_bins");
    PyObject* minLambda = PyObject_GetAttrString(CPMG_object, "min_lambda");
    PyObject* maxLambda = PyObject_GetAttrString(CPMG_object, "max_lambda");
    PyObject* numLambdas = PyObject_GetAttrString(CPMG_object, "num_lambdas");
    PyObject* pruneNum = PyObject_GetAttrString(CPMG_object, "prune_num");
    PyObject* noiseAmp = PyObject_GetAttrString(CPMG_object, "noise_amp");
    PyObject* saveMode = PyObject_GetAttrString(CPMG_object, "save_mode");
    PyObject* saveT2 = PyObject_GetAttrString(CPMG_object, "save_t2");
    PyObject* saveWalkers = PyObject_GetAttrString(CPMG_object, "save_walkers");
    PyObject* saveDecay = PyObject_GetAttrString(CPMG_object, "save_decay");
    PyObject* saveHistogram = PyObject_GetAttrString(CPMG_object, "save_histogram");
    PyObject* saveHistogramList = PyObject_GetAttrString(CPMG_object, "save_histogram_list");

    if (applyBulk == NULL || obsTime == NULL || method == NULL || timeVerbose == NULL || residualField == NULL ||
        gradientValue == NULL || gradientDirection == NULL || interpolateField == NULL ||
        minT2 == NULL || maxT2 == NULL || useT2Logspace == NULL || numT2Bins == NULL || minLambda == NULL ||
        maxLambda == NULL || numLambdas == NULL || pruneNum == NULL || noiseAmp == NULL || saveMode == NULL ||
        saveT2 == NULL || saveWalkers == NULL || saveDecay == NULL || saveHistogram == NULL || saveHistogramList == NULL) {
        Py_XDECREF(applyBulk);
        Py_XDECREF(obsTime);
        Py_XDECREF(method);
        Py_XDECREF(timeVerbose);
        Py_XDECREF(residualField);
        Py_XDECREF(gradientValue);
        Py_XDECREF(gradientDirection);
        Py_XDECREF(interpolateField);
        Py_XDECREF(minT2);
        Py_XDECREF(maxT2);
        Py_XDECREF(useT2Logspace);
        Py_XDECREF(numT2Bins);
        Py_XDECREF(minLambda);
        Py_XDECREF(maxLambda);
        Py_XDECREF(numLambdas);
        Py_XDECREF(pruneNum);
        Py_XDECREF(noiseAmp);
        Py_XDECREF(saveMode);
        Py_XDECREF(saveT2);
        Py_XDECREF(saveWalkers);
        Py_XDECREF(saveDecay);
        Py_XDECREF(saveHistogram);
        Py_XDECREF(saveHistogramList);
        return NULL;
    }

    cpmg_config.setConfig(
        PyUnicode_AsUTF8(applyBulk),
        PyUnicode_AsUTF8(obsTime),
        PyUnicode_AsUTF8(method),
        PyUnicode_AsUTF8(timeVerbose),
        PyUnicode_AsUTF8(residualField),
        PyUnicode_AsUTF8(gradientValue),
        PyUnicode_AsUTF8(gradientDirection),
        PyUnicode_AsUTF8(interpolateField),
        PyUnicode_AsUTF8(minT2),
        PyUnicode_AsUTF8(maxT2),
        PyUnicode_AsUTF8(useT2Logspace),
        PyUnicode_AsUTF8(numT2Bins),
        PyUnicode_AsUTF8(minLambda),
        PyUnicode_AsUTF8(maxLambda),
        PyUnicode_AsUTF8(numLambdas),
        PyUnicode_AsUTF8(pruneNum),
        PyUnicode_AsUTF8(noiseAmp),
        PyUnicode_AsUTF8(saveMode),
        PyUnicode_AsUTF8(saveT2),
        PyUnicode_AsUTF8(saveWalkers),
        PyUnicode_AsUTF8(saveDecay),
        PyUnicode_AsUTF8(saveHistogram),
        PyUnicode_AsUTF8(saveHistogramList)
    );

    Py_DECREF(applyBulk);
    Py_DECREF(obsTime);
    Py_DECREF(method);
    Py_DECREF(timeVerbose);
    Py_DECREF(residualField);
    Py_DECREF(gradientValue);
    Py_DECREF(gradientDirection);
    Py_DECREF(interpolateField);
    Py_DECREF(minT2);
    Py_DECREF(maxT2);
    Py_DECREF(useT2Logspace);
    Py_DECREF(numT2Bins);
    Py_DECREF(minLambda);
    Py_DECREF(maxLambda);
    Py_DECREF(numLambdas);
    Py_DECREF(pruneNum);
    Py_DECREF(noiseAmp);
    Py_DECREF(saveMode);
    Py_DECREF(saveT2);
    Py_DECREF(saveWalkers);
    Py_DECREF(saveDecay);
    Py_DECREF(saveHistogram);
    Py_DECREF(saveHistogramList);
    return PyUnicode_FromFormat("Caminho para o campo: %s", cpmg_config.getMethod().c_str());
    
}

static PyObject *recebe_objeto_classe(PyObject *self, PyObject *args) {
    PyObject *obj_python;
    PyObject *classe_python;
    if (!PyArg_ParseTuple(args, "OO", &obj_python, &classe_python)) {
        return NULL; // Erro: argumentos incorretos
    }

    // Verificando se obj_python é uma instância de classe_python
    if (PyObject_IsInstance(obj_python, classe_python)) {
        // Exemplo: acessando um atributo do objeto
        PyObject *nome_attr = PyObject_GetAttrString(obj_python, "path_to_field");
        if (nome_attr != NULL) {
            if (PyUnicode_Check(nome_attr)) {
                const char *nome = PyUnicode_AsUTF8(nome_attr);
                PyObject *result = PyUnicode_FromFormat("Nome do objeto: %s", nome);
                Py_DECREF(nome_attr);
                return result;
            }
            Py_DECREF(nome_attr);
        }
        Py_RETURN_NONE;
    } else {
        return PyUnicode_FromString("Objeto Python recebido NÃO é uma instância da classe esperada.");
    }

    Py_RETURN_NONE;
}

static struct PyMethodDef methods[] = {
    {"metodo1", (PyCFunction) metodo1,METH_VARARGS, "Testando metodo simples"},
    {"metodo2", (PyCFunction) metodo2,METH_VARARGS, "Testando print simples"},
    {"CPMG", (PyCFunction) CPMG,METH_VARARGS, "Testando CPMG"},
    {"recebe_objeto_classe", (PyCFunction) recebe_objeto_classe, METH_VARARGS, "Recebe um objeto Python e uma classe Python"},
    {NULL, NULL}
};

static struct PyModuleDef rwnmr ={
    PyModuleDef_HEAD_INIT,
    "wrapper",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_rwnmr(void) {
    import_array();  // Necessary for NumPy initialization
    return PyModule_Create(&rwnmr);
}