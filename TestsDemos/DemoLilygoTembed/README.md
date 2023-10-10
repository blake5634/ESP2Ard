#  Lilygo T-Embed Demo


   ![T-embed by LilyGo](../../Doc/LilyGoTembed.png?raw=true)


```
[ Caution: This LilyGo T-embed example won't likely work with other ESP32 boards
because of dependencies on special board hardware.
For a lighter weight ESP-32 demo, more likely to work on generic ESP32 boards,
see ../ESP32Ard_ESP32_IDF/ or /ESP32Ard_ESP32_AIDE. ]
```

This code is set up to build on IDF. IDF is a professional grade tool chain for building and flashing ESP32s.

First set up your hardware along the lines of [Wiring Information](../../Doc/TestSetupESP2Ard.png).


This directory builds a working demo of 1-way communication (ESP32->Arduino) which relies on the
[LilyGo T-Embed user-interface device.](https://github.com/Xinyuan-LilyGO/T-Embed).  This is a full `idf.py`  project which includes ESP2Ard and a demo app `tembed_main.c` in which you can
change numbers on the T-embed display and send them to Arduino using `ESP2Ard`.



## Software Step-by-step

Here is a linux-oriented step-by-step for getting this to run.

1) If you don't already have `idf.py` follow these steps
If you already have it, go to next step.
    1) This demo is based on the [LilyGo T-Embed repository](https://github.com/Xinyuan-LilyGO/T-Embed/tree/main)

    2) First install ESP-IDF using [these instructions from Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

    3) Follow their documentation and build, flash, and test the "hello world" example.  This will make sure you get the details such as correct USB tty  device for uploading code to ESP32.

    4) Then, navigate to the [example](https://github.com/Xinyuan-LilyGO/T-Embed/tree/main/example/esp-idf-v5.0) of an `idf.py` project which includes nice graphics and UI elements.

    5) Get that example (esp-idf-v5.0) working on your T-embed hardware (see AliExpress or Amazon). Observe UI elements change on its screen when you move the scroll wheel etc.

6) Now navigate to this directory.

9) Initialize the idf.py environment with `>get_idf` (I find you have to do this again sometimes if your computer goes to sleep and you wake it up).  Probably also a good idea to do `>idf.py fullclean` to start fresh.

8) Set the target:
`> export IDF_TARGET=esp32s3`  (T-embed uses the S3-version).

7) try to build it! (`> idf.py build`). In case of build problems see Espressif's documentation and ask ChatGPT.

8) try to flash it into your T-embed.
(`> idf.py -p /dev/ttyXXX flash && idf.py monitor`)
(replace XXX with your devices serial port).

9) Assuming successful flash: T-embed App guide for this code:

    1. You should see three buttons.  You can move a highlight outline to select one by turning the T-embed wheel.
    2. Move to `Set Name' and press button in center of wheel.  Now, the outline changes color and when you turn the wheel some names should change in the bottom row.
    3. Press center button again to get back to icon selection.
    5. Turn the wheel until "HH:MM" is selected in a red outline.
    7. Press the button - the outline turns green and you are in the "HH:MM" button.
    9. Turn the wheel and the time changes from 06:30 as you turn the wheel
    11. Press the button again and you go back to red outline for button select.

#### THIS IS WHEN THE SERIAL DATA IS SENT OUT ON YOUR PORT (the new time value is sent in a single packet)

10)  Observe data received by the Arduino on your laptop through USB via the Serial Monitor.


The app (`tembed_main.c`) will let you set a time (HH:MM) using a rotary dial.
1) Rotate dial until
the (HH:MM) window is outlined.

2) Press the dial button to select.

3) Rotate dial to set time

4) Press button to save new time

5) When you save the new time (press button), it is sent
to the Arduino using `ESP2Ard`.


