#pragma once
#ifndef __XFakeInput_h
#define __XFakeInput_h

#include <Windows.h>
#include <stdint.h>

//Prevent collisions with original XInput DLL
namespace x_original{
#include <Xinput.h>
}

/*
 * Constants
 */

/**Capabilities to return when queried
 * copied from an actual xbox360 pad response.
 */
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

//XFakeInput specific methods
void fake_Init(DWORD version);

/*
 * Fake xinputs declarations.
 */
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