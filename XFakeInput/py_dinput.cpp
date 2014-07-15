/*
Copyright (c) 2014, Juan Luis �lvarez Mart�nez
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

/**
 * @brief Provide the number of configured DirectInput devices
 */
PyObject* di_num_devices(PyObject *self, PyObject *args){
    PyObject* v = PyLong_FromLong(iJoyCount);
    return v;
}

/**
 * @brief Provide a dictionary with the extracted DirectInput information.
 */
PyObject* di_device_info(PyObject *self, PyObject *args){
    PyObject* po_number;
    //Function has only one parameter (integer)
    if (!PyArg_UnpackTuple(args, "device_number", 1, 1, &po_number))
        return NULL;
    //Check if it is an integer
    if (!PyLong_Check(po_number)){
        PyErr_SetString(PyExc_RuntimeError, "Parameter must be an integer");
        return NULL;
    }
    //Extract the value
    long device_number = PyLong_AsLong(po_number);
    //Device number in range?
    if ((device_number >= iJoyCount) || (device_number < 0)){
        PyErr_SetString(PyExc_IndexError, "Device index out of bounds");
        return NULL;
    }
    //Build a dictionary with the device info
    PyObject* info_dict = PyDict_New();
    PyObject* value;

    //dwDevType
    value = PyLong_FromLong(diJoyInfos[device_number].dwDevType);
    PyDict_SetItemString(info_dict, "DevType", value);
    Py_DecRef(value);

    //dwSize
    value = PyLong_FromLong(diJoyInfos[device_number].dwSize);
    PyDict_SetItemString(info_dict, "Size", value);
    Py_DecRef(value);

    //tszInstanceName
    value = PyUnicode_FromWideChar(diJoyInfos[device_number].tszInstanceName, -1);
    PyDict_SetItemString(info_dict, "InstanceName", value);
    Py_DecRef(value);

    //tszName
    value = PyUnicode_FromWideChar(diJoyInfos[device_number].tszProductName, -1);
    PyDict_SetItemString(info_dict, "ProductName", value);
    Py_DecRef(value);

    //wUsage
    value = PyLong_FromLong(diJoyInfos[device_number].wUsage);
    PyDict_SetItemString(info_dict, "Usage", value);
    Py_DecRef(value);

    //wUsagePage
    value = PyLong_FromLong(diJoyInfos[device_number].wUsagePage);
    PyDict_SetItemString(info_dict, "UsagePage", value);
    Py_DecRef(value);

    return info_dict;
}

PyObject* di_device_poll(PyObject *self, PyObject *args){
    PyErr_SetString(PyExc_RuntimeError, "Unimplemented");
    return NULL;
}
