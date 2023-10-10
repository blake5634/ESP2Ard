# How to wire up ESP32 and Arduino (Uno) for ESP2ard testing


We will use the [Lilygo T-embed](https://www.lilygo.cc/products/t-embed) ESP32-based user I/O kit. (I'm aiming for a nice way to add a UI for configuring an Arduino app without reflashing.)

   ![T-embed by LilyGo](https://github.com/blake5634/DemoESP2Ard_ESP32_tembed/blob/main/LilyGoTembed.png?raw=true)



##### Suggested hardware setup (works for me).

   - I soldered 20-AWG wires into pin holes 16, 17 and GND on the T-embed (see [pinout](https://github.com/Xinyuan-LilyGO/T-Embed/blob/main/image/T-Embed1.png))
   - plug them in to pins 2,3,GND respectively on Arduino Uno.

   ![Wiring Diagram](https://github.com/blake5634/DemoESP2Ard_ESP32_tembed/blob/main/TestSetupESP2Ard.png?raw=true)
