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

DllMain                         0x00402bc0    0x00002bc0    1 (0x1)
XInputEnable                    0x00403500    0x00003500    2 (0x2)
XInputGetCapabilities           0x00403300    0x00003300    3 (0x3)
XInputGetDSoundAudioDeviceGuids 0x004035c0    0x000035c0    4 (0x4)
XInputGetState                  0x00402ea0    0x00002ea0    5 (0x5)
XInputSetState                  0x00403160    0x00003160    6 (0x6)
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
        fake_Init(FXINPUT_V1_2);
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
    return fake_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities, FXINPUT_V1_2);
}

DWORD WINAPI XInputGetDSoundAudioDeviceGuids(
    DWORD dwUserIndex,
    GUID* pDSoundRenderGuid,
    GUID* pDSoundCaptureGuid
    ){
    return fake_XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid, FXINPUT_V1_2);
}

DWORD WINAPI XInputGetState(
    _In_   DWORD dwUserIndex,
    _Out_  x_original::XINPUT_STATE *pState

    ){
    return fake_XInputGetState(dwUserIndex, pState, FXINPUT_V1_2);
}

DWORD WINAPI XInputSetState(
    _In_     DWORD dwUserIndex,
    _Inout_  x_original::XINPUT_VIBRATION *pVibration
    ){
    return fake_XInputSetState(dwUserIndex, pVibration, FXINPUT_V1_2);
}