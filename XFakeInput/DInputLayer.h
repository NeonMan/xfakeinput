#ifndef __DINPUTLAYER_H
#define __DINPUTLAYER_H
#include <Windows.h>
#include <dinput.h>

//DirectInput entry object
static LPDIRECTINPUT8 di;

//DirectInput device list
#define MAX_JOY 32
static LPDIRECTINPUTDEVICE8 joysticks[MAX_JOY];
static DIDEVICEINSTANCEW joy_infos[MAX_JOY];
static DIDEVCAPS joy_caps[MAX_JOY];
static int joy_count = 0;

//XInput variables
static DWORD packet_number = 100;

//Functions
int device_startup();
int instance_to_id(WCHAR* tszInstanceName);
DIJOYSTATE2 get_state(int id);
DIJOYSTATE2 get_state(WCHAR* tszInstanceName);

#endif