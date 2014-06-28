#include "XFakeInput.h"

// ---------------
// --- Globals ---
// ---------------
///Number of times fake_Init has been called
int init_count = 0;
///XInput passthrough enabled/disabled
bool passthrough[4] = { TRUE, TRUE, TRUE, TRUE }; //{ FALSE, FALSE, FALSE, FALSE };
///Pad states, one for each pad
x_original::XINPUT_STATE pad_states[4];

// --------------------------------------------------
// --- Original XInputMethods' pointers (V 9.1.0) ---
// --------------------------------------------------
DWORD(__stdcall* orig_XInputGetState)(DWORD, x_original::XINPUT_STATE*) = 0;
DWORD(__stdcall* orig_XInputSetState)(DWORD, x_original::XINPUT_VIBRATION*) = 0;
DWORD(__stdcall* orig_XInputGetDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*) = 0;
DWORD(__stdcall* orig_XInputGetCapabilities)(DWORD, DWORD, x_original::XINPUT_CAPABILITIES*) = 0;

#undef ENABLE_LOGGING
#ifdef ENABLE_LOGGING
#include <stdio.h>
FILE* log_f;
char msg[1024];
void log_init(){
    log_f = fopen("XFakeInput.log", "wb");
    fclose(log_f);
}

///Logs a message
int log(char* msg){
    log_f = fopen("XFakeInput.log", "a+b");
    fwrite(msg, strlen(msg), 1, log_f);
    fclose(log_f);
    return 1;
}

#endif

/**
 * @brief Initialices XFakeInput.
 *
 * Should be called by each XInput clone when loaded. For some reason
 * multiple calls to this function will happen so multiple initializations
 * must be detected and ignored.
 */
void fake_Init(DWORD version){
    ///@note this is definetly *not* thread safe.
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
        //If for some reason loading the original XInput fails, passthrough will be disabled.
        if (
            (orig_XInputGetState == 0) ||
            (orig_XInputSetState == 0) ||
            (orig_XInputGetDSoundAudioDeviceGuids == 0) ||
            (orig_XInputGetCapabilities == 0)
            ){
            if (passthrough[0] || passthrough[1] || passthrough[2] || passthrough[3]){
#ifdef ENABLE_LOGGING
                char msg[1024];
                sprintf(msg, "@fake_init: Failed to locate xinput functions\n");
                log(msg);
#endif
                passthrough[0] = FALSE;
                passthrough[1] = FALSE;
                passthrough[2] = FALSE;
                passthrough[3] = FALSE;
            }
        }
    }
    ///Same as before, failing to load XInput disables passthrough.
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

/**
 * @brief Provides XInputEnable to clone DLLs.
 *
 * @note Available on: XInput 1.1, 1.2, 1.3, 1.4
 */
void fake_XInputEnable(
    BOOL enable
    ){
#ifdef ENABLE_LOGGING
    sprintf(msg, "@fake_XInputEnable: Done!\n");
    log(msg);
#endif
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
#ifdef ENABLE_LOGGING
    sprintf(msg, "@fake_XinputSetState: FIXME!\n");
    log(msg);
#endif
    if (passthrough[dwUserIndex])
        return orig_XInputSetState(dwUserIndex, pVibration);

    return ERROR_DEVICE_NOT_CONNECTED;
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
#ifdef ENABLE_LOGGING
    sprintf(msg, "@fake_XinputGetKeyStroke: Begin/End\n");
    log(msg);
#endif
    return ERROR_DEVICE_NOT_CONNECTED;

    //Return when device is connected
    //return ERROR_EMPTY;
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
#ifdef ENABLE_LOGGING
    sprintf(msg, "@fake_XinputGetBatteryInformation: Begin/End\n");
    log(msg);
#endif
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
#ifdef ENABLE_LOGGING
    sprintf(msg, "@fake_XinputGetAudioDeviceIds: Begin/End\n");
    log(msg);
#endif
    return ERROR_DEVICE_NOT_CONNECTED;
}
