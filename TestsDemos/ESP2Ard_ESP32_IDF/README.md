(This is adapted from the official Espressif example `wifi/scan`)

## STATUS:    This WILL be modified to be an ESP2Ard example that can run on generic ESP32S3 boards.

No changes have been made to this running example `main/scan.c` yet.



| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- |

# `scan.c` is a Wi-Fi Scan Example

This example shows how to scan your wifi environment for the available set of access-points (wifi networks).

## How to use example

Before project configuration and build, be sure to set the correct chip target using `idf.py set-target <chip_name>`.

### Hardware Required

* A development board with ESP32/ESP32-S2/ESP32-C3 SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.).
* A USB cable for Power supply and programming.

### Configure the project

Open the project configuration menu (`idf.py menuconfig`).

In the `Example Configuration` menu:

* Set the Example configuration.
    * Use `Max size of scan list` to set the maximum nunber of access points in the list.

### Build and Flash

Build the project and flash it to the board, then run the monitor tool to view the serial output:

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for all the steps to configure and use the ESP-IDF to build projects.

* [ESP-IDF Getting Started Guide on ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)
* [ESP-IDF Getting Started Guide on ESP32-S2](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)
* [ESP-IDF Getting Started Guide on ESP32-C3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/index.html)

## Example Output

As you run the example, you will see the following log: (specific to the `Esp2Ard` version!)

```
I (438) wifi:wifi driver task: 3fca9bfc, prio:23, stack:6656, core=0
I (448) wifi:wifi firmware version: 4fcdbd6
I (448) wifi:wifi certification version: v7.0
I (448) wifi:config NVS flash: enabled
I (448) wifi:config nano formating: disabled
I (448) wifi:Init data frame dynamic rx buffer num: 32
I (458) wifi:Init static rx mgmt buffer num: 5
I (458) wifi:Init management short buffer num: 32
I (468) wifi:Init dynamic tx buffer num: 32
I (468) wifi:Init static tx FG buffer num: 2
I (478) wifi:Init static rx buffer size: 1600
I (478) wifi:Init static rx buffer num: 10
I (478) wifi:Init dynamic rx buffer num: 32
I (488) wifi_init: rx ba win: 6
I (488) wifi_init: tcpip mbox: 32
I (488) wifi_init: udp mbox: 6
I (498) wifi_init: tcp mbox: 6
I (498) wifi_init: tcp tx win: 5744
I (508) wifi_init: tcp rx win: 5744
I (508) wifi_init: tcp mss: 1440
I (508) wifi_init: WiFi IRAM OP enabled
I (518) wifi_init: WiFi RX IRAM OP enabled
I (518) phy_init: phy_version 610,2bff4c8,Jul 27 2023,20:22:14
I (568) wifi:mode : sta (34:85:18:44:33:0c)
I (568) wifi:enable tsf
I (3068) scan: Total APs scanned = 8
I (3068) scan: SSID             NotAvailable
I (3068) scan: RSSI             -77
I (3068) scan: Channel          6
I (3068) scan: SSID             bsmt_guest
I (3068) scan: RSSI             -85
I (3078) scan: Channel          1
I (3078) scan: SSID             bsmt_guest
I (3078) scan: RSSI             -86
I (3088) scan: Channel          11
I (3088) scan: SSID             hannaford1
I (3098) scan: RSSI             -86
I (3098) scan: Channel          11
I (3098) scan: SSID             hannaford1
I (3108) scan: RSSI             -90
I (3108) scan: Channel          6
I (3108) scan: SSID             Main House
I (3118) scan: RSSI             -95
I (3118) scan: Channel          3
I (3118) scan: SSID             lee-weizenbaum-guest
I (3128) scan: RSSI             -96
I (3128) scan: Channel          1
I (3128) scan: SSID             Series of Tubes
I (3138) scan: RSSI             -96
I (3138) scan: Channel          8
I (3148) main_task: Returned from app_main()
```
