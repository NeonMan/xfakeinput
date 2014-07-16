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
PyObject* xfi_long_to_axis(PyObject *self, PyObject *args);

//Definition of Module methods
PyMethodDef XFIMethods[] = {
    PYTHON_METHOD_DECL(xfi_get_passthrough, "get_passthrough", "Returns the passthrough state"),
    PYTHON_METHOD_DECL(xfi_set_passthrough, "set_passthrough", "Sets the passthrough state"),
    PYTHON_METHOD_DECL(xfi_long_to_axis, "long_to_axis", "converts DInput long integer to XInput short integer"),
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

/**
 * @brief provide the passtrough array.
 */
PyObject* xfi_get_passthrough(PyObject *self, PyObject *args){ 
    PyObject* rv = PyTuple_New(4);
    for (int i = 0; i < 4; i++){
        PyTuple_SetItem(rv, i, PyBool_FromLong((long)passthrough[i]));
    }
    return rv;
}

/**
 * @brief set the passthrough array
 */
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

/**
* @brief Converts axis values from DInput ranges to XInput ranges.
*/
PyObject* xfi_long_to_axis(PyObject *self, PyObject *args){
    PyObject* f_param = 0;
    //Function has only one parameter
    if (!PyArg_UnpackTuple(args, "passthrough", 1, 1, &f_param))
        return NULL;
    //It is a Long
    if (!PyLong_Check(f_param)){
        PyErr_SetString(PyExc_RuntimeError, "Parameter must be a long integer");
        return NULL;
    }
    LONG v = PyLong_AsLong(f_param);
    LONG shortened_axis = (v & 0x0000FFFF) - 32768;
    return PyLong_FromLong(((SHORT)shortened_axis) == -32768 ? -32767 : ((SHORT)shortened_axis));
}

/**
 * @brief get the pad state from python.
 * @note make error detection and reporting more robust.
 */
DWORD py_GetState(DWORD dwUserIndex, x_original::XINPUT_STATE *pState_new){
    PyObject* mod_name = PyUnicode_FromString("xfakeinput");
    PyObject* module = PyImport_Import(mod_name);
    Py_DecRef(mod_name);
    PyObject* function = PyObject_GetAttrString(module, "get_state");
    Py_DecRef(module);

    //Get the function
    if (function == NULL){
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    //Call the function
    if (!PyCallable_Check(function)){
        Py_DecRef(function);
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    //Prepare the arguments, one integer (dwUserIndex)
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(dwUserIndex));

    //Make the call
    PyObject* py_return = PyObject_CallObject(function, pArgs);

    //Must be a dictionary
    if (!PyDict_Check(py_return)){
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    //Get the contents
    pState_new->Gamepad = initial_state.Gamepad;
    // -- TriggerLeft
    PyObject* value = PyDict_GetItemString(py_return, "TriggerL"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.bLeftTrigger = PyLong_AsLong(value);
    // -- TriggerRight
    value = PyDict_GetItemString(py_return, "TriggerR"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.bRightTrigger = PyLong_AsLong(value);
    // -- ThumbLX
    value = PyDict_GetItemString(py_return, "ThumbLX"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.sThumbLX = PyLong_AsLong(value);
    // -- ThumbLY
    value = PyDict_GetItemString(py_return, "ThumbLY"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.sThumbLY = PyLong_AsLong(value);
    // -- ThumbRX
    value = PyDict_GetItemString(py_return, "ThumbRX"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.sThumbRX = PyLong_AsLong(value);
    // -- ThumbRY
    value = PyDict_GetItemString(py_return, "ThumbRY"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.sThumbRY = PyLong_AsLong(value);
    //Buttons
    // -- A
    value = PyDict_GetItemString(py_return, "ButtonA"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_A;
    // -- B
    value = PyDict_GetItemString(py_return, "ButtonB"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_B;
    // -- X
    value = PyDict_GetItemString(py_return, "ButtonX"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_X;
    // -- Y
    value = PyDict_GetItemString(py_return, "ButtonY"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_Y;
    // -- Start
    value = PyDict_GetItemString(py_return, "ButtonStart"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_START;
    // -- Back
    value = PyDict_GetItemString(py_return, "ButtonBack"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_BACK;
    // -- Up
    value = PyDict_GetItemString(py_return, "ButtonU"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
    // -- Down
    value = PyDict_GetItemString(py_return, "ButtonD"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
    // -- Left
    value = PyDict_GetItemString(py_return, "ButtonL"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
    // -- Right
    value = PyDict_GetItemString(py_return, "ButtonR"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
    // -- LThumb
    value = PyDict_GetItemString(py_return, "ButtonLThumb"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_THUMB;
    // -- RThumb
    value = PyDict_GetItemString(py_return, "ButtonRThumb"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_THUMB;
    // -- LShoulder
    value = PyDict_GetItemString(py_return, "ButtonLShoulder"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
    // -- RShoulder
    value = PyDict_GetItemString(py_return, "ButtonRShoulder"); //Borrowed reference
    if (value != NULL)
        pState_new->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;

    Py_DecRef(py_return);
    return ERROR_SUCCESS;
}

/**
* @brief send state to python.
*/
DWORD py_SetState(DWORD dwUserIndex, x_original::XINPUT_VIBRATION *pVibration){
    return ERROR_SUCCESS;
}
