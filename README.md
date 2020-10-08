# Arduino Based Individually Addressable LED Controller with Blynk

Control individually addressable LED's from a smartphone using an Arduino and any device capable of running python scripts using serial communication.

## Why?
Prebuilt controllers are a quick and easy way to control individually addressable LED's, but they often lack customizability and are generally unreliable.  

This setup provides a seamless way to control the LED's from anywhere in the world using a smartphone, and has over 20 custom effects included.  

My desktop is always on so I chose to run the host there, but you can also **use a Raspberry Pi as a host without any changes to the application.**

## Basics
There are three components that need to be in place for the controller to work.
* Linux/Mac/Windows machine running the [Blynk](https://blynk.io/) python script used as the Master
* [Arduino](https://www.arduino.cc/) or a similar microcontroller used as the Slave
* [Blynk](https://blynk.io/) app installed on a smartphone

## Setup
**Before doing anything, clone this repository and save it to your machine**

Let's start this guide by getting the Blynk setup on the smartphone.  After downloading the app and making an account, initialize a new project with your respective hardware, and select 'USB' for the connection type.  You should now have a blank app and an auth token.

Next, clone my Blynk App using this QR code by scanning this QR Code inside of Blynk  
![alt text](https://github.com/gabrielsaintangel/Arduino-Adressable-Led-Controller/blob/main/qr.jpg?raw=false=50x50)

**You should now have the Controller Setup on Blynk, and have a Auth Token emailed to you that will be used in the python script to communicate with your instance of the app.**


### Lets get the Arduino setup. ### 

_controller.ino_ contains all the custom patterns which will be available on your controller, as well as the logic for communicating between _blynkmaster.py_ and your Arduino through USB Serial Communication.  

If you don't have the Arduino IDE downloaded, download it now.
We need to install one package inside the Arduino IDE before we can upload the program.  

Open _controller.ino_ in the Arduino IDE and open the Library Manager by navigating to _Sketch -> 
Include Libraries -> Manage Libraries_  

Install the latest version of _FastLed_ which was created by Daniel Garcia.  

There is some setup inside the code with which you need to modify before you can control your LEDS.
#### In the header:
* ```#define LED_DT 7 ``` designates which pin will be used as the data pin for the LEDs
* ```#define LED_CK 11``` designates which pin will be used for the clock pin, if any.
   * _WS2812 LEDs don't use a clock pin, APA102 LEDs do._
* ```#define COLOR_ORDER GRB``` designates the order that your led strip interprets RGB values
   * _It should be GRB if you are using WS2812 LEDs, or BGR if you are using APA102 LEDs_  
* ```#define NUM_LEDS 150``` tells the program how many addressable LEDS are in your strip.
* ```LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);```
#### In the setup method:
You need to decide which of the following methods you need to use in order to initialize your LED strip in FastLED
* ```LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);``` This is to be used with WS2812.     
* ```LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);``` This is to be used with APA102
   * Uncomment the method for your respective LED strip, and comment out the unused one.

#### You are now ready to upload to the Arduino
* Plug your Arduino into the computer using a USB A-B cable
* Select your board type in _Tools -> Board_  
* Select your port in _Tools -> Port_  
***NOTE: Remember what port you are on, as this will be used in the blynkmaster.py script to initialize serial communication***
* Compile and Upload the file to the Arduino

### Lets now get the Python Script Setup. ###
After setting up the Arduino, install the requirements by running the following command in a terminal window inside the repository's folder on your machine to install the python dependencies
```
pip install -r requirements.txt
```
Next, open _blynkmaster.py_ in IDLE or your favorite Python IDE and add the AUTH token that was sent to your email to the _BLYNK_AUTH_ variable 
```
#Insert your Blynk Auth Token here
BLYNK_AUTH = 'xZeZ7ZY9pjZ4xqg5iZ3JejE5c69-m4YZ'
```

Finally, tell the program what port your Arduino is by editing the following variable  
```
ser = serial.Serial('com4', 2400, timeout=1)
```
_'com4'_ tells where to initialize serial communication for the Arduino, change this to reflect the port that was shown in the Arduino IDE.

#### You are now ready to begin controlling the LED's
* Plug your LED's data line into the respective port on your Arduino, which as designated in the Arduino Setup Section  
* Plug the LED's clock line into the respective port on the Arduino (only required if using APA102 LEDs)
* Make sure your Arduino is plugged into the the host machine, and that the ports are setup correctly
* **Run the _blynkmaster.py_ script**
* Open the Blynk Project on your smartphone
* ***You should now be able to control your LEDs though the interface on the app!***  


## Customization

**The FastLED library allows for a virtually unlimited number of custom patterns**  

* In order to add custom patterns, you need to:
   * Add a animation function to the *controller.ino* file
   * Add a case statement inside the main loop for the new animation
   * Add one the range of the *mode* widget on the Blynk App
   * Add an elif statement to the ```@blynk.handle_event('write V2')``` method in the blynkmaster.py script to change the label for the current mode.

The current patterns on a Arduino Uno only take up a fraction of the max memory, so there is room for many more custom animations.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
