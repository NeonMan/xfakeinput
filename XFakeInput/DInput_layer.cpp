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
#include "DInput_layer.h"
#include <Windows.h>
#include <cmath>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// --- DInput layer globals ---
int iJoyCount;       ///<-- Number of DirectInput joypads
LPDIRECTINPUT8 di;   ///<-- DirectInput object
#define MAX_JOY 32
LPDIRECTINPUTDEVICE8 lpJoysticks[MAX_JOY]; ///<-- Joystick objects
DIDEVICEINSTANCEW diJoyInfos[MAX_JOY];     ///<-- Joystick info structs
DIDEVCAPS diJoyCaps[MAX_JOY];              ///<-- Joystick capabilities structs
// ----------------------------

/**
 * @brief Callback function for DirectInput device enumeration.
 */
BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    if (iJoyCount < MAX_JOY){
        hr = di->CreateDevice(instance->guidInstance, lpJoysticks + iJoyCount, NULL);
        // If it failed, then we can't use this joystick. (Maybe the user unplugged
        // it while we were in the middle of enumerating it.)
        if (FAILED(hr)) {
            return DIENUM_CONTINUE;
        }
        else
            iJoyCount++;
    }
    else{

    }
    return DIENUM_CONTINUE;
}

/**
 * @brief Initialize DirectInput devices.
 */
int dinput_init(){
    //Initialize globals
    iJoyCount = 0;
    for (int i = 0; i < MAX_JOY; i++){
        lpJoysticks[i] = 0;
    }

    //Initialize dInput
    HRESULT hr;
    if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
        IID_IDirectInput8, (VOID**)&di, NULL))) {
        return -1;
    }

    // Enumerate all dInput devices
    if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_ALL, enumCallback,
        NULL, DIEDFL_ATTACHEDONLY))) {
        return -2;
    }

    //Extract info
    for (int i = 0; i < iJoyCount; i++){
        diJoyInfos[i].dwSize = sizeof(DIDEVICEINSTANCEW);
        if (FAILED(hr = lpJoysticks[i]->GetDeviceInfo(diJoyInfos + i))){
            return -6;
        }
    }

    //Configure (set data format)
    for (int i = 0; i < iJoyCount; i++){
        if ((diJoyInfos[i].dwDevType & 0x00FF) == DI8DEVTYPE_KEYBOARD){
            //Configure as keyboard
            if (FAILED(hr = lpJoysticks[i]->SetDataFormat(&c_dfDIKeyboard))){
                return -3;
            }
        }
        else if ((diJoyInfos[i].dwDevType & 0x00FF) == DI8DEVTYPE_MOUSE){
            //Configure as mouse
            if (FAILED(hr = lpJoysticks[i]->SetDataFormat(&c_dfDIMouse2))){
                return -3;
            }
        }
        else{
            //Otherwise, configure like a game device (joystick)
            if (FAILED(hr = lpJoysticks[i]->SetDataFormat(&c_dfDIJoystick2))){
                return -3;
            }
        }
    }

    //Configure (coop level)
    for (int i = 0; i < iJoyCount; i++){
        if (FAILED(hr = lpJoysticks[i]->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))){
            return -4;
        }
    }

    //Acquire
    for (int i = 0; i < iJoyCount; i++){
        if (FAILED(hr = lpJoysticks[i]->Acquire())){
            return -5;
        }
    }

    //Extract capabilities
    for (int i = 0; i < iJoyCount; i++){
        diJoyCaps[i].dwSize = sizeof(DIDEVCAPS);
        if (FAILED(hr = lpJoysticks[i]->GetCapabilities(diJoyCaps + i))){
            return -7;
        }
    }
    return iJoyCount;
}
