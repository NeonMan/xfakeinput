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
#include <Windows.h>
#include <iostream>
using namespace std;

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// --- Globals ---
LPDIRECTINPUT8 di;
#define MAX_JOY 32
LPDIRECTINPUTDEVICE8 joysticks[MAX_JOY];
int joy_count = 0;
// ---------------

BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    if (joy_count < MAX_JOY){
        hr = di->CreateDevice(instance->guidInstance, &joysticks[joy_count], NULL);
        // If it failed, then we can't use this joystick. (Maybe the user unplugged
        // it while we were in the middle of enumerating it.)
        if (FAILED(hr)) {
            return DIENUM_CONTINUE;
        }
        else
            joy_count++;
    }
    else
        cerr << "Maximum device count reached, tell Neon to update MAX_JOY" << endl;

    return DIENUM_CONTINUE;
}

int main(){
    //Clear arrays
    memset(joysticks, 0, sizeof(LPDIRECTINPUTDEVICE8)* MAX_JOY);
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
    // Show each device info
    cout << "Found: " << joy_count << " devices." << endl;
    for (int i = 0; i < joy_count; i++){
        //Configure device
        hr = joysticks[i]->SetDataFormat(&c_dfDIJoystick2);
        hr = joysticks[i]->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        hr = joysticks[i]->Acquire();
        //Extract info
        DIDEVICEINSTANCEW didi;
        didi.dwSize = sizeof(DIDEVICEINSTANCEW);
        hr = joysticks[i]->GetDeviceInfo(&didi);
        DIDEVCAPS capabilities;
        capabilities.dwSize = sizeof(DIDEVCAPS);
        hr = joysticks[i]->GetCapabilities(&capabilities);
        cout  <<  "  Device #" << i << ":" << endl;
        wcout << L"        Name: " << didi.tszProductName << endl;
        wcout << L"    Instance: " << didi.tszInstanceName << endl;
        wcout << L"        Axes: " << capabilities.dwAxes << endl;
        wcout << L"     Buttons: " << capabilities.dwButtons << endl;
        //Poll current state
        hr = joysticks[i]->Poll();
        if ((hr != DI_OK) && (hr != DI_NOEFFECT)){
            wcerr << L"Error while polling: " << didi.tszInstanceName << endl;
        }
        DIJOYSTATE2 joy_stat;
        if (joysticks[i]->GetDeviceState(sizeof(DIJOYSTATE2), &joy_stat) == DI_OK){
            cout << "    ---- Polled state ----" << endl;
            cout << "          X: " << joy_stat.lX << endl;
            cout << "          Y: " << joy_stat.lY << endl;
            cout << "          Z: " << joy_stat.lZ << endl;
            cout << "         rX: " << joy_stat.lRx << endl;
            cout << "         rY: " << joy_stat.lRy << endl;
            cout << "         rZ: " << joy_stat.lRz << endl;
            cout << "    lSlider: " << joy_stat.rglSlider[0] << ", " << joy_stat.rglSlider[1] << endl;
            cout << "       POVs: " << joy_stat.rgdwPOV[0] << ", " << joy_stat.rgdwPOV[1] << ", " << joy_stat.rgdwPOV[2] << ", " << joy_stat.rgdwPOV[3] << endl;
            cout << "    Buttons: ";
            for (DWORD j = 0; j < capabilities.dwButtons; j++){
                if (joy_stat.rgbButtons[j])
                    cout << "1";
                else
                    cout << "0";
                if (j < capabilities.dwButtons - 1)
                    cout << ", ";
            }
            cout << endl;
        }
        else{

        }

        joysticks[i]->Release();
    }
    MessageBoxA(0, "End", "---", 0);
    return 0;
}