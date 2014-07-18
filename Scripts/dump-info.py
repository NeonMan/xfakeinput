#
# dump-info.py
#
# This script obtains all the info from every detected device and
# logs it to a file. 
#
#
import dinput
import sys
import xfi

#If an error happens, it will silently pass, unless we redirect the standard
#output (logs) and the standard error (errors) to files. Here we tell the 
#script to save those streams to files
sys.stderr = open("stderr.log", "w") #<-- Errors will be saved to 'stderr.log'
sys.stdout = open("stdout.log", "w") #<-- Logs will be saved to 'stout.log'

#Will emulate the first pad.
xfi.set_passthrough( (False, True, True, True) )

#Get device count
device_count = dinput.num_devices()
print("Devices detected: ", device_count)

#For each device, obtain info
for i in range(device_count):
 info = dinput.device_info(i)
 print("%s:" % info['ProductName'])
 keys = list(info.keys())
 keys.sort()
 for k in keys:
  print("    %s: %s" % (k, info[k]))
print("-----------------------------------------------------")

#For each call, dump the state
def get_state(dwUserIndex):
 for n in range(device_count):
  dinput.poll(n)
  try:
   #Keyboard is unimplemented and fails, hence the try/except
   state = dinput.get_state(n)
  except IOError as e:
   print("IOError:", e)
   continue
  except Exception as e:
   print("Error:", e)
   continue
  info = dinput.device_info(n)
  print("%s:" % (info["InstanceName"]))
  keys = list(state.keys())
  keys.sort()
  for k in keys:
   print("    %s: %s" % (k, state[k]))
 print("-----------------------------------------------------")
 return None
  
 
def set_state(dwUserIndex, state):
 pass
 