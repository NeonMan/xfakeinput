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
PyObject* di_get_joystate2(PyObject *self, PyObject *args);

//Definition of Module methods
PyMethodDef DInputMethods[] =  {
    PYTHON_METHOD_DECL(di_num_devices, "num_devices", "Number of Detected DirectInput devices"),
    PYTHON_METHOD_DECL(di_device_info, "device_info", "Information regarding the selected device"),
    PYTHON_METHOD_DECL(di_device_poll, "device_poll", "Poll the device"),
    PYTHON_METHOD_DECL(di_get_joystate2, "get_joystate2", "Obtain the device state (DIJOYSTATE2)"),
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

/**
 * @brief call the device's poll method.
 */
PyObject* di_device_poll(PyObject *self, PyObject *args){
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
    return PyLong_FromLong(lpJoysticks[device_number]->Poll());
}

/**
 *@brief provide a dictionary containing the device's DIJOYSTATE2 struct.
 */
PyObject* di_get_joystate2(PyObject *self, PyObject *args){
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

    //Get device state
    DIJOYSTATE2 state;
    HRESULT rv;
    rv = lpJoysticks[device_number]->GetDeviceState(sizeof(DIJOYSTATE2), &state);
    if (rv != DI_OK){
        switch (rv){
        case DIERR_INPUTLOST:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Input lost");
            return NULL;
        case DIERR_INVALIDPARAM:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Invalid param");
            return NULL;
        case DIERR_NOTACQUIRED:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Not acquired");
            return NULL;
        case DIERR_NOTINITIALIZED:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Not initialized");
            return NULL;
        case E_PENDING:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Pending");
            return NULL;
        default:
            PyErr_SetString(PyExc_IOError, "DirectInput error: Unknown");
            return NULL;
        }
    }

    //Build the dictionary
    PyObject* dict_state = PyDict_New();
    PyObject* value;

    //AngularAccelerationRX
    value = PyLong_FromLong(state.lARx);
    PyDict_SetItemString(dict_state, "ARx", value);
    Py_DecRef(value);

    //AngularAccelerationRY
    value = PyLong_FromLong(state.lARy);
    PyDict_SetItemString(dict_state, "ARy", value);
    Py_DecRef(value);

    //AngularAccelerationRZ
    value = PyLong_FromLong(state.lARz);
    PyDict_SetItemString(dict_state, "ARz", value);
    Py_DecRef(value);

    //AxisAccelX
    value = PyLong_FromLong(state.lAX);
    PyDict_SetItemString(dict_state, "AX", value);
    Py_DecRef(value);

    //AxisAccelY
    value = PyLong_FromLong(state.lAY);
    PyDict_SetItemString(dict_state, "AY", value);
    Py_DecRef(value);

    //AxisAccelZ
    value = PyLong_FromLong(state.lAZ);
    PyDict_SetItemString(dict_state, "AZ", value);
    Py_DecRef(value);

    //ForceRX
    value = PyLong_FromLong(state.lFRx);
    PyDict_SetItemString(dict_state, "FRx", value);
    Py_DecRef(value);

    //ForceRY
    value = PyLong_FromLong(state.lFRy);
    PyDict_SetItemString(dict_state, "FRy", value);
    Py_DecRef(value);

    //ForceRZ
    value = PyLong_FromLong(state.lFRz);
    PyDict_SetItemString(dict_state, "FRz", value);
    Py_DecRef(value);

    //ForceX
    value = PyLong_FromLong(state.lFX);
    PyDict_SetItemString(dict_state, "FX", value);
    Py_DecRef(value);

    //ForceY
    value = PyLong_FromLong(state.lFY);
    PyDict_SetItemString(dict_state, "FY", value);
    Py_DecRef(value);

    //ForceZ
    value = PyLong_FromLong(state.lFX);
    PyDict_SetItemString(dict_state, "FZ", value);
    Py_DecRef(value);

    //RotationX
    value = PyLong_FromLong(state.lFRx);
    PyDict_SetItemString(dict_state, "Rx", value);
    Py_DecRef(value);

    //RotationY
    value = PyLong_FromLong(state.lFRy);
    PyDict_SetItemString(dict_state, "Ry", value);
    Py_DecRef(value);

    //RotationZ
    value = PyLong_FromLong(state.lFRz);
    PyDict_SetItemString(dict_state, "Rz", value);
    Py_DecRef(value);

    //AngularVelocityX
    value = PyLong_FromLong(state.lVRx);
    PyDict_SetItemString(dict_state, "VRx", value);
    Py_DecRef(value);

    //AngularVelocityY
    value = PyLong_FromLong(state.lVRy);
    PyDict_SetItemString(dict_state, "VRy", value);
    Py_DecRef(value);

    //AngularVelocityZ
    value = PyLong_FromLong(state.lVRz);
    PyDict_SetItemString(dict_state, "VRz", value);
    Py_DecRef(value);

    //VelocityX
    value = PyLong_FromLong(state.lVX);
    PyDict_SetItemString(dict_state, "VX", value);
    Py_DecRef(value);

    //VelocityY
    value = PyLong_FromLong(state.lVY);
    PyDict_SetItemString(dict_state, "VY", value);
    Py_DecRef(value);

    //VelocityZ
    value = PyLong_FromLong(state.lVZ);
    PyDict_SetItemString(dict_state, "VZ", value);
    Py_DecRef(value);

    //X-Axis
    value = PyLong_FromLong(state.lX);
    PyDict_SetItemString(dict_state, "X", value);
    Py_DecRef(value);

    //Y-Axis
    value = PyLong_FromLong(state.lY);
    PyDict_SetItemString(dict_state, "Y", value);
    Py_DecRef(value);

    //Z-Axis
    value = PyLong_FromLong(state.lZ);
    PyDict_SetItemString(dict_state, "Z", value);
    Py_DecRef(value);

    //Buttons array
    value = PyTuple_New(128);
    for (int i = 0; i < 128; i++){
        PyObject* button = PyBool_FromLong(state.rgbButtons[i]);
        PyTuple_SetItem(value, i, button);
        //Py_DecRef(button); //<-- not needed, setItem 'steals' a reference
    }
    PyDict_SetItemString(dict_state, "Buttons", value);
    Py_DecRef(value);

    ///@note Add aditional axis/accelerations/velocities/forces if requested.
    
    return dict_state;
}