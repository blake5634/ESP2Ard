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
    EA_msg_byte  pkt[ESP2Ard_max_payload_size]={0};

    // WiFi.scanNetworks will return the number of networks found
    int nnets = WiFi.scanNetworks();
    Serial.println("scan done");
    if ( nnets == 0) {
        EA_msg_pkt_build(pkt,"no networks found");
        Serial.println("no networks found");
    } else {
        Serial.print(nnets); printf("%d",nnets); printf("(local) nets found");
        Serial.println("(local) networks found:");
        int plen = EA_msg_pkt_build(pkt, "(remote wifi) access points:");
        int code = EA_test_packet(pkt);
        if (code < 0){
            Serial.println("network found message packet defective...");
        }
        EA_write_pkt_serial(pkt,plen); // send!
        
        for (int i = 0; i < nnets; ++i) {
            // Print SSID and RSSI for each network found    
           // char msg[] =  "                                      ";
            char msg[200]={0};
            // reset msg buffer and packet
            for (int j=0;j<200;j++) {
                msg[j] = 0;
                pkt[j] = 0;
                }
            char ssid[200] ;
            WiFi.SSID(i).toCharArray(ssid,200);
            sprintf_P(msg, "%2d:%s",i+1,ssid);
            Serial.print("About to send msg:"); Serial.println(msg);
            Serial.print("SSID Dump:");

            for (int j=0;j<200;j++) {
                char c = ssid[j];
                Serial.print(c,HEX);Serial.print(" ");
                if(c==0) break;
            }
            Serial.print("\n");
            delay(1000);
            int plen = EA_msg_pkt_build(pkt, msg);
            int code = EA_test_packet(pkt);
            if (code < 0){
                Serial.println("SSID message packet defective...");
            }
            EA_write_pkt_serial(pkt,plen); // send!
            Serial.print("         Sent msg:"); Serial.println(msg);

            // reset msg buffer and packet
            for (int j=0;j<200;j++) {
                msg[j] = 0;
                pkt[j] = 0;
                }

            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            //Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            Serial.println("");
            delay(800);
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(3000);
}
