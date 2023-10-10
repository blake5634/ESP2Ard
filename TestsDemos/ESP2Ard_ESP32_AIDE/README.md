# This folder  (UNDER CONSTRUCTION)

This is a demo for the ESP32 side of an ESP32-->Arduino link which is built using the Arduino ide 
configured for ESP32 develoment.   This code will send data to an Arduino already running the version in
an adjacent folder.  Please make sure the receiving arduino is running and wired properly before proceding. 

## Note: this must be used with Arduino 1.18.  Arduino IDE version 2.0 does not yet support ESP32

## instructions
1) copy ESP2ard.cpp and ESP2Ard.h from one level up into this directory.
2) Edit `ESP2Ard.h` to uncomment `#define ESP32_Arduino_PLATFORM` and comment out the other platforms.
3) start Arduino IDE: `> arduino ESP2Ard_ESP32_AIDE.ino`
4) Make sure it is configured for your ESP32's serial port and correct ESP32 board. 
5) Upload into your ESP32 board. (Arduino right-arrow).
6) The ESP32 will scan for open access points on wifi and send info to Arduino
via serial ports.
7) Observe that wifi network scan results from the ESP32 should display on Arduino Serial Monitor after being sent over using ESP2Ard.
