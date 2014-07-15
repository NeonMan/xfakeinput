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

def get_state():
  '''Return the pad state'''
  return sample_state

def set_state(state):
  '''Receive state from the game'''
  pass
  