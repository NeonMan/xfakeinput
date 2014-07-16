#
# simple-mapping.py
#
# This script shows how to make a simple pad mapping where a DirectInput
# gamepad will be used to emulate the second gamepad. If there is an XBOX
# pad connected and set up as pad number 1,3 or 4. It will be available too
#



#Here we import packages we will need
import xfi     #<-- xfi gives access to XFakeInput
import dinput  #<-- dinput gives access to DirectInput
import sys     #<-- sys provides common system utilities (python built-in)

#If an error happens, it will silently pass, unless we redirect the standard
#output (logs) and the standard error (errors) to files. Here we tell the 
#script to save those streams to files
sys.stderr = open("stderr.log", "w") #<-- Errors will be saved to 'stderr.log'
sys.stdout = open("stdout.log", "w") #<-- Logs will be saved to 'stout.log'

#This is a sample pad state, with all axes at rest and no buttons pressed.
#Can be used as a 'default' state when we are unable to retrieve the actual
#device state.
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

#Which input device are we going to use for this simple mapping. 
#in this case 'AUTO PAD'
dev_id = dinput.device_by_instance("AUTO PAD")

#Which of the four pads we want to emulate, in this case the second one.
#Remember that pad numeration starts at 0. therefore:
#Pad 1 --> pad_id 0
#Pad 2 --> pad_id 1
#Pad 3 --> pad_id 2
#Pad 4 --> pad_id 3
pad_id = 1

#Configure which pads will be left unemulated.
#If true, all calls to the selected pad will be pass-through
#to the original XInput API.
#In this case, only the second pad will be emulated.
xfi.set_passthrough( (True, False, True, True) )

#This function converts a DirectInput state to a XInput state.
def make_state(jstat):
  rv = {}
  #Analog sticks
  #XInput Left thumb X is mapped to DirectInput X-Axis
  #DirectInput and XInput ranges are different, to convert DInput axis values
  #to XInput axis values, we use the long_to_axis() function
  rv['ThumbLX'] = xfi.long_to_axis(jstat['X'])
  #Left thumb Y --> Y-Axis
  rv['ThumbLY'] = -xfi.long_to_axis(jstat['Y'])
  #Right thumb X --> Z-Axis
  rv['ThumbRX'] = xfi.long_to_axis(jstat['Z'])
  #Right thumb Y --> Y-Rotation
  #DirectInput assigns the names, not my fault.
  rv['ThumbRY'] = -xfi.long_to_axis(jstat['Rz'])
  
  #Buttons
  #Button mapping is pretty straightforward
  #A --> Button 1
  rv['ButtonA'] = jstat['Buttons'][1]
  #B --> Button 2
  rv['ButtonB'] = jstat['Buttons'][2]
  #X --> Button 0
  rv['ButtonX'] = jstat['Buttons'][0]
  #Y --> Button 3
  rv['ButtonY'] = jstat['Buttons'][3]
  #BACK --> Button 8
  rv['ButtonBack'] = jstat['Buttons'][8]
  #START --> Button 9
  rv['ButtonStart'] = jstat['Buttons'][9]
  #Left shoulder --> Button 4
  rv['ButtonLShoulder'] = jstat['Buttons'][4]
  #Right shoulder --> Button 5
  rv['ButtonRShoulder'] = jstat['Buttons'][5]
  #Left thumb button --> Button 10
  rv['ButtonLThumb'] = jstat['Buttons'][10]
  #Right thumb button --> Button 11
  rv['ButtonRThumb'] = jstat['Buttons'][11]
  
  #Triggers
  #Triggers have a value range, [0 255]. Here we are mapping a trigger to
  #a button, so if the button is pressed, the trigger is full-down, else
  #the trigger is at rest.
  #Left trigger --> Button 6
  if jstat['Buttons'][6]:
   rv['TriggerL'] = 255
  else:
   rv['TriggerL'] = 0
  #Right trigger --> Button 7
  if jstat['Buttons'][7]:
   rv['TriggerR'] = 255
  else:
   rv['TriggerR'] = 0
  #Return the dictionary with the new state.
  return rv
  
#Each time the game requests the pad state, this function will be called and
#must return a pad state dictionary.
def get_state(dwUserIndex):
  '''Return the pad state'''
  #If the requested pad is the one we want to emulate. 
  if dwUserIndex == pad_id:
   #Poll the device (prepare to get new state)
   dinput.poll(dev_id)
   #Get the DirectInput state
   dinput_state = dinput.get_joystate2(dev_id)
   #Build the XInput state and return it
   return make_state(dinput_state)
  else:
   #Otherwise, return a default state.
   return sample_state

#This function is called when the game wants the pad to vibrate.
#The function MUST be defined, even if it does nothing like this one.
def set_state(dwUserIndex, state):
  '''Receive state from the game'''
  pass
  