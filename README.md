# ESP2Ard
A package for serial (small packets only) between ESP32 and Arduino.

The package consists of one code file and one `.h` file.

## Code files ESP2Ard.c / ESP2Ard.cpp / ESP2Ard.h

`ESP2Ard.c` and `ESP2Ard.cpp` are identical files.
You must use the ".c" name for building in ESP32 idf.py  environment and
the ".cpp" name for building in the Arduino environment (at least for
software serial).

`ESP2Ard.h` is used for all platforms/build systems.   It must be edited
to set a couple of  `#define`s for your specific setup.

# Initial Testing Suggestions
The `/TestsDemos` directory contains 4 apps to informally test and
demonstrate  `ESP2Ard`.   Please consult the [Wiring Setup doc](Doc/WiringInfo.md)
to connect your ESP32 and Arduino boards together.

# Sofware Step by Step

1) First we will get a test app running on an Arduino board (so far using
only SoftwareSerial).
    1) cd to

### Adding `ESP2Ard` to an EXISTING idf.py  project


If you have an existing ESP32 idf.py  project, this code (`ESP2Ard.c, ESP2Ard.h`)
can be added in the `main` directory.
Add `"ESP2Ard.c"` to your `CMakelist.txt` file in the `idf_component_register()` command with
the other source files.


### ESP32 LilyGo T-embed device w/ idf.py Environment


   ![T-embed by LilyGo](https://github.com/blake5634/DemoESP2Ard_ESP32_tembed/blob/main/LilyGoTembed.png?raw=true)


```
[ Update: LilyGo T-embed example won't likely work with other USB boards.
I'm now working on a lighter weight ESP-32 demo more similar to the
demo for ESP32 + Arduino IDE (AIDE) demo (ESP32Ard_ESP32_AIDE/). ]
```
I have implemented a working demo of 1-way communication (ESP32->Arduino) which relies on the 
[LilyGo T-Embed user-interface device.](https://github.com/Xinyuan-LilyGO/T-Embed).  The separate repository [DemoESP2Ard_ESP32_tembed](https://github.com/blake5634/DemoESP2Ard_ESP32_tembed) is a full idf.py  project which includes ESP2Ard and a demo app `tembed_main.c` in which you can
change numbers on the T-embed display and send them to Arduino using `ESP2Ard`.

This information is essentially repeated in
[TestsDemos/LilyGoTembed](TestsDemos/DemoLilygoTembed/README.md).

### Arduino Environment:  build for Arduino Board on Arduino IDE

1) Go to github and copy the link for repository `blake5634/ESP2Ard`

2) Create a folder named ESP2Ard, cd into it, and `git clone`.

3) Delete the file ESP2Ard.c (which is used only for ESP32)

4) Select the Arduino platform:  edit `ESP2Ard.h` to uncomment the  `#define` for

 `#define ARDUINO_PLATFORM`

 (don't forget to comment out

```
#define ESP32_IDF_PLATFORM        // ESP32 HW on IDF IDE
#define ESP32_Arduino_PLATFORM    // ESP32 HW on Arduino IDE
```
) so you are selecting ONLY the Arduino on Arduino IDE,
 and choose which type of Arduino serial port:

```
#define ARDUINO_SW_SERIAL
//#define ARDUINO_HW_SERIAL
```

(note: no code exists yet for Arduino hardware serial ports. Any
Arduino can use software serial ports on free I/O pins.
Arduino Uno can only use ARDUINO_SW_SERIAL because it's one hardware port is
dedicated to USB, but some other Arduino models have open hardware serial ports).

Also set serial communication parameters (baud rate etc.) in this file.

5) Our example project is ESP2Ard.ino.  This assumes another device is sending data TO the ardunio and echos it to the USB serial (use Serial  Monitor to see the received data). 

6) You may run this example project using:  `> arduino ESP2Ard.ino`
   
8) If you have an existing project somewhere else, copy `ESP2Ard.cpp` and `ESP2Ard.h` into the project folder containing your Arduino .ino file.

