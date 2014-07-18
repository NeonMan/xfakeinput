/*
Copyright (c) 2014, Juan Luis Álvarez Martínez
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "XFakeInput.h"
#include "DInput_layer.h"
#include "python_layer.h"
#include <mutex>

#include "py_xfakeinput.h"

// ---------------
// --- Globals ---
// ---------------
///The pad state lock
std::mutex mutex_state;
///Is DirectInput initialized?
bool bDirectInput_started = FALSE;
///Number of times fake_Init has been called
int init_count = 0;
///XInput passthrough enabled/disabled
bool passthrough[4] = { FALSE, FALSE, FALSE, FALSE };
///Old pad states, one for each pad
x_original::XINPUT_STATE pad_states[4];

// --------------------------------------------------
// --- Original XInputMethods' pointers (V 9.1.0) ---
// --------------------------------------------------
DWORD(__stdcall* orig_XInputGetState)(DWORD, x_original::XINPUT_STATE*) = 0;
DWORD(__stdcall* orig_XInputSetState)(DWORD, x_original::XINPUT_VIBRATION*) = 0;
DWORD(__stdcall* orig_XInputGetDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*) = 0;
DWORD(__stdcall* orig_XInputGetCapabilities)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*) = 0;

/**
 * @brief Initialices XFakeInput.
 *
 * Should be called by each XInput clone when loaded. For some reason
 * multiple calls to this function will happen so multiple initializations
 * must be detected and ignored.
 * DLLMAIN is serialized, so apparently no locks/mutex are needed.
 */
void fake_Init(DWORD version){
    if (init_count){
        ++init_count;
        return;
    }
    ++init_count;
    //Initialize variables
    pad_states[0] = initial_state;
    pad_states[1] = initial_state;
    pad_states[2] = initial_state;
    pad_states[3] = initial_state;

    //Load the original DLL for XInput passthrough.
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
        //If for some reason loading the original XInput fails
        if (
            (orig_XInputGetState == 0) ||
            (orig_XInputSetState == 0) ||
            (orig_XInputGetDSoundAudioDeviceGuids == 0) ||
            (orig_XInputGetCapabilities == 0)
            ){
            MessageBoxA(0, "Unable to load XInput functions", "ERROR", 0);
        }
    }
    else{
        MessageBoxA(0, "Unable to load XInput", "ERROR", 0);
    }
}

/**
 * @brief Performs state cleanup (DLL unloaded)
 */
void fake_Cleanup(){
    py_cleanup();
}

/**
 * @brief Initialize DirectInput.
 * @note refactor as dinput/python initializer
 */
int directInput_init(){
    bDirectInput_started = TRUE;
    int rv = dinput_init();
    if ((rv < 0)){
        for (int i = 0; i < 4; i++){ passthrough[i] = TRUE; }
        return -1;
    }
    if (py_init("xfakeinput") != 0){
        for (int i = 0; i < 4; i++){ passthrough[i] = TRUE; }
        return -2;
    }
    return 0;
}

/**
 * @brief Test if two XINPUT_GAMEPAD structs are equal.
 * 
 * @param p1 Pad structure
 * @param p2 Pad structure
 * @return TRUE if both structs are equal.
 */
inline bool gamepad_equals(x_original::XINPUT_GAMEPAD p1, x_original::XINPUT_GAMEPAD p2){
    bool rv = TRUE;
    rv &= p1.bLeftTrigger == p2.bLeftTrigger;
    rv &= p1.bRightTrigger == p2.bRightTrigger;
    rv &= p1.sThumbLX == p2.sThumbLX;
    rv &= p1.sThumbLY == p2.sThumbLY;
    rv &= p1.sThumbRX == p2.sThumbRX;
    rv &= p1.sThumbRY == p2.sThumbRY;
    rv &= p1.wButtons == p2.wButtons;
    return rv;
}

/******************************************\
 * Implementation of fake xinputs follows *
\******************************************/

/**
* @brief Returns the current pad state.
* @note Available on: XInput 1.1, 1.2, 1.3, 1.4, 9.1.0
*
* @param dwUserIndex    xbox pad number.
* @param pState         pointer to the pad state struct to be written.
* @param caller_version which XInput version is being emulated.
*
* @returns ERROR_SUCCESS or ERROR_DEVICE_NOT_CONNECTED
*
* pState->dwPacketNumber seems to be always even, increments by two, starting as 2.
*/
DWORD fake_XInputGetState(
    DWORD dwUserIndex,
    x_original::XINPUT_STATE *pState,
    DWORD caller_version
    ){
    DWORD rv;
    if (dwUserIndex > 3)
        return ERROR_DEVICE_NOT_CONNECTED;
    mutex_state.lock();
    if (!bDirectInput_started) directInput_init();

    if (passthrough[dwUserIndex]){
        mutex_state.unlock();
        rv = orig_XInputGetState(dwUserIndex, pState);
        return rv;
    }
    //Not passthrough
    //Get the new State
    *pState = pad_states[dwUserIndex];

    //Retrieve the state from python script
    rv = py_GetState(dwUserIndex, pState);
    
    if (rv == ERROR_SUCCESS){
        //Check if old state is different from the new one.
        //If so, increment packet ID and replace the old state with the new one.
        if (!gamepad_equals(pState->Gamepad, pad_states[dwUserIndex].Gamepad)){
            pState->dwPacketNumber = pad_states[dwUserIndex].dwPacketNumber + 2;
            pad_states[dwUserIndex] = *pState;
        }
    }
    mutex_state.unlock();
    return rv;
}

