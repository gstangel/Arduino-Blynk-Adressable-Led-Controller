import blynklib
import serial
import time

#pySerial instance for communication to the arduino
#Change 'com4' to reflex your arduino's current port
ser = serial.Serial('com4', 2400, timeout=1)

#Insert your Blynk Auth Token here
BLYNK_AUTH = ''

# initialize Blynk with the AUTH token
blynk = blynklib.Blynk(BLYNK_AUTH)


#register handler for brightness slider change at v0
@blynk.handle_event('write V0')
def brightness_slider_changed(pin, value):
    #Write the current pin and brightness through serial
    ser.write(bytes([int(pin),int(value[0]),int(value[0]),int(value[0])]))


#register handler for RGB widget change at v1
@blynk.handle_event('write V1')
def color_changed(pin, value):
    #Write the current pin, red value, green value, and blue value through serial
    ser.write(bytes([int(pin),int(value[0]),int(value[1]),int(value[2])]))
    #Change the mode on the app to Solid RGB
    blynk.virtual_write(2,0)
    #Change the mode label to Solid RGB
    blynk.virtual_write(3, "Solid RGB")
 

#register handler for mode widget change at v2
@blynk.handle_event('write V2')
def mode_slider_changed(pin, value):
    #Write the current pin and current mode through serial
    ser.write(bytes([int(pin),int(value[0]),int(value[0]),int(value[0])]))

    
    #Change the value of the mode display widget on the app with each change
    #If you add a custom mode, you need to add another elif statement
    #in order to control the current mode label on the app.
    if(value[0] == '0'):
        blynk.virtual_write(3, "Solid RGB")
    elif(value[0] == '1'):
        blynk.virtual_write(3, "BeatWave Slow")
    elif(value[0] == '2'):
        blynk.virtual_write(3, "BeatWave Medium")
    elif(value[0] == '3'):
        blynk.virtual_write(3, "BeatWave Fast")
    elif(value[0] == '4'):
        blynk.virtual_write(3, "Rainbow 1")
    elif(value[0] == '5'):
        blynk.virtual_write(3, "Rainbow 2")
    elif(value[0] == '6'):
        blynk.virtual_write(3, "Rainbow 3")
    elif(value[0] == '7'):
        blynk.virtual_write(3, "Rainbow w/ Glitter")
    elif(value[0] == '8'):
        blynk.virtual_write(3, "Confetti")
    elif(value[0] == '9'):
        blynk.virtual_write(3, 'BeatsPerMin Slow')
    elif(value[0] == '10'):
        blynk.virtual_write(3, 'BeatsPerMin Medium')
    elif(value[0] == '11'):
        blynk.virtual_write(3, 'BeatsPerMin Fast')
    elif(value[0] == '12'):
        blynk.virtual_write(3, 'SineWave Slow')
    elif(value[0] == '13'):
        blynk.virtual_write(3, 'SineWave Medium')
    elif(value[0] == '14'):
        blynk.virtual_write(3, 'SineWave Fast')
    elif(value[0] == '15'):
        blynk.virtual_write(3, 'Fire 1')
    elif(value[0] == '16'):
        blynk.virtual_write(3, 'Fire 2')
    elif(value[0] == '17'):
        blynk.virtual_write(3, 'Juggle 1 Slow')
    elif(value[0] == '18'):
        blynk.virtual_write(3, 'Juggle 1 Medium')
    elif(value[0] == '19'):
        blynk.virtual_write(3, 'Juggle 1 Fast')
    elif(value[0] == '20'):
        blynk.virtual_write(3, 'Juggle 2 Slow')
    elif(value[0] == '21'):
        blynk.virtual_write(3, 'Juggle 2 Medium')
    elif(value[0] == '22'):
        blynk.virtual_write(3, 'Juggle 2 Fast')


if __name__ == '__main__':
    #infinite loop to run Blynk and update changes
    while True:
        blynk.run()