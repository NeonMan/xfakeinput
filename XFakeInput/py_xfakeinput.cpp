/*
Copyright (c) 2014, Juan Luis Álvarez Martínez
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "py_common.h"
#include "py_xfakeinput.h"

//Forward declaration of exported functions
PyObject* xfi_get_passthrough(PyObject *self, PyObject *args);
PyObject* xfi_set_passthrough(PyObject *self, PyObject *args);

//Definition of Module methods
PyMethodDef XFIMethods[] = {
    PYTHON_METHOD_DECL(xfi_get_passthrough, "get_passthrough", "Returns the passthrough state"),
    PYTHON_METHOD_DECL(xfi_set_passthrough, "set_passthrough", "Sets the passthrough state"),
    PYTHON_END_METHOD_DECL
};

//Definition of DInput module
PyModuleDef XFIModule = {
    PyModuleDef_HEAD_INIT, "xfi", NULL, -1, XFIMethods,
    NULL, NULL, NULL, NULL
};

///Initialization function
PyObject* PyInit_xfi(void){
    return PyModule_Create(&XFIModule);
}

//Definition of functions
PyObject* xfi_get_passthrough(PyObject *self, PyObject *args){ 
    PyObject* rv = PyTuple_New(4);
    for (int i = 0; i < 4; i++){
        PyTuple_SetItem(rv, i, PyBool_FromLong((long)passthrough[i]));
    }
    return rv;
}

PyObject* xfi_set_passthrough(PyObject *self, PyObject *args){
    PyObject* f_param = 0;
    //Function has only one parameter
    if(!PyArg_UnpackTuple(args, "passthrough", 1, 1, &f_param))
        return NULL;
    //The parameter is a tuple
    if (!PyTuple_Check(f_param)){
        PyErr_SetString(PyExc_RuntimeError, "Parameter must be a tuple");
        return NULL;
    }
    else{
        //Size >= 4
        if (PyTuple_Size(f_param) < 4){
            PyErr_SetString(PyExc_RuntimeError, "Tuple must have at least 4 elements");
            return NULL;
        }
    }
    //Write back the values contained in the tuple
    for (int i = 0; i < 4; i++){
        PyObject* v = PyTuple_GetItem(f_param, i);
        passthrough[i] = PyObject_IsTrue(v);
    }

    //Return success
    Py_RETURN_NONE;
}