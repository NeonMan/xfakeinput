#xfi module provides functions to control XFakeInput
import xfi
#dinput module provides access to DirectInput devices
import dinput
#sys module is built into python
import sys

#Redirect the standard output/error
#If an error happens, it will be logged to text files
sys.stderr = open("stderr.log", "w")
sys.stdout = open("stdout.log", "w")

#Configure which pads will be left unemulated.
#If true, all calls to the selected pad will be pass-through
#to the original XInput API.
xfi.set_passthrough( (False, True, True, True) )

#A sample state dictionary, get_state should return
#a dictionary like this.
#Buttons will be tested as booleans.
#Triggers have a valid range of [0 to 255] (Byte)
#Thumbs have a valid range of [–32,768 to 32,767] (Short)
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

#Print the number of configured devices
print("DInput devices:", dinput.num_devices())

#Print the info about the first device
print("Device #0 info:")
print(dinput.device_info(0))

#Dump the joystate2 from the first device
dinput.poll(0)
print(dinput.get_joystate2(0))

#Search for a device with instance name 'AUTO PAD'
iname = "AUTO PAD"
print("instance name", iname, "is device number:", dinput.device_by_instance(iname))

#Search for a device with instance name 'nonexistant'
iname = "Nonexistant"
print("instance name", iname, "is device number:", dinput.device_by_instance(iname))

#Enumerate the devices with a given name
iname = "AUTO PAD"
print(iname, "devices:", dinput.devices_by_name(iname))
iname = "Not a chance"
print(iname, "devices:", dinput.devices_by_name(iname))

def get_state(dwUserIndex):
  '''Return the pad state'''
  print("Requesting state for pad number:", dwUserIndex)
  return sample_state

def set_state(dwUserIndex, state):
  '''Receive state from the game'''
  pass
  