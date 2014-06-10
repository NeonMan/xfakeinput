/*
 Functions exported by this DLL according to DLLExport.
 DllMain                            0x00000001800011c8	0x000011c8	1 (0x1)
 XInputGetCapabilities              0x000000018000134c	0x0000134c	2 (0x2)
 XInputGetDSoundAudioDeviceGuids    0x00000001800013b0	0x000013b0	3 (0x3)
 XInputGetState                     0x00000001800012e4	0x000012e4	4 (0x4)
 XInputSetState                     0x0000000180001318	0x00001318	5 (0x5)

*/
#include <Windows.h>
#include <stdint.h>
#include "../XFakeInput/XFakeInput.h"

//Prevent collisions with original XInput DLL
namespace x_original{
#include <Xinput.h>
}

#define VERSION_ID 9001000

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	return TRUE;
}

DWORD XInputGetCapabilities(
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

DWORD XInputGetState(
	_In_   DWORD dwUserIndex,
	_Out_  x_original::XINPUT_STATE *pState

	){
	return fake_XInputGetState(dwUserIndex, pState, VERSION_ID);
}

DWORD XInputSetState(
	_In_     DWORD dwUserIndex,
	_Inout_  x_original::XINPUT_VIBRATION *pVibration
	){
	return fake_XInputSetState(dwUserIndex, pVibration, VERSION_ID);
}