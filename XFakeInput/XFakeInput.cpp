#include "XFakeInput.h"

/*
 * Startup and globals
 */
int init_count = 0;
bool passthrough[5] = { TRUE, TRUE, TRUE, TRUE, FALSE };
//bool passthrough[5] = { FALSE, FALSE, FALSE, FALSE, FALSE };
#define TEST_INSTANCE L"AUTO PAD"

//Original xinput function pointers
DWORD(__stdcall* orig_XInputGetState)(DWORD, x_original::XINPUT_STATE*) = 0;
DWORD(__stdcall* orig_XInputSetState)(DWORD, x_original::XINPUT_VIBRATION*) = 0;
DWORD(__stdcall* orig_XInputGetDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*) = 0;
DWORD(__stdcall* orig_XInputGetCapabilities)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*) = 0;

#define ENABLE_LOGGING
#ifdef ENABLE_LOGGING
#include <stdio.h>
FILE* log_f;
char msg[1024];
void log_init(){
	log_f = fopen("XFakeInput.log", "wb");
	fclose(log_f);
}

int log(char* msg){
	log_f = fopen("XFakeInput.log", "a+b");
	fwrite(msg, strlen(msg), 1, log_f);
	fclose(log_f);
	return 1;
}

#endif

void fake_Init(DWORD version){
	if (init_count){
		++init_count;
		return;
	}
	++init_count;
#ifdef ENABLE_LOGGING
	log_init();
	char msg[1024];
	sprintf(msg, "@fake_init: Starting XFakeInput as Xinput V%d\n", version);
	log(msg);
#endif
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
#ifdef ENABLE_LOGGING
				char msg[1024];
				sprintf(msg, "@fake_init: Failed to llocate xinput functions\n");
				log(msg);
#endif
				passthrough[0] = FALSE;
				passthrough[1] = FALSE;
				passthrough[2] = FALSE;
				passthrough[3] = FALSE;
			}
		}
	}
	else{
		if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3]){
#ifdef ENABLE_LOGGING
			char msg[1024];
			sprintf(msg, "@fake_init: Unable to load XInput9_1_0.dll\n");
			log(msg);
#endif
			passthrough[0] = FALSE;
			passthrough[1] = FALSE;
			passthrough[2] = FALSE;
			passthrough[3] = FALSE;
		}
	}
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_init: Done!\n");
	log(msg);
#endif
}

/*
* Implementation of fake xinputs follows
*/

void fake_XInputEnable(
	BOOL enable
	){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XInputEnable: Done!\n");
	log(msg);
#endif
}

DWORD fake_XInputGetCapabilities(
	DWORD dwUserIndex,
	DWORD dwFlags,
	x_original::XINPUT_CAPABILITIES *pCapabilities,
	DWORD caller_version
	){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetCapabilities: Pad #%d, Flags 0x%X\n", dwUserIndex, dwFlags);
	log(msg);
#endif
	if (passthrough[dwUserIndex]){
		DWORD rv = orig_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
#ifdef ENABLE_LOGGING
		char msg[1024];
		sprintf(msg, "@fake_XinputGetCapabilities: {%X %X %X {%X %X %X %X %X %X %X} {%X %X}} [%d] (Passthrough)\n",
			pCapabilities->Type,
			pCapabilities->SubType,
			pCapabilities->Flags,
			pCapabilities->Gamepad.wButtons,
			pCapabilities->Gamepad.bLeftTrigger,
			pCapabilities->Gamepad.bRightTrigger,
			pCapabilities->Gamepad.sThumbRX,
			pCapabilities->Gamepad.sThumbRY,
			pCapabilities->Gamepad.sThumbLX,
			pCapabilities->Gamepad.sThumbLY,
			pCapabilities->Vibration.wLeftMotorSpeed,
			pCapabilities->Vibration.wRightMotorSpeed,
			rv
			);
		log(msg);
#endif
		return rv;
	}

#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetCapabilities: ERROR_DEVICE_NOT_CONNECTED\n");
	log(msg);
#endif
	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetDSoundAudioDeviceGuids(
	DWORD dwUserIndex,
	GUID* pDSoundRenderGuid,
	GUID* pDSoundCaptureGuid,
	DWORD caller_version
	){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XInputGetDSoundAudioDeviceGuids: Pad #%d\n", dwUserIndex);
	log(msg);
#endif
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
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XInputGetState: Pad #%d\n", dwUserIndex);
	log(msg);
#endif
	if (passthrough[dwUserIndex]){
		DWORD rv = orig_XInputGetState(dwUserIndex, pState);
#ifdef ENABLE_LOGGING
		sprintf(msg, "@fake_XinputGetState: #%X {%X %X %X %X %X %X %X} [%d] (Passthrough)\n",
			pState->dwPacketNumber,
			pState->Gamepad.wButtons,
			pState->Gamepad.bLeftTrigger,
			pState->Gamepad.bRightTrigger,
			pState->Gamepad.sThumbRX,
			pState->Gamepad.sThumbRY,
			pState->Gamepad.sThumbLX,
			pState->Gamepad.sThumbLY,
			rv
			);
		log(msg);
#endif
		return rv;
	}
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetState: ERROR_DEVICE_NOT_CONNECTED\n");
	log(msg);
#endif
	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputSetState(
	DWORD dwUserIndex,
	x_original::XINPUT_VIBRATION *pVibration,
	DWORD caller_version
	){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputSetState: FIXME!\n");
	log(msg);
#endif
	if (passthrough[dwUserIndex])
		return orig_XInputSetState(dwUserIndex, pVibration);

	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD fake_XInputGetKeystroke(
	DWORD dwUserIndex,
	DWORD dwReserved,
	x_original::PXINPUT_KEYSTROKE pKeystroke,
	DWORD caller_version){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetKeyStroke: Begin/End\n");
	log(msg);
#endif
	return ERROR_DEVICE_NOT_CONNECTED;

	//Return when device is connected
	//return ERROR_EMPTY;
}

DWORD fake_XInputGetBatteryInformation(
	DWORD dwUserIndex,
	BYTE devType,
	x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation,
	DWORD caller_version){
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetBatteryInformation: Begin/End\n");
	log(msg);
#endif
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
#ifdef ENABLE_LOGGING
	sprintf(msg, "@fake_XinputGetAudioDeviceIds: Begin/End\n");
	log(msg);
#endif
	return ERROR_DEVICE_NOT_CONNECTED;
}
