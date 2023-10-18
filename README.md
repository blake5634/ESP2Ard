# ESP2Ard
A package for serial communication between ESP32 and Arduino.

## Table of Contents

1. [Introduction](https://github.com/blake5634/ESP2Ard#Introduction)

2. [Code Files](https://github.com/blake5634/ESP2Ard#Code-files)


3. [Adding `ESP2Ard` to an EXISTING idf.py  project](https://github.com/blake5634/ESP2Ard#adding-esp2ard-to-an-existing-idfpy--project)

4. [Trying it Out](https://github.com/blake5634/ESP2Ard#trying-it-out-with-Test-and-Demo-Apps)

5. [Software Building and Installation](https://github.com/blake5634/ESP2Ard#software-building-and-installation)


## Introduction

With this package, you can connect an ESP32 based system to an Arduino for 3-wire bi-directional
communication supported by UART hardware (and also Arduino's `SoftwareSerial` package).  Data is sent
in packets with up to 256 bytes of data.  Utilities are provided to assemble packets and validate packets.
Packets have basic error detection in form of a checksum and byte count.

So far three configurations are supported:
   1. Arduino hardware with the Arduino Integrated Development Environment (IDE).
   2. ESP32 hardware with the Arduino Integrated Development Environment (IDE).
   3. ESP32 hardware with the IDF (`idf.py`) command line tool chain.

There is a unit test package for parts of `ESP2Ard` which tests the packet utility functions.  This runs
on Arduino only as of now.  Test results are printed through the Arduino's USB/Serial connection to the
IDE's "SerialMonitor".


## Code files

### ESP2Ard.c / ESP2Ard.cpp / ESP2Ard.h
The package consists of one code file and one `.h` file.

`ESP2Ard.c` and `ESP2Ard.cpp` are identical files.
You must use the ".c" name for building in ESP32 idf.py  environment and
the ".cpp" name for building in the Arduino environment (at least for
software serial).

`ESP2Ard.h` is used for all platforms/build systems.   It must be edited
to set a couple of  `#define`s for your specific setup.



## Adding `ESP2Ard` to an EXISTING idf.py  project

If you have an existing ESP32 idf.py  project, this code (`ESP2Ard.c, ESP2Ard.h`)
can be added in the `main` directory.
Add `"ESP2Ard.c"` to your `CMakelist.txt` file in the `idf_component_register()` command with
the other source files.



## Trying it out with Tests and Demo Apps

As of 18-Oct-23 ONLY the Arduino project below is programmed to RECEIVE test data and
ESP32 boards are programmed to SEND data.  So, build and flash an Arduino first, then build
any platform and attempt to send data back to that Arduino.

Please consult the [Wiring Setup doc](Doc/WiringInfo.md)
to physically connect your ESP32 and Arduino boards together.

Only the unit tests in `unitTests_AIDE` are actually formal software tests.   The `TestsDemos`
directory contains informal tests which send data and receive data.  You can verify that they work
by watching serial debugging output on the receiving end.


The Test/Demo examples so far are:

1) Arduino Uno using AIDE  (`TestsDemos/ESP2Ard_Ard_AIDE`)

2) ESP32 Using Espressif's `idf.py` toolchain (`TestsDemos/ESP2Ard_ESP32_IDF`)

3) ESP32 Using Arduino's IDE (AIDE) (`TestsDemos/ESP2Ard_ESP32_AIDE`)

4) Lilygo's T-Embded device (ESP32-based) with `idf.py` (`TestsDemos/DemoLilygoTembed`)

The Arduino example has been tested only on Arduino Uno.   The ESP32 hardware examples (2,3 above)
should work with a generic ESP32 board (tested with the ESP32-S3 variant).   There is no support
yet for Arduino hardware serial ports (UARTs) because Arduino Uno can only use Software Serial for this.

## Software Building and installation

Once you choose
The `ESP2Ard.*` files in the top level and  be copied into your chosen Test or Demo. See instructions.

Go to `README.md` in the appropriate subdirectory in `TestsDemos` for instructions.
