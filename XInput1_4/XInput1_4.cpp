/*
 DLL Exports:

 DllMain                        0x0000000180001420	0x00001420	1 (0x1)
 XInputEnable                   0x0000000180001eac	0x00001eac	5 (0x5)
 XInputGetAudioDeviceIds        0x0000000180002548	0x00002548	10 (0xa)
 XInputGetBatteryInformation    0x000000018000212c	0x0000212c	7 (0x7)
 XInputGetCapabilities          0x0000000180001d94	0x00001d94	4 (0x4)
 XInputGetKeystroke             0x000000018000283c	0x0000283c	8 (0x8)
 XInputGetState                 0x00000001800018c4	0x000018c4	2 (0x2)
 XInputSetState                 0x0000000180001af0	0x00001af0	3 (0x3)
 */
#include <Windows.h>
#include <stdint.h>
#include "../XFakeInput/XFakeInput.h"
namespace x_original{
#include <Xinput.h>
}
#define VERSION_ID 14

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

DWORD WINAPI XInputGetAudioDeviceIds(
	_In_         DWORD dwUserIndex,
	_Out_opt_    LPWSTR pRenderDeviceId,
	_Inout_opt_  UINT *pRenderCount,
	_Out_opt_    LPWSTR pCaptureDeviceId,
	_Inout_opt_  UINT *pCaptureCount
	){

	return fake_XInputGetAudioDeviceIds(
		dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount, VERSION_ID
		);
}
