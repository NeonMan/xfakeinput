#include "XFakeInput.h"
#include <dinput.h>


/*
 * Startup and globals
 */
bool passthrough[5] = { TRUE, TRUE, TRUE, TRUE, FALSE };

//Original xinput function pointers
void(__stdcall* orig_XInputEnable)(BOOL);
DWORD(__stdcall* orig_XInputGetState)(DWORD, x_original::XINPUT_STATE*);
DWORD(__stdcall* orig_XInputSetState)(DWORD, x_original::XINPUT_VIBRATION*);
DWORD(__stdcall* orig_XInputGetKeyStroke)(DWORD, DWORD, x_original::PXINPUT_KEYSTROKE);
DWORD(__stdcall* orig_XInputGetDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*);
DWORD(__stdcall* orig_XInputGetCapabilities)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*);
DWORD(__stdcall* orig_XInputGetBatteryInformation)(DWORD, DWORD, x_original::XINPUT_BATTERY_INFORMATION*);
DWORD(__stdcall* orig_XInputGetAudioDeviceIds)(DWORD, LPWSTR, UINT*, LPWSTR, UINT*);

void fake_Init(){
	for (int i = 0; i < 4; i++)
		passthrough[i] = TRUE;

	//Load the original DLL. Version 1.3 since it has most of the functionality
	TCHAR dllpath[MAX_PATH];
	GetWindowsDirectory(dllpath, MAX_PATH);
	wcscat_s(dllpath, L"\\System32\\XInput1_3.dll");
	HMODULE l = LoadLibraryW(dllpath);

	orig_XInputEnable= 
		(void(__stdcall*)(BOOL)) GetProcAddress(l, "XInputEnable");
	orig_XInputGetState = 
		(DWORD(__stdcall*)(DWORD, x_original::XINPUT_STATE*)) GetProcAddress(l, "XInputGetState");
	orig_XInputSetState =
		(DWORD(__stdcall*)(DWORD, x_original::XINPUT_VIBRATION*)) GetProcAddress(l, "XInputSetState");
	orig_XInputGetKeyStroke =
		(DWORD(__stdcall*)(DWORD, DWORD, x_original::PXINPUT_KEYSTROKE)) GetProcAddress(l, "XInputGetKeyStroke");
	orig_XInputGetDSoundAudioDeviceGuids =
		(DWORD(__stdcall*)(DWORD, GUID*, GUID*)) GetProcAddress(l, "XInputGetDSoundAudioDeviceGuids");
	orig_XInputGetCapabilities =
		(DWORD(__stdcall*)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*)) GetProcAddress(l, "XInputGetCapabilities");
	orig_XInputGetBatteryInformation =
		(DWORD(__stdcall*)(DWORD, DWORD, x_original::XINPUT_BATTERY_INFORMATION*)) GetProcAddress(l, "XInputGetBatteryInformation");
	orig_XInputGetAudioDeviceIds =
		(DWORD(__stdcall*)(DWORD, LPWSTR, UINT*, LPWSTR, UINT*)) GetProcAddress(l, "XInputGetAudioDeviceIds");
}

/*
* Implementation of fake xinputs follows
*/

void fake_XInputEnable(
	BOOL enable
	){
	if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3])
		orig_XInputEnable(enable);
}

DWORD fake_XInputGetCapabilities(
	DWORD dwUserIndex,
	DWORD dwFlags,
	x_original::XINPUT_CAPABILITIES *pCapabilities,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetDSoundAudioDeviceGuids(
	DWORD dwUserIndex,
	GUID* pDSoundRenderGuid,
	GUID* pDSoundCaptureGuid,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);

	*pDSoundRenderGuid = GUID_NULL;
	*pDSoundCaptureGuid = GUID_NULL;
	return ERROR_SUCCESS;
}

DWORD fake_XInputGetState(
	DWORD dwUserIndex,
	x_original::XINPUT_STATE *pState,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputGetState(dwUserIndex, pState);

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputSetState(
	DWORD dwUserIndex,
	x_original::XINPUT_VIBRATION *pVibration,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputSetState(dwUserIndex, pVibration);

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetKeystroke(
	DWORD dwUserIndex,
	DWORD dwReserved,
	x_original::PXINPUT_KEYSTROKE pKeystroke,
	DWORD caller_version){
	if (passthrough[dwUserIndex])
		return orig_XInputGetKeyStroke(dwUserIndex, dwReserved, pKeystroke);

	return ERROR_EMPTY;
}

DWORD fake_XInputGetBatteryInformation(
	DWORD dwUserIndex,
	BYTE devType,
	x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation,
	DWORD caller_version){
	if (passthrough[dwUserIndex])
		return orig_XInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation);

	pBatteryInformation->BatteryLevel = BATTERY_LEVEL_FULL;
	pBatteryInformation->BatteryType = BATTERY_TYPE_WIRED;
	return ERROR_SUCCESS;
}

DWORD fake_XInputGetAudioDeviceIds(
	DWORD dwUserIndex,
	LPWSTR pRenderDeviceId,
	UINT *pRenderCount,
	LPWSTR pCaptureDeviceId,
	UINT *pCaptureCount,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputGetAudioDeviceIds(dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount);

	return ERROR_DEVICE_NOT_CONNECTED;
}
