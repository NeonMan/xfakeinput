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
#include "py_dinput.h"

//Forward declaration of exported functions
PyObject* di_num_devices(PyObject *self, PyObject *args);
PyObject* di_device_info(PyObject *self, PyObject *args);
PyObject* di_device_poll(PyObject *self, PyObject *args);

//Definition of Module methods
PyMethodDef DInputMethods[] =  {
    PYTHON_METHOD_DECL(di_num_devices, "num_devices", "Number of Detected DirectInput devices"),
    PYTHON_METHOD_DECL(di_device_info, "device_info", "Information regarding the selected device"),
    PYTHON_METHOD_DECL(di_device_poll, "device_poll", "Get current device state"),
    PYTHON_END_METHOD_DECL
};

//Definition of DInput module
PyModuleDef DInputModule = {
    PyModuleDef_HEAD_INIT, "dinput", NULL, -1, DInputMethods,
    NULL, NULL, NULL, NULL
};

///Initialization function
PyObject* PyInit_dinput(void){
    return PyModule_Create(&DInputModule);
}

// ----------------------------
// --- Function definitions ---
// ----------------------------
PyObject* di_num_devices(PyObject *self, PyObject *args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* di_device_info(PyObject *self, PyObject *args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* di_device_poll(PyObject *self, PyObject *args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}