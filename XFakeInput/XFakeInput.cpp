#include "XFakeInput.h"
#include "DInputLayer.h"
#include <fstream>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/*
 * Startup and globals
 */
bool passthrough[5] = { FALSE, TRUE, TRUE, TRUE, FALSE };
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
	}
	else{
		if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3]){
			MessageBoxA(NULL, "Unable to initialize original XInput. \r\nPassthrough disabled.", "Error", 0);
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

	//Hardcoded capabilities
	const x_original::XINPUT_GAMEPAD pad = {
		0xFFFF,
		0xFF,
		0xFF,
		0xFFFF,
		0xFFFF,
		0xFFFF,
		0xFFFF
	};
	const x_original::XINPUT_VIBRATION vib = {
		65535,
		65535
	};
	const x_original::XINPUT_CAPABILITIES caps = {
		XINPUT_DEVTYPE_GAMEPAD,
		XINPUT_DEVSUBTYPE_GAMEPAD,
		0,
		pad,
		vib
	};
	*pCapabilities = caps;
	return ERROR_SUCCESS;
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

	if (dwUserIndex > 0)
		return ERROR_NOT_CONNECTED;
	++packet_number;
	pState->dwPacketNumber = packet_number;
	// Map directInput device to xinput pad
	DIJOYSTATE2 di_joy = get_state(TEST_INSTANCE);
	pState->Gamepad.sThumbLX = (SHORT)((long)di_joy.lX) - 32767;
	pState->Gamepad.sThumbLY = (SHORT)((long)di_joy.lY) - 32767;
	pState->Gamepad.sThumbRX = (SHORT)((long)di_joy.lZ) - 32767;
	pState->Gamepad.sThumbRY = (SHORT)((long)di_joy.lRz) - 32767;

	pState->Gamepad.bLeftTrigger  = di_joy.rgbButtons[6] ? 1 : 0; //Button 7
	pState->Gamepad.bRightTrigger = di_joy.rgbButtons[7] ? 1 : 0; //Button 8
	WORD buttons = 0;
	if (di_joy.rgbButtons[9]) buttons |= XINPUT_GAMEPAD_START; //Start -> 10
	if (di_joy.rgbButtons[8]) buttons |= XINPUT_GAMEPAD_BACK;  //Back -> 9
	if (di_joy.rgbButtons[1]) buttons |= XINPUT_GAMEPAD_A;     //A --> 2
	if (di_joy.rgbButtons[2]) buttons |= XINPUT_GAMEPAD_B;     //B --> 3
	if (di_joy.rgbButtons[0]) buttons |= XINPUT_GAMEPAD_X;     //X --> 1
	if (di_joy.rgbButtons[3]) buttons |= XINPUT_GAMEPAD_Y;     //Y --> 4
	if (di_joy.rgbButtons[4]) buttons |= XINPUT_GAMEPAD_LEFT_SHOULDER; //LShoulder --> 5
	if (di_joy.rgbButtons[5]) buttons |= XINPUT_GAMEPAD_RIGHT_SHOULDER; //RShoulderr --> 6
	pState->Gamepad.wButtons = buttons;

	return ERROR_SUCCESS;
}

DWORD fake_XInputSetState(
	DWORD dwUserIndex,
	x_original::XINPUT_VIBRATION *pVibration,
	DWORD caller_version
	){
	if (passthrough[dwUserIndex])
		return orig_XInputSetState(dwUserIndex, pVibration);

	if (dwUserIndex > 0)
		return ERROR_NOT_CONNECTED;

	return ERROR_SUCCESS;
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
