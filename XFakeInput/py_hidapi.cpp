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
#include "py_hidapi.h"
#include "hidapi/hidapi.h"

//Forward declaration of python exported functions
PyObject* hidapi_hid_init(PyObject* self, PyObject* args);                /* int (void) */
PyObject* hidapi_hid_exit(PyObject* self, PyObject* args);                /* int (void) */
PyObject* hidapi_hid_enumerate(PyObject* self, PyObject* args);           /* hid_dev_info (vid, pid) */
PyObject* hidapi_hid_free_enumeration(PyObject* self, PyObject* args);    /* void (hid_dev_info) */
PyObject* hidapi_hid_open(PyObject* self, PyObject* args);                /* hid_dev (vid, pid, serial) */
PyObject* hidapi_hid_open_path(PyObject* self, PyObject* args);           /* hid_dev (path) */
PyObject* hidapi_hid_write(PyObject* self, PyObject* args);               /* int (hid_dev, data, len) */
PyObject* hidapi_hid_read_timeout(PyObject* self, PyObject* args);        /* int (hid_dev, data, len, timeout) */
PyObject* hidapi_hid_read(PyObject* self, PyObject* args);                /* int (hid_dev, data, len, timeout) */
PyObject* hidapi_hid_set_nonblocking(PyObject* self, PyObject* args);     /* int (hid_dev, nonblock) */
PyObject* hidapi_hid_send_feature_report(PyObject* self, PyObject* args); /*int (hid_dev, data, len) */
PyObject* hidapi_hid_get_feature_report(PyObject* self, PyObject* args);  /*int (hid_dev, data, len) */
PyObject* hidapi_hid_close(PyObject* self, PyObject* args);               /* void (hid_dev) */
PyObject* hidapi_hid_get_product_str(PyObject* self, PyObject* args);       /* int (hid_dev, str, maxlen) */
PyObject* hidapi_hid_get_serial_number_str(PyObject* self, PyObject* args); /* int (hid_dev, str, maxlen) */
PyObject* hidapi_hid_get_manufacturer_str(PyObject* self, PyObject* args);  /* int (hid_dev, str, maxlen) */
PyObject* hidapi_hid_get_indexed_str(PyObject* self, PyObject* args);       /* int (hid_dev, str, maxlen) */

//Module method declarations
PyMethodDef HidapiMethods[] = {
    PYTHON_METHOD_DECL(hidapi_hid_init, "__init", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_exit, "__exit", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_enumerate, "__enumerate", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_free_enumeration, "__free_enumeration", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_open, "__open", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_open_path, "__open_path", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_write, "__write", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_read_timeout, "__read_timeout", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_read, "__read", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_set_nonblocking, "__set_nonblocking", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_send_feature_report, "__send_feature_report", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_get_feature_report, "__get_feature_report", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_close, "__close", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_get_product_str, "__get_product_string", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_get_serial_number_str, "__get_serial_number_string", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_get_manufacturer_str, "__get_manufacturer_string", "FILLME"),
    PYTHON_METHOD_DECL(hidapi_hid_get_indexed_str, "__get_indexed_string", "FILLME"),
    PYTHON_END_METHOD_DECL
};

//Definition of DInput module
PyModuleDef HidapiModule = {
    PyModuleDef_HEAD_INIT, "hidapi", NULL, -1, HidapiMethods,
    NULL, NULL, NULL, NULL
};

///Initialization function
PyObject* PyInit_hidapi(){
    return PyModule_Create(&HidapiModule);
}

// ------------------------------------------
// --- Implemenation of package functions ---
// ------------------------------------------

PyObject* hidapi_hid_init(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_exit(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_enumerate(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_free_enumeration(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_open(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_open_path(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_write(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_read_timeout(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_read(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_set_nonblocking(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_send_feature_report(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_get_feature_report(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_close(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_get_product_str(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_get_serial_number_str(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_get_manufacturer_str(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}

PyObject* hidapi_hid_get_indexed_str(PyObject* self, PyObject* args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}
