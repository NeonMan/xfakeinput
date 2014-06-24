#include "XFakeInput.h"

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

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetDSoundAudioDeviceGuids(
	DWORD dwUserIndex,
	GUID* pDSoundRenderGuid,
	GUID* pDSoundCaptureGuid,
	DWORD caller_version
	){

	*pDSoundRenderGuid = GUID_NULL;
	*pDSoundCaptureGuid = GUID_NULL;
	return ERROR_SUCCESS;
}

DWORD fake_XInputGetState(
	DWORD dwUserIndex,
	x_original::XINPUT_STATE *pState,
	DWORD caller_version
	){

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputSetState(
	DWORD dwUserIndex,
	x_original::XINPUT_VIBRATION *pVibration,
	DWORD caller_version
	){

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetKeystroke(
	DWORD dwUserIndex,
	DWORD dwReserved,
	x_original::PXINPUT_KEYSTROKE pKeystroke,
	DWORD caller_version){

	return ERROR_EMPTY;
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
