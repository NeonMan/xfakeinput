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
#include "py_utils.h"
#include <Python.h>
#include <Windows.h>

//Forward declaration of python exported functions
PyObject* util_get_ticks(PyObject *self, PyObject *args);

//Module method declarations
PyMethodDef UtilMethods[] = {
    PYTHON_METHOD_DECL(util_get_ticks, "get_ticks", "Miliseconds since the system staerted up"),
    PYTHON_END_METHOD_DECL
};

//Definition of DInput module
PyModuleDef UtilModule = {
    PyModuleDef_HEAD_INIT, "util", NULL, -1, UtilMethods,
    NULL, NULL, NULL, NULL
};

///Initialization function
PyObject* PyInit_util(void){
    return PyModule_Create(&UtilModule);
}

// ----------------------------
// --- Function definitions ---
// ----------------------------

/**
 * @brief Returns the ms since the system booted.
 */
PyObject* util_get_ticks(PyObject *self, PyObject *args){
    if (!PyArg_UnpackTuple(args, "get_ticks", 0, 0)){
        return NULL;
    }
    return PyLong_FromLong(GetTickCount());
}
