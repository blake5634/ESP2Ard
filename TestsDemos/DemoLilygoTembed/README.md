#  Lilygo T-Embed Demo


   ![T-embed by LilyGo](../../Doc/LilyGoTembed.png?raw=true)


```
[ Caution: This LilyGo T-embed example won't likely work with other ESP32 boards
because of dependencies on special board hardware.
For a lighter weight ESP-32 demo more likely to work on generic ESP32 boards,
see (../ESP32Ard_ESP32_IDF/). ]
```

This code is set up to build on IDF. IDF is a professional grade tool chain for building and flashing ESP32s.

First set up your hardware along the lines of [Wiring Information](../../Doc/TestSetupESP2Ard.png).


This directory builds a working demo of 1-way communication (ESP32->Arduino) which relies on the
[LilyGo T-Embed user-interface device.](https://github.com/Xinyuan-LilyGO/T-Embed).  This is a full `idf.py`  project which includes ESP2Ard and a demo app `tembed_main.c` in which you can
change numbers on the T-embed display and send them to Arduino using `ESP2Ard`.



## Software Step-by-step

Here is a linux-oriented step-by-step for getting this to run.

1) This demo is based on the [LilyGo T-Embed repository](https://github.com/Xinyuan-LilyGO/T-Embed/tree/main)

2) First install ESP-IDF using [these instructions from Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

3) Follow their documentation and build, flash, and test the "hello world" example.  This will make sure you get the details such as correct USB tty  device for uploading code to ESP32.

4) Then, navigate to the [example](https://github.com/Xinyuan-LilyGO/T-Embed/tree/main/example/esp-idf-v5.0) of an `idf.py` project which includes nice graphics and UI elements.

5) Get that example (esp-idf-v5.0) working on your T-embed hardware (see AliExpress or Amazon). Observe UI elements change on its screen when you move the scroll wheel etc.

6) Now navigate to your clone of this repo.   ESP2Ard is already included but may not be the latest version.  The next two steps might not be needed but it would be better to get the latest version:

7) go into `main` and clone [Esp2Ard](https://github.com/blake5634/ESP2Ard).

8) Copy `ESP2Ard.c` and `ESP2Ard.h` back up in to `main` (do not copy the `.cpp` version per note below).

9) go back to the project level (`DemoEsp2Ard_ESP32_tembed`) and initialize the idf.py environment with `>get_idf` (I find you have to do this again sometimes if your computer goes to sleep and you wake it up).  Probably also a good idea to do `>idf.py fullclean` to start fresh.

7) try to build it! (`> idf.py build`). In case of build problems see Espressif's documentation and ask ChatGPT.

8) try to flash it into your T-embed.

9) T-embed App guide for this code:

1. You should see three buttons.  You can move a highlight outline to select one by turning the T-embed wheel.
2. Move to `Set Name' and press button in center of wheel.  Now, the outline changes color and when you turn the wheel some names should change in the bottom row.
3. Press center button again to get back to icon selection.
5. Turn the wheel until "HH:MM" is selected in a red outline.
7. Press the button - the outline turns green and you are in the "HH:MM" button.
9. Turn the wheel and the time changes from 06:30 as you turn the wheel
11. Press the button again and you go back to red outline for button select.

#### THIS IS WHEN THE SERIAL DATA IS SENT OUT ON YOUR PORT (the new time value is sent in a single packet)

12.  Observe data received by the Arduino on your laptop through USB via the Serial Monitor.


The app (tembed_main.c) will let you set a time (HH:MM) using a rotary dial.  When you save the new time, it is sent
to an Arduino using ESP2Ard.


