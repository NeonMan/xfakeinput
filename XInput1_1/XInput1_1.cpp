/*
DLL Exports:

DllMain                         0x00402bc0    0x00002bc0    1 (0x1)
XInputEnable                    0x004034a0    0x000034a0    2 (0x2)
XInputGetCapabilities           0x004032a0    0x000032a0    3 (0x3)
XInputGetDSoundAudioDeviceGuids 0x00403560    0x00003560    4 (0x4)
XInputGetState                  0x00402ea0    0x00002ea0    5 (0x5)
XInputSetState                  0x00403100    0x00003100    6 (0x6)
*/
#include <Windows.h>
#include <stdint.h>
#include "../XFakeInput/XFakeInput.h"
namespace x_original{
#include <Xinput.h>
}
#define VERSION_ID 11

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    fake_Init(VERSION_ID);
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
    return fake_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities, VERSION_ID);
}

DWORD WINAPI XInputGetDSoundAudioDeviceGuids(
    DWORD dwUserIndex,
    GUID* pDSoundRenderGuid,
    GUID* pDSoundCaptureGuid
    ){
    return fake_XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid, VERSION_ID);
}

DWORD WINAPI XInputGetState(
    _In_   DWORD dwUserIndex,
    _Out_  x_original::XINPUT_STATE *pState

    ){
    return fake_XInputGetState(dwUserIndex, pState, VERSION_ID);
}

DWORD WINAPI XInputSetState(
    _In_     DWORD dwUserIndex,
    _Inout_  x_original::XINPUT_VIBRATION *pVibration
    ){
    return fake_XInputSetState(dwUserIndex, pVibration, VERSION_ID);
}