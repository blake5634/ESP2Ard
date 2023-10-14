/*  ESP32 sketch for Arduino IDE to test ESP2Ard serial coms library
 *
 *  Please uncomment #define ESP32_Arduino_PLATFORM  in
 *     ESP2Ard.h (and comment out the other PLATFORMs)
 *
 *
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include
 *
 * Thanks to: https://www.makerguides.com/how-to-install-the-esp32-arduino-core-in-the-arduino/
 */
#include "WiFi.h"
#include "ESP2Ard.h"

void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");

    // set up UART serial port
    EA_init_serial(ESP_PIN_RX, ESP_PIN_TX);
}

void loop()
{
    Serial.println("scan start");

    // packet for serial com to Arduino via UART
    EA_msg_byte pkt[ESP2Ard_max_payload_size];

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) { 
        EA_msg_pkt_build(pkt,"no networks found");
        Serial.println("no networks found");
    } else {
        Serial.print(n); printf("%d",n); printf("(local) nets found");
        Serial.println("(local) networks found:");
        int plen = EA_msg_pkt_build(pkt, "(rmt) nets:");
        int code = EA_test_packet(pkt);
        if (code < 0){
            Serial.println("network found message packet defective...");
        }
        EA_write_pkt_serial(pkt,plen); // send!
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found    
           // char msg[] =  "                                      ";
           // sprintf(msg, "%2d: %20s",i+1, WiFi.SSID(i));
            char msg[] = "test msg only\n";
            int plen = EA_msg_pkt_build(pkt, msg);
            int code = EA_test_packet(pkt);
            if (code < 0){
                Serial.println("SSID message packet defective...");
            }
            EA_write_pkt_serial(pkt,plen); // send!
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);
}
