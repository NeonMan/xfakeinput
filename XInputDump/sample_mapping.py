import xfi
import dinput
import sys

#Log to file
sys.stderr = open("stderr.log", "w")
sys.stdout = open("stdout.log", "w")

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

dev_id = dinput.device_by_instance("AUTO PAD")

def make_state(jstat):
  rv = {}
  #Analog sticks
  rv['ThumbLX'] = xfi.long_to_axis(jstat['X'])
  rv['ThumbLY'] = -xfi.long_to_axis(jstat['Y'])
  rv['ThumbRX'] = xfi.long_to_axis(jstat['Z'])
  rv['ThumbRY'] = -xfi.long_to_axis(jstat['Rz'])
  
  #Buttons
  rv['ButtonA'] = jstat['Buttons'][1]
  rv['ButtonB'] = jstat['Buttons'][2]
  rv['ButtonX'] = jstat['Buttons'][0]
  rv['ButtonY'] = jstat['Buttons'][3]
  rv['ButtonBack'] = jstat['Buttons'][8]
  rv['ButtonStart'] = jstat['Buttons'][9]
  rv['ButtonLShoulder'] = jstat['Buttons'][4]
  rv['ButtonRShoulder'] = jstat['Buttons'][5]
  rv['ButtonLThumb'] = jstat['Buttons'][10]
  rv['ButtonRThumb'] = jstat['Buttons'][11]
  
  #Triggers
  if jstat['Buttons'][6]:
   rv['TriggerL'] = 255
  else:
   rv['TriggerL'] = 0
  
  if jstat['Buttons'][7]:
   rv['TriggerR'] = 255
  else:
   rv['TriggerR'] = 0 
  return rv
  

def get_state(dwUserIndex):
  '''Return the pad state'''
  if dev_id == None:
   return sample_state
   
  if dwUserIndex == 0:
   dinput.poll(dev_id)
   dinput_state = dinput.get_joystate2(dev_id)
   return make_state(dinput_state)
  else:
   return sample_state

def set_state(dwUserIndex, state):
  '''Receive state from the game'''
  pass
  