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
/// @note Select DirectInput version
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
    if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback,
        NULL, DIEDFL_ATTACHEDONLY))) {
        return -2;
    }

    //Why not a single for loop? Readability, maybe...
    //Configure (set data format)
    for (int i = 0; i < iJoyCount; i++){
        if ( FAILED(hr = lpJoysticks[i]->SetDataFormat(&c_dfDIJoystick2)) ){
            return -3;
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

    //Extract info
    for (int i = 0; i < iJoyCount; i++){
        diJoyInfos[i].dwSize = sizeof(DIDEVICEINSTANCEW);
        if (FAILED(hr = lpJoysticks[i]->GetDeviceInfo(diJoyInfos + i))){
            return -6;
        }
    }

    //Extract capabilities
    for (int i = 0; i < iJoyCount; i++){
        diJoyCaps[i].dwSize = sizeof(DIDEVCAPS);
        if (FAILED(hr = lpJoysticks[i]->GetCapabilities(diJoyCaps + i))){
            return -7;
        }
    }   

    //Load configuration
    ///@ToDo load DInput-to-XInput mappings
    return 0;
}

/**
 * @brief Converts axis values from DInput ranges to XInput ranges.
 * 
 * @param v DirectInput axis value [0,65535]
 * @return XInput axis value [–32768,32767]
 */
inline SHORT convert_axis(LONG v){
    LONG shortened_axis = (v & 0x0000FFFF) - 32768;
    return ((SHORT)shortened_axis) == -32768 ? -32767 : ((SHORT)shortened_axis);
}

/**
 * @brief Converts a POV value to XInput DPAD
 *
 * @param pov a DInput POV value
 * @return A button mask
 */
inline WORD  convert_POV(DWORD pov){
    if (pov <= 3000)
        return XINPUT_GAMEPAD_DPAD_UP;
    else if (pov <= 6000)
        return XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_RIGHT;
    else if (pov <= 12000)
        return XINPUT_GAMEPAD_DPAD_RIGHT;
    else if (pov <= 15000)
        return XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_DOWN;
    else if (pov <= 21000)
        return XINPUT_GAMEPAD_DPAD_DOWN;
    else if (pov <= 24000)
        return XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT;
    else if (pov <= 30000)
        return XINPUT_GAMEPAD_DPAD_LEFT;
    else if (pov <= 33000)
        return XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_UP;
    else if (pov <= 35900)
        return XINPUT_GAMEPAD_DPAD_UP;
    else
        return 0;
}
/**
 * @brief Translate a directInput device to XInput struct
 * @fixme Hardcoded to my current gamepad.
 */
DWORD dinput_XInputGetState(
    DWORD dwUserIndex,
    x_original::XINPUT_STATE *pState_new
    ){
    if (dwUserIndex != 0)
        return ERROR_DEVICE_NOT_CONNECTED;

    //Poll pad state
    lpJoysticks[0]->Poll();
    DIJOYSTATE2 joy_stat;
    lpJoysticks[0]->GetDeviceState(sizeof(DIJOYSTATE2), &joy_stat);
    //Make the DInput --> XInput conversion

    //Axis: X(RX),Y(RY),Z(LX),rZ(LY)
    pState_new->Gamepad.sThumbLX = convert_axis(joy_stat.lX);
    pState_new->Gamepad.sThumbLY = - convert_axis(joy_stat.lY);
    pState_new->Gamepad.sThumbRX = convert_axis(joy_stat.lZ);
    pState_new->Gamepad.sThumbRY = - convert_axis(joy_stat.lRz);

    //Buttons: A  B  X  Y  Ba  St  LS  RS  LB  RB  LT  RT
    //BTN #:   2  3  1  4  9   10  5   6   11  12  7   8
    pState_new->Gamepad.bLeftTrigger = joy_stat.rgbButtons[6] ? 255 : 0;
    pState_new->Gamepad.bRightTrigger = joy_stat.rgbButtons[7] ? 255 : 0;
    pState_new->Gamepad.wButtons = 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[1] ? XINPUT_GAMEPAD_A : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[2] ? XINPUT_GAMEPAD_B : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[0] ? XINPUT_GAMEPAD_X : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[3] ? XINPUT_GAMEPAD_Y : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[8] ? XINPUT_GAMEPAD_BACK : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[9] ? XINPUT_GAMEPAD_START : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[4] ? XINPUT_GAMEPAD_LEFT_SHOULDER : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[5] ? XINPUT_GAMEPAD_RIGHT_SHOULDER : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[10] ? XINPUT_GAMEPAD_LEFT_THUMB : 0;
    pState_new->Gamepad.wButtons |= joy_stat.rgbButtons[11] ? XINPUT_GAMEPAD_RIGHT_THUMB : 0;
    pState_new->Gamepad.wButtons |= convert_POV(joy_stat.rgdwPOV[0]);
    return ERROR_SUCCESS;
}

/**
 * @brief Accept data from XInput (does nothing right now...)
 */
DWORD dinput_XInputSetState(
    DWORD dwUserIndex,
    x_original::XINPUT_VIBRATION *pVibration
    ){
    if (dwUserIndex != 0)
        return ERROR_DEVICE_NOT_CONNECTED;
    else
        return ERROR_SUCCESS;
}