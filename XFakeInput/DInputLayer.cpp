#include "DInputLayer.h"

//Device enumeration callback, called once for every device during enumeration
BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	HRESULT hr;

	// Obtain an interface to the enumerated joystick.
	if (joy_count < MAX_JOY){
		hr = di->CreateDevice(instance->guidInstance, &joysticks[joy_count], NULL);
		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if (FAILED(hr)) {
			return DIENUM_CONTINUE;
		}
		else
			joy_count++;
	}
	return DIENUM_CONTINUE;
}

//Enumerate all devices, configure as joystick, get non-exclusive access to them.
int device_startup(){
	//Clear arrays
	memset(joysticks, 0, sizeof(LPDIRECTINPUTDEVICE8)* MAX_JOY);
	//Initialize dInput
	HRESULT hr;
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&di, NULL))) {
		return -1;
	}
	// Enumerate all dInput Gamectrl devices
	if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback,
		NULL, DIEDFL_ATTACHEDONLY))) {
		return -2;
	}
	//Configure devices
	for (int i = 0; i < joy_count; i++){
		hr =  !joysticks[i]->SetDataFormat(&c_dfDIJoystick2);
		hr |= !joysticks[i]->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		hr |= !joysticks[i]->Acquire();
		if (hr) return -3;
		joy_infos[i].dwSize = sizeof(DIDEVICEINSTANCEW);
		joy_caps[i].dwSize = sizeof(DIDEVCAPS);
		hr =  !joysticks[i]->GetDeviceInfo(joy_infos + i);
		hr |= !joysticks[i]->GetCapabilities(joy_caps + i);
		if (hr) return -4;
	}
}

int instance_to_id(WCHAR* tszInstanceName){
	for (int i = 0; i < joy_count; i++){
		if (wcsncmp(tszInstanceName, joy_infos[i].tszInstanceName, 260 ) == 0) return i;
	}
	return -1;
}

DIJOYSTATE2 get_state(int id){
	DIJOYSTATE2 rv;
	joysticks[id]->Poll();
	joysticks[id]->GetDeviceState(sizeof(DIJOYSTATE2), &rv);
	return rv;
}

DIJOYSTATE2 get_state(WCHAR* tszInstanceName){
	return get_state(instance_to_id(tszInstanceName));
}