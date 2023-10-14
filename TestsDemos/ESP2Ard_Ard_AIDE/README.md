# ESP2Ard for the Arduino

Use this part to install a test app on the Arduino.

1) Copy `ESP2Ard.cpp` and `ESP2Ard.h` from the main  level to this
directory. (Do not copy the file ESP2Ard.c (which is used only for ESP32)).

4) In the Arduino platform, open `ESP2Ard_Ard_AIDE.ino`

5) Select the correct board and port for your Arduino setup.

3) You should see 3 code tabs. Edit `ESP2Ard.h` to uncomment the  `#define` for

 `#define ARDUINO_PLATFORM`

 (and don't forget to comment out

```
//#define ESP32_IDF_PLATFORM        // ESP32 HW on IDF IDE
//#define ESP32_Arduino_PLATFORM    // ESP32 HW on Arduino IDE
```
) so you are selecting ONLY the Arduino on Arduino IDE,

4) Choose which type of Arduino serial port you will use:

```
#define ARDUINO_SW_SERIAL
//#define ARDUINO_HW_SERIAL
```

(note: no `ESP32Ard` code exists yet for Arduino hardware serial ports.
Software Serial ports means that Arduino is using software to send and
receive serial data over two general purpose digital I/O pins.  Hardware serial ports are a subset of pins which are supported by an internal hardware device which makes serial data faster and more efficient.
Any
Arduino can use software serial ports on free I/O pins. Only some Arduinos
have usable hardware serial ports.
Arduino Uno can only use ARDUINO_SW_SERIAL because its one hardware port is
dedicated to USB, but some other Arduino models have open hardware serial ports).

5) Set serial communication parameters (baud rate etc.) in this file (best to
start testing at a modest 9600 baud rate. Increase speed if needed after everything works).

5) Our example project is ESP2Ard.ino.  This app assumes another device is sending data TO the ardunio and it will echo the received data to the USB serial (use Arduino's Serial  Monitor to see the received data).

7) The project should build and upload into Arduino.   Don't forget to
open the Serial Monitor to see the apps output.

8) Now you are ready to prepare an ESP32 to talk to this arduino!
