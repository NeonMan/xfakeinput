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
#pragma once
#ifndef __XFakeInput_h
#define __XFakeInput_h

//Version ID definitions
#define FXINPUT_V1_1   11
#define FXINPUT_V1_2   12
#define FXINPUT_V1_3   13
#define FXINPUT_V1_4   14
#define FXINPUT_V9_1_0 910

#include <Windows.h>
#include <stdint.h>

//Prevent collisions with original XInput DLL
namespace x_original{
#include <Xinput.h>
}

// -----------------
// --- Constants ---
// -----------------
///Capabilities to return when queried. Copied from an actual xbox360 pad at rest.
const x_original::XINPUT_CAPABILITIES default_capabilities{
    (BYTE) 0x1,
    (BYTE) 0x1,
    (WORD) 0x4,
    {
        /*wButtons*/      (WORD)  0xF3FF,
        /*bLeftTrigger*/  (BYTE)  0x1,
        /*bRightTrigger*/ (BYTE)  0x1,
        /*sThumbLX*/      (SHORT) 0x1,
        /*sThumbLY*/      (SHORT) 0x1,
        /*sThumbRX*/      (SHORT) 0x1,
        /*sThumbRY*/      (SHORT) 0x1
    },
    {
        /*wLeftMotorSpeed*/  (WORD) 0x1,
        /*wRightMotorSpeed*/ (WORD) 0x1
    }
};

///Initial pad state, from a real pad at rest
const x_original::XINPUT_STATE initial_state{
    (DWORD)0x1000, /*dwPacketNumber*/
    {
        (WORD)  0,
        (BYTE)  0,
        (BYTE)  0,
        (SHORT) 3580,
        (SHORT) -4118,
        (SHORT) -3524,
        (SHORT) 329
    }
};

// --------------------------
// --- XFakeInput methods ---
// --------------------------
void fake_Init(DWORD version);

void fake_XInputEnable(
    BOOL enable
    );

DWORD fake_XInputGetCapabilities(
    DWORD dwUserIndex,
    DWORD dwFlags,
    x_original::XINPUT_CAPABILITIES *pCapabilities,
    DWORD caller_version
    );

DWORD fake_XInputGetDSoundAudioDeviceGuids(
    DWORD dwUserIndex,
    GUID* pDSoundRenderGuid,
    GUID* pDSoundCaptureGuid,
    DWORD caller_version
    );

DWORD fake_XInputGetState(
    DWORD dwUserIndex,
    x_original::XINPUT_STATE *pState,
    DWORD caller_version
    );

DWORD fake_XInputSetState(
    DWORD dwUserIndex,
    x_original::XINPUT_VIBRATION *pVibration,
    DWORD caller_version
    );

DWORD fake_XInputGetKeystroke(
    DWORD dwUserIndex,
    DWORD dwReserved,
    x_original::PXINPUT_KEYSTROKE pKeystroke,
    DWORD caller_version);

DWORD fake_XInputGetBatteryInformation(
    DWORD dwUserIndex,
    BYTE devType,
    x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation,
    DWORD caller_version);

DWORD fake_XInputGetAudioDeviceIds(
    DWORD dwUserIndex,
    LPWSTR pRenderDeviceId,
    UINT *pRenderCount,
    LPWSTR pCaptureDeviceId,
    UINT *pCaptureCount,
    DWORD caller_version
    );

#endif