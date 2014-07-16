#
# passthrough.py
#
# This script will configure all pads as passthrough, it should behave
# like the original XInput library.
#

#Here we import packages we will need
import xfi     #<-- xfi gives access to XFakeInput

#Configure which pads will be left unemulated.
#If true, all calls to the selected pad will be pass-through
#to the original XInput API.
#In this case, all pads will be left unemulated
xfi.set_passthrough( (True, True, True, True) )

def get_state(dwUserIndex):
  return None

def set_state(dwUserIndex, state):
  pass
  