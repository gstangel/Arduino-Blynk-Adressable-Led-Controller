import blynklib
import serial
import time
import logging

# start logging
_log = logging.getLogger('BlynkLog')
logFormatter = logging.Formatter("%(asctime)s [%(levelname)s]  %(message)s")
consoleHandler = logging.StreamHandler()
consoleHandler.setFormatter(logFormatter)
_log.addHandler(consoleHandler)
_log.setLevel(logging.DEBUG)

#pySerial instance for communication to the arduino
ser = serial.Serial('com4', 2400, timeout=1)

#blynk auth
BLYNK_AUTH = 'xFeD7GY9pjH7xqg5iZ3LejE5c69-m4YK'

# initialize Blynk
blynk = blynklib.Blynk(BLYNK_AUTH)

#adressable led output pin on arduino
LED_PIN = 7

#global variables for virtual pins
currentRed,currentGreen,CurrentBlue = 255,255,255 #start off with plain white
brightness = 255 #start off with max bright
mode = 0#start off on solid color mode

#print pin values for debugging
WRITE_EVENT_PRINT_MSG = "[WRITE_VIRTUAL_PIN_EVENT] Pin: V{} Value: '{}'"

#

#register handler for brightness sldier change at v0
@blynk.handle_event('write V0')
def brightness_slider_changed(pin, value):
    ser.write(bytes([int(pin),int(value[0]),int(value[0]),int(value[0])]))

    
        
        
    
    
@blynk.handle_event('write V1')
def color_changed(pin, value):
    ser.write(bytes([int(pin),int(value[0]),int(value[1]),int(value[2])]))
    blynk.virtual_write(2,0)
    
    
  
 
    
@blynk.handle_event('write V2')
def mode_slider_changed(pin, value):
    ser.write(bytes([int(pin),int(value[0]),int(value[0]),int(value[0])]))
    
    if(value[0] == '0'):
        blynk.virtual_write(3, "Solid RGB")
    elif(value[0] == '1'):
        blynk.virtual_write(3, "BeatWave")
    elif(value[0] == '2'):
        blynk.virtual_write(3, "Rainbow March")
    elif(value[0] == '3'):
        blynk.virtual_write(3, "Rainbow Simple")
    elif(value[0] == '4'):
        blynk.virtual_write(3, "Rainbow W/ Glitter")
    elif(value[0] == '5'):
        blynk.virtual_write(3, "Confetti")
    elif(value[0] == '6'):
        blynk.virtual_write(3, "BPM")
    elif(value[0] == '7'):
        blynk.virtual_write(3, "Sine Wave RGB")
    elif(value[0] == '8'):
        blynk.virtual_write(3, "Campfire")
    elif(value[0] == '9'):
        blynk.virtual_write(3, 'Sunrise')

if __name__ == '__main__':
    while True:
        blynk.run()