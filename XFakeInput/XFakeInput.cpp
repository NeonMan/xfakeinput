#include "XFakeInput.h"

/*
 * Startup and globals
 */
bool passthrough[5] = { TRUE, TRUE, TRUE, TRUE, FALSE };
#define TEST_INSTANCE L"AUTO PAD"

//Original xinput function pointers
DWORD(__stdcall* orig_XInputGetState)(DWORD, x_original::XINPUT_STATE*) = 0;
DWORD(__stdcall* orig_XInputSetState)(DWORD, x_original::XINPUT_VIBRATION*) = 0;
DWORD(__stdcall* orig_XInputGetDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*) = 0;
DWORD(__stdcall* orig_XInputGetCapabilities)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*) = 0;

void fake_Init(DWORD version){
	//Load the original DLL.
	TCHAR dllpath[MAX_PATH];
	GetWindowsDirectory(dllpath, MAX_PATH);
	wcscat_s(dllpath, L"\\System32\\XInput9_1_0.dll");
	HMODULE l = LoadLibraryW(dllpath);

	//Get the functions' addresses
	if (l){
		orig_XInputGetState =
			(DWORD(__stdcall*)(DWORD, x_original::XINPUT_STATE*)) GetProcAddress(l, "XInputGetState");
		orig_XInputSetState =
			(DWORD(__stdcall*)(DWORD, x_original::XINPUT_VIBRATION*)) GetProcAddress(l, "XInputSetState");
		orig_XInputGetDSoundAudioDeviceGuids =
			(DWORD(__stdcall*)(DWORD, GUID*, GUID*)) GetProcAddress(l, "XInputGetDSoundAudioDeviceGuids");
		orig_XInputGetCapabilities =
			(DWORD(__stdcall*)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*)) GetProcAddress(l, "XInputGetCapabilities");
		if (
			(orig_XInputGetState == 0) ||
			(orig_XInputSetState == 0) ||
			(orig_XInputGetDSoundAudioDeviceGuids == 0) ||
			(orig_XInputGetCapabilities == 0)
			){
			if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3]){
				MessageBoxA(NULL, "Uable to load original XInput functions. \r\nPassthroug disabled.", "Error", 0);
				passthrough[0] = FALSE;
				passthrough[1] = FALSE;
				passthrough[2] = FALSE;
				passthrough[3] = FALSE;
			}
		}
	}
	else{
		if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3]){
			MessageBoxA(NULL, "Unable to locate original XInput9_1_0.dll \r\nPassthrough disabled.", "Error", 0);
			passthrough[0] = FALSE;
			passthrough[1] = FALSE;
			passthrough[2] = FALSE;
			passthrough[3] = FALSE;
		}
	}
}

/*
* Implementation of fake xinputs follows
*/

void fake_XInputEnable(
	BOOL enable
	){

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

	return ERROR_DEVICE_NOT_CONNECTED;

	//Return this if device connected
	//*pDSoundRenderGuid = GUID_NULL;
	//*pDSoundCaptureGuid = GUID_NULL;
	//return ERROR_SUCCESS;
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

	return ERROR_DEVICE_NOT_CONNECTED;

	//Return when device is connected
	//return ERROR_EMPTY;
}

DWORD fake_XInputGetBatteryInformation(
	DWORD dwUserIndex,
	BYTE devType,
	x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation,
	DWORD caller_version){

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
	return ERROR_DEVICE_NOT_CONNECTED;
}
