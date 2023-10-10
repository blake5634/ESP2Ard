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



## Adding `ESP2Ard` to an EXISTING idf.py  project

If you have an existing ESP32 idf.py  project, this code (`ESP2Ard.c, ESP2Ard.h`)
can be added in the `main` directory.
Add `"ESP2Ard.c"` to your `CMakelist.txt` file in the `idf_component_register()` command with
the other source files.



## Initial Testing Suggestions
The `/TestsDemos` directory contains 4 apps to informally test and
demonstrate  `ESP2Ard`.   Please consult the [Wiring Setup doc](Doc/WiringInfo.md)
to connect your ESP32 and Arduino boards together.


## Software Building and installation

So far we support 4 configurations ("AIDE" == Arduino IDE 1.8)

1) Arduino Uno using AIDE  (`TestsDemos/ESP2Ard_Ard_AIDE`)

2) ESP32 Using Espressif's `idf.py` toolchain (`TestsDemos/ESP2Ard_ESP32_IDF`)

3) ESP32 Using Arduino's IDE (AIDE) (`TestsDemos/ESP2Ard_ESP32_AIDE`)

4) Lilygo's T-Embded device (ESP32-based) with `idf.py` (`TestsDemos/DemoLilygoTembed`)

Go to the appropriate subdirectory for instructions.
