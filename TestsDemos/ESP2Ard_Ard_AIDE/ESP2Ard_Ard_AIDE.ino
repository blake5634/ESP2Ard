/*
 * This is a test/demo app to run in Arduino (w/ Arduino IDE).
 *    It receives packets from another device running ESP2Ard
 *    and checks them and dumps them to the USB/Serial link back
 *    to laptop (Arduino Serial Monitor).
 *
 *    B. Hannaford  Oct 2023
 *
 *
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/DigitalReadSerial
*/

#include <SoftwareSerial.h>
#include "ESP2Ard.h"

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 2;

// the setup routine runs once when you press reset:
#define ARD_PIN_RX 2  // redefine for our app.
#define ARD_PIN_TX 3
// SoftwareSerial swSerial(PIN_RX,PIN_TX) ;  // serial coms on pins 2,3

char packet[ESP32Ard_max_packet_size];  // coms packet from ESP32 via SoftwareSerial

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
//  swSerial.begin(9600); 
  EA_init_serial(ARD_PIN_RX, ARD_PIN_TX);
  pinMode(LED_BUILTIN, OUTPUT);
}

 
// the loop routine runs over and over again forever:
void loop() {
  int ercd = 0;
  // read the input pin:
  int nrcvd = 0;
  //
  //   Check the serial port for data from ESP32
  //
  nrcvd = EA_get_packet_serial(packet);
  if (nrcvd == ESP32Ard_timeout_error) {
    Serial.println(" serial port timed out");
    }
  else if (nrcvd > 0) {
   
    ercd = EA_test_packet(packet);
    if (ercd < 0){
       Serial.print(" bad packet received: code, n:");
       Serial.print(ercd);
       Serial.print(" ");
       Serial.println(nrcvd);
    }
    else Serial.println("No packet errors! ");
#ifdef ESP2Ard_DEBUG
#ifdef ARDUINO_PLATFORM
    // print it out raw for user
    Serial.println(">> received bytes: ");
    for (int i=0;i<nrcvd;i++){
      char ch = packet[i];
      Serial.print(" [");
      Serial.print( (unsigned char) ch, HEX);
      Serial.print(", ");
      char rep = ' ';
      if (ch >= 32 && ch <= 126) rep = ch;
      Serial.print((char)ch);
      Serial.print("] ");
      }
    Serial.println("");
    Serial.println("-------"); 
#endif
#endif
    //
    // unpack the data packet
    //
    int charcnt = packet[2];
    int hour = packet[3];
    int minute = packet[4];
    int cksum = packet[nrcvd-2];
    int eof = packet[nrcvd-1];
   // Serial.println(sprintf("cnt: %d hour: %d min: %d  chsum: %d", charcnt, hour, minute, cksum));
    Serial.print("The app itself: \n cnt:");
    Serial.print(charcnt);
    Serial.print("  hour:");
    Serial.print(hour);
    Serial.print("  min:");
    Serial.print(minute);
    Serial.print("  cksum(HEX):");
    Serial.print(cksum,HEX);
    Serial.println(""); 
    for (int i=0;i<ESP32Ard_max_packet_size;i++){  // clear the pkt buffer to prevent logic breaks.
      packet[i] = 0;
    }
  }

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(225);        // delay xx ms in between reads for stability
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(225);        // delay xx ms in between reads for stability
}
