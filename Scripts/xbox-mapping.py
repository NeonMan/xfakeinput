#
# xbox-mapping.py
#
# This script maps an XBox pad as an XBox pad. Why? Because we can.


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
#in this case 'Xbox 360 Controller for Windows'
instance_name = "Controller (XBOX 360 For Windows)"
dev_id = dinput.device_by_instance(instance_name)

#Which of the four pads we want to emulate.
#Remember that pad numeration starts at 0. therefore:
#Pad 1 --> pad_id 0
#Pad 2 --> pad_id 1
#Pad 3 --> pad_id 2
#Pad 4 --> pad_id 3
pad_id = 0

#Configure which pads will be left unemulated.
#If true, all calls to the selected pad will be pass-through
#to the original XInput API.
xfi.set_passthrough( (False, True, True, True) )

#Write some info to the log
print("Pad instance '%s' maps to device #%d" % (instance_name, dev_id))

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
  #Right thumb X --> X-Rotation
  rv['ThumbRX'] = xfi.long_to_axis(jstat['Rx'])
  #Right thumb Y --> Y-Rotation
  #DirectInput assigns the names, not my fault.
  rv['ThumbRY'] = -xfi.long_to_axis(jstat['Ry'])
  
  #Buttons
  #Button mapping is pretty straightforward
  #A --> Button 0
  rv['ButtonA'] = jstat['Buttons'][0]
  #B --> Button 1
  rv['ButtonB'] = jstat['Buttons'][1]
  #X --> Button 2
  rv['ButtonX'] = jstat['Buttons'][2]
  #Y --> Button 3
  rv['ButtonY'] = jstat['Buttons'][3]
  #BACK --> Button 6
  rv['ButtonBack'] = jstat['Buttons'][6]
  #START --> Button 7
  rv['ButtonStart'] = jstat['Buttons'][7]
  #Left shoulder --> Button 4
  rv['ButtonLShoulder'] = jstat['Buttons'][4]
  #Right shoulder --> Button 5
  rv['ButtonRShoulder'] = jstat['Buttons'][5]
  #Left thumb button --> Button 8
  rv['ButtonLThumb'] = jstat['Buttons'][8]
  #Right thumb button --> Button 9
  rv['ButtonRThumb'] = jstat['Buttons'][9]
  
  #Triggers
  #When reading the triggers using DirectInput, they are treated as the
  #same axis (Z-Axis). Left trigger pulls it down and Right pulls it up
  trigger_axis = jstat['Z']
  if trigger_axis > 60000:
   rv['TriggerL'] = 255
  else:
   rv['TriggerL'] = 0;
  if trigger_axis < -1000:
   rv['TriggerR'] = 255
  else:
   rv['TriggerR'] = 0;
  #Return the dictionary with the new state.
  
  #D-Pad
  # Unimplemented, must interpret the first POV.
  
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
  