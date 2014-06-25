/*
DLL Exports:

DllMain	                        0x00402780	0x00002780	1 (0x1)
XInputEnable                    0x00403190	0x00003190	5 (0x5)
XInputGetBatteryInformation     0x00403380	0x00003380	7 (0x7)
XInputGetCapabilities           0x00402f60	0x00002f60	4 (0x4)
XInputGetDSoundAudioDeviceGuids	0x00403ad0	0x00003ad0	6 (0x6)
XInputGetKeystroke	            0x00403e30	0x00003e30	8 (0x8)
XInputGetState	                0x00402980	0x00002980	2 (0x2)
XInputSetState                  0x00402db0	0x00002db0	3 (0x3)
*/
#include <Windows.h>
#include <stdint.h>
#include "../XFakeInput/XFakeInput.h"
namespace x_original{
#include <Xinput.h>
}
#define VERSION_ID 13

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

DWORD WINAPI XInputGetKeystroke(
	DWORD dwUserIndex,
	DWORD dwReserved,
	x_original::PXINPUT_KEYSTROKE pKeystroke){
	return fake_XInputGetKeystroke(dwUserIndex, dwReserved, pKeystroke, VERSION_ID);
}

DWORD WINAPI XInputGetBatteryInformation(
	_In_   DWORD dwUserIndex,
	_In_   BYTE devType,
	_Out_  x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation
	){
	return fake_XInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation, VERSION_ID);
}
