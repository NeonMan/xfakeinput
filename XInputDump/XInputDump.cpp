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
#include <Windows.h>
#include <Xinput.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Xinput9_1_0.lib")

void main(){
    //Dump the capabilities
    printf("----------- Capabilities -----------\r\n");
    for (int i = 0; i < 4; i++){
        XINPUT_CAPABILITIES c;
        DWORD rv;
        rv = XInputGetCapabilities(i, 0, &c);

        if (rv == ERROR_DEVICE_NOT_CONNECTED){
            printf("Pad #%d: --- Not connected ---\r\n", i);
        }
        else{
            printf("Pad #%d capabilities:\r\n");
            printf("  {\r\n");
            printf("    type:    0x%X,\r\n", c.Type);
            printf("    subtype: 0x%X,\r\n", c.SubType);
            printf("    flags:   0x%X,\r\n", c.Flags);
            printf("    Gamepad\r\n");
            printf("    {\r\n");
            printf("      wbuttons:      0x%X,\r\n", c.Gamepad.wButtons);
            printf("      bLeftTrigger:  0x%X,\r\n", c.Gamepad.bLeftTrigger);
            printf("      bRightTrigger: 0x%X,\r\n", c.Gamepad.bRightTrigger);
            printf("      sThumbLX:      0x%X,\r\n", c.Gamepad.sThumbLX);
            printf("      sThumbLY:      0x%X,\r\n", c.Gamepad.sThumbLY);
            printf("      sThumbRX:      0x%X,\r\n", c.Gamepad.sThumbRX);
            printf("      sThumbLY:      0x%X\r\n", c.Gamepad.sThumbLY);
            printf("    }\r\n");
            printf("    Vibration\r\n");
            printf("    {\r\n");
            printf("      wLeftMotorSpeed:  0x%X,\r\n", c.Vibration.wLeftMotorSpeed);
            printf("      wRightMotorSpeed: 0x%X\r\n", c.Vibration.wRightMotorSpeed);
            printf("    }\r\n");
            printf("  }\r\n");
        }

    }
    printf("------------------------------------\r\n");

    //Continuously dump each pad state
    while(1){
        for (int i = 0; i < 4; i++){
            XINPUT_STATE s;
            DWORD hs;
            hs = XInputGetState(i, &s);
            if (hs == ERROR_DEVICE_NOT_CONNECTED){
                printf("Pad #%d: --- Not connected ---\r\n", i);
            }
            else{
                printf("Pad #%d: ID:0x%X LX:%d LY:%d RX:%d RY:%d RT:%d LT:%d\r\n",
                    i,                       /*Pad #*/
                    s.dwPacketNumber,        /*Packet #*/
                    s.Gamepad.sThumbLX,      /*LX*/
                    s.Gamepad.sThumbLY,      /*LY*/
                    s.Gamepad.sThumbRX,      /*RX*/
                    s.Gamepad.sThumbRY,      /*RY*/
                    s.Gamepad.bRightTrigger, /*RT*/
                    s.Gamepad.bLeftTrigger   /*LT*/
                    );
                printf("         A:%d B:%d X:%d Y:%d DU:%d DD:%d DL:%d DR:%d ST:%d BK:%d LT:%d RT:%d LS:%d RS:%d\r\n",
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_A ? 1 : 0,             /*A*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_B ? 1 : 0,             /*B*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_X ? 1 : 0,             /*X*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? 1 : 0,             /*Y*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? 1 : 0,       /*DU*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? 1 : 0,     /*DD*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? 1 : 0,     /*DL*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? 1 : 0,    /*DR*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_START ? 1 : 0,         /*Start*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? 1 : 0,          /*Back*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? 1 : 0,    /*LT*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? 1 : 0,   /*RT*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? 1 : 0, /*LS*/
                    s.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? 1 : 0 /*LS*/
                    );
            }
        }
        Sleep(2000);
    }
}