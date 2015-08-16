#
# keyboard-mapping.py
#
# This script crudely maps the keyboard into the X360 pad
#
#
import dinput
import sys
import xfi

from keynames import key_names

#If an error happens, it will silently pass, unless we redirect the standard
#output (logs) and the standard error (errors) to files. Here we tell the 
#script to save those streams to files
#sys.stderr = open("stderr.log", "w") #<-- Errors will be saved to 'stderr.log'
#sys.stdout = open("stdout.log", "w") #<-- Logs will be saved to 'stout.log'

#Will emulate the first pad.
xfi.set_passthrough( (False, True, True, True) )

#Get device count
device_count = dinput.num_devices()
print("Devices detected: ", device_count)

#Store the selected device here
device_id = None

#For each device, obtain info
for i in range(device_count):
 info = dinput.device_info(i)
 print("%s:" % info['ProductName'])
 keys = list(info.keys())
 keys.sort()
 for k in keys:
  print("    %s: %s" % (k, info[k]))
  
 if info["DevType"] == 1043:
  #Detected a keyboard!
  device_id = i
  
  if device_id == None:
    raise(BaseException("Keyboard not found"))
print("-----------------------------------------------------")


#Sample state. Use when no device is available or as a base for a new state.
sample_state = { \
  'ButtonA' : False, \
  'ButtonB' : False, \
  'ButtonX' : False, \
  'ButtonY' : False, \
  'ButtonBack' : False, \
  'ButtonStart' : False, \
  'ButtonU' : False, \
  'ButtonD' : False, \
  'ButtonL' : False, \
  'ButtonR' : False, \
  'ButtonLThumb' : False, \
  'ButtonRThumb' : False, \
  'ButtonLShoulder' : False, \
  'ButtonRShoulder' : False, \
  'TriggerL' : 0, \
  'TriggerR' : 0, \
  'ThumbLX' : 0, \
  'ThumbLY' : 0, \
  'ThumbRX' : 0, \
  'ThumbRY' : 0, \
}

#For each call, dump the state
def get_state(dwUserIndex):
  if device_id == None:
    return None
  if dwUserIndex != 0:
    return None
  
  #Get the DirectInput state
  dinput.poll(device_id)
  dinput_state = dinput.get_keyboard(device_id)
  
  #Prepare a new state
  rv = sample_state
  #############################
  ##                         ##
  ##  KEY MAPPINGS BELOW !!  ##
  ##                         ##
  #############################
  
  #        /---- This is the XBOX pad button name
  #        |
  #        |    This is the keyboard key mapped to the button ---\
  #        |                                                     |
  #  |-----+-----|                                     |---------+|
  rv["ButtonStart"]  = (dinput_state["Keys"][key_names["DIK_ESCAPE"]] != 0)   #          Start <--> ESC
  rv["ButtonBack"]   = (dinput_state["Keys"][key_names["DIK_BACK"]] != 0)     #           Back <--> Backspace
  rv["ButtonA"]      = (dinput_state["Keys"][key_names["DIK_SPACE"]] != 0)    #              A <--> Space
  rv["ButtonB"]      = (dinput_state["Keys"][key_names["DIK_LCONTROL"]] != 0) #              B <--> Left Control
  rv["ButtonX"]      = (dinput_state["Keys"][key_names["DIK_LSHIFT"]] != 0)   #              X <--> Left Shift
  rv["ButtonY"]      = (dinput_state["Keys"][key_names["DIK_Z"]] != 0)        #              Y <--> Z
  rv["ButtonU"]      = (dinput_state["Keys"][key_names["DIK_1"]] != 0)        #             Up <--> 1
  rv["ButtonD"]      = (dinput_state["Keys"][key_names["DIK_2"]] != 0)        #           Down <--> 2
  rv["ButtonL"]      = (dinput_state["Keys"][key_names["DIK_3"]] != 0)        #           Left <--> 3
  rv["ButtonR"]      = (dinput_state["Keys"][key_names["DIK_4"]] != 0)        #          Right <--> 4
  rv["ButtonLThumb"] = (dinput_state["Keys"][key_names["DIK_Q"]] != 0)        #     Left Thumb <--> Q
  rv["ButtonRThumb"] = (dinput_state["Keys"][key_names["DIK_E"]] != 0)        #    Right Thumb <--> E
  rv["ButtonLShoulder"] = (dinput_state["Keys"][key_names["DIK_Z"]] != 0)     #  Left Shoulder <--> Z
  rv["ButtonRShoulder"] = (dinput_state["Keys"][key_names["DIK_C"]] != 0)     # Right Shoulder <--> C
  
  #Axis work a bit different that buttons since they are analog inputs.
  #     Here is the keyboard key name -\
  #                                 |--+--|
  if dinput_state["Keys"][key_names["DIK_R"]] != 0: # Left trigger <--> R
    rv["TriggerL"] = 127 #<-- Here is the axis value when the key IS pressed.
  else:
    rv["TriggerL"] = 0 #<-- Here is the value when the key is NOT pressed.
  if dinput_state["Keys"][key_names["DIK_F"]] != 0: # Right trigger <--> F
    rv["TriggerR"] = 127
  else:
    rv["TriggerR"] = 0
  if dinput_state["Keys"][key_names["DIK_W"]] != 0: # Left thumb (Forward) <--> W
    rv["ThumbLY"] = 32760
  else:
    rv["ThumbLY"] = 0
  if dinput_state["Keys"][key_names["DIK_S"]] != 0: # Left thumb (Backwards) <--> S
    rv["ThumbLY"] = -32760
  else:
    pass
  if dinput_state["Keys"][key_names["DIK_D"]] != 0: # Left thumb (Right) <--> D
    rv["ThumbLX"] = 32760
  else:
    rv["ThumbLX"] = 0
  if dinput_state["Keys"][key_names["DIK_A"]] != 0: # Left thumb (Left) <--> A
    rv["ThumbLX"] = -32760
  else:
    pass
  if dinput_state["Keys"][key_names["DIK_I"]] != 0: # Left thumb (Forward) <--> I
    rv["ThumbRY"] = 32760
  else:
    rv["ThumbRY"] = 0
  if dinput_state["Keys"][key_names["DIK_K"]] != 0: # Left thumb (Backwards) <--> K
    rv["ThumbRY"] = -32760
  else:
    pass
  if dinput_state["Keys"][key_names["DIK_L"]] != 0: # Left thumb (Right) <--> L
    rv["ThumbRX"] = 32760
  else:
    rv["ThumbRX"] = 0
  if dinput_state["Keys"][key_names["DIK_J"]] != 0: # Left thumb (Left) <--> J
    rv["ThumbRX"] = -32760
  else:
    pass
  return rv
  
 
def set_state(dwUserIndex, state):
  pass
 