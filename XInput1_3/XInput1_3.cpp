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
/*
DLL Exports:

DllMain                            0x00402780    0x00002780    1 (0x1)
XInputEnable                    0x00403190    0x00003190    5 (0x5)
XInputGetBatteryInformation     0x00403380    0x00003380    7 (0x7)
XInputGetCapabilities           0x00402f60    0x00002f60    4 (0x4)
XInputGetDSoundAudioDeviceGuids    0x00403ad0    0x00003ad0    6 (0x6)
XInputGetKeystroke                0x00403e30    0x00003e30    8 (0x8)
XInputGetState                    0x00402980    0x00002980    2 (0x2)
XInputSetState                  0x00402db0    0x00002db0    3 (0x3)
*/
#include <Windows.h>
#include <stdint.h>
#include "../XFakeInput/XFakeInput.h"
namespace x_original{
#include <Xinput.h>
}

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        fake_Init(FXINPUT_V1_3);
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
        fake_Cleanup();
    return TRUE;
}

void WINAPI XInputEnable(
    _In_  BOOL enable
    ){
    fake_XInputEnable(enable);
}

DWORD WINAPI XInputGetCapabilities(
    _In_   DWORD dwUserIndex,
    _In_   DWORD dwFlags,
    _Out_  x_original::XINPUT_CAPABILITIES *pCapabilities

    ){
    return fake_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities, FXINPUT_V1_3);
}

DWORD WINAPI XInputGetDSoundAudioDeviceGuids(
    DWORD dwUserIndex,
    GUID* pDSoundRenderGuid,
    GUID* pDSoundCaptureGuid
    ){
    return fake_XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid, FXINPUT_V1_3);
}

DWORD WINAPI XInputGetState(
    _In_   DWORD dwUserIndex,
    _Out_  x_original::XINPUT_STATE *pState

    ){
    return fake_XInputGetState(dwUserIndex, pState, FXINPUT_V1_3);
}

DWORD WINAPI XInputSetState(
    _In_     DWORD dwUserIndex,
    _Inout_  x_original::XINPUT_VIBRATION *pVibration
    ){
    return fake_XInputSetState(dwUserIndex, pVibration, FXINPUT_V1_3);
}

DWORD WINAPI XInputGetKeystroke(
    DWORD dwUserIndex,
    DWORD dwReserved,
    x_original::PXINPUT_KEYSTROKE pKeystroke){
    return fake_XInputGetKeystroke(dwUserIndex, dwReserved, pKeystroke, FXINPUT_V1_3);
}

DWORD WINAPI XInputGetBatteryInformation(
    _In_   DWORD dwUserIndex,
    _In_   BYTE devType,
    _Out_  x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation
    ){
    return fake_XInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation, FXINPUT_V1_3);
}