/**
* @brief Sends data to the pad (vibration)
* @note Available on: XInput 1.1, 1.2, 1.3, 1.4, 9.1.0
*
* @param dwUserIndex    xbox pad number.
* @param pVibration     vibration speed struct pointer.
* @param caller_version which XInput version is being emulated.
*
* @returns ERROR_SUCCESS or ERROR_DEVICE_NOT_CONNECTED
*/
DWORD fake_XInputSetState(
    DWORD dwUserIndex,
    x_original::XINPUT_VIBRATION *pVibration,
    DWORD caller_version
    ){
    DWORD rv;
    if (dwUserIndex > 3)
        return ERROR_DEVICE_NOT_CONNECTED;
    mutex_state.lock();
    if (!bDirectInput_started) directInput_init();
    mutex_state.lock();
    if (!bDirectInput_started) directInput_init();
    if (passthrough[dwUserIndex]){
        rv = orig_XInputSetState(dwUserIndex, pVibration);
        mutex_state.unlock();
        return rv;
    }

    //Not passthrough
    mutex_state.unlock();
    return ERROR_SUCCESS;
}

/**
 * @brief Provides XInputEnable to clone DLLs.
 *
 * @note Available on: XInput 1.1, 1.2, 1.3, 1.4
 */
void fake_XInputEnable(
    BOOL enable
    ){

}

/**
 * @brief Returns the pad capabilities like XInput (9_1_0).
 * @note Available on: XInput 1.1, 1.2, 1.3, 1.4, 9.1.0
 *
 * @param dwUserIndex    xbox pad number.
 * @param dwFlags
 * @param pCapabilities  pointer to the capabilities struct.
 * @param caller_version which XInput version is being emulated.
 *
 * @returns ERROR_SUCCESS or ERROR_DEVICE_NOT_CONNECTED
 */
DWORD fake_XInputGetCapabilities(
    DWORD dwUserIndex,
    DWORD dwFlags,
    x_original::XINPUT_CAPABILITIES *pCapabilities,
    DWORD caller_version
    ){
    if (dwUserIndex > 3)
        return ERROR_DEVICE_NOT_CONNECTED;
    mutex_state.lock();
    if (!bDirectInput_started) directInput_init();
    if (passthrough[dwUserIndex]){
        DWORD rv = orig_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
        mutex_state.unlock();
        return rv;
    }
    //Not passthrough
    mutex_state.unlock();
    *pCapabilities = default_capabilities;
    return ERROR_SUCCESS;
}

/**
 * @brief Emulates XInput function. Always returns 'no sound device'
 * @note Available on: XInput 1.1, 1.2, 1.3, 9.1.0
 *
 * @param dwUserIndex        xbox pad number.
 * @param pDSoundRenderGuid
 * @param pDSoundCaptureGuid
 * @param caller_version     which XInput version is being emulated.
 *
 * @returns ERROR_SUCCESS or ERROR_DEVICE_NOT_CONNECTED
 */
DWORD fake_XInputGetDSoundAudioDeviceGuids(
    DWORD dwUserIndex,
    GUID* pDSoundRenderGuid,
    GUID* pDSoundCaptureGuid,
    DWORD caller_version
    ){
    if (passthrough[dwUserIndex])
        return orig_XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);

    //Not passthrough
    if (dwUserIndex > 3)
        return ERROR_DEVICE_NOT_CONNECTED;
    *pDSoundRenderGuid = GUID_NULL;
    *pDSoundCaptureGuid = GUID_NULL;
    return ERROR_SUCCESS;
}

/**
 * @brief Dummy emulation of keyboard function.
 * @note Available on: XInput 1.3, 1.4
 *
 * @param dwUserIndex    xbox pad number.
 * @param dwReserved     reserved.
 * @param pKeyStroke
 * @param caller_version which XInput version is being emulated.
 *
 * @returns ERROR_DEVICE_NOT_CONNECTED or ERROR_EMPTY
 */
DWORD fake_XInputGetKeystroke(
    DWORD dwUserIndex,
    DWORD dwReserved,
    x_original::PXINPUT_KEYSTROKE pKeystroke,
    DWORD caller_version){
    if (dwUserIndex > 3)
        return ERROR_DEVICE_NOT_CONNECTED;
    return ERROR_EMPTY;
}

/**
 * @brief Dummy function emulation, battery always full
 * @note Available on: XInput 1.3, 1.4
 *
 * @param dwUserIndex          xbox pad number.
 * @param devType        
 * @param pBatteryInformation  Battery info struct pointer.
 * @param caller_version       which XInput version is being emulated.
 *
 * @returns ERROR_SUCCESS
 */
DWORD fake_XInputGetBatteryInformation(
    DWORD dwUserIndex,
    BYTE devType,
    x_original::XINPUT_BATTERY_INFORMATION *pBatteryInformation,
    DWORD caller_version){
    pBatteryInformation->BatteryLevel = BATTERY_LEVEL_FULL;
    pBatteryInformation->BatteryType = BATTERY_TYPE_WIRED;
    return ERROR_SUCCESS;
}

/**
 * @brief Dummy emulation, always returns no audio device.
 * @note Available on: XInput 1.4
 *
 * @param dwUserIndex      xbox pad number.
 * @param pRenderDeviceId
 * @param pRenderCount
 * @param pCaptureDeviceId
 * @param pCaptureCount
 * @param caller_version   which XInput version is being emulated.
 *
 * @returns ERROR_DEVICE_NOT_CONNECTED
 */
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
