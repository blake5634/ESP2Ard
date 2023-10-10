# This folder 
Under construction:  An example project for the ESP32 with the Arduino IDE.

# preparation
1) make another folder somewhere and clone ESP2Ard again in it.
2) delete the file `ESP2Ard.c`
3) Edit `ESP2Ard.h` and select `ARDUINO_PLATFORM`
4) Open an instance of arduino and open the arduino demo app: `> arduino ESP2Ard_Ard_AIDE.ino`
5) Configure `arduino` IDE (use Arduino IDE menus) for your arduino board and its port.
6) On your Arduino, open the serial monitor (double check the port number).

Now, returning to this folder, we work on the ESP32 being programmed with Arduino IDE

# instructions
1) copy ESP2ard.cpp and ESP2Ard.h into this directory
2) `> arduino ESP2Ard_ESP32_AIDE.ino`
3) Upload into your ESP32 board. (`> idf.py flash && idf.py monitor`)
6) The ESP32 will scan for open access points on wifi and send info to Arduino
via serial ports.
7) Observe that wifi network scan results from the ESP32 should display on Arduino Serial Monitor after being sent over using ESP2Ard
