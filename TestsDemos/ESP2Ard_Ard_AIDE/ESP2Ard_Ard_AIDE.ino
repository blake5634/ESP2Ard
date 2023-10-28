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

  Reads a serial data input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/DigitalReadSerial
*/

#include "ESP2Ard.h"

// the setup routine runs once when you press reset:
#define ARD_PIN_RX 2  // redefine for our app.
#define ARD_PIN_TX 3
// SoftwareSerial swSerial(PIN_RX,PIN_TX) ;  // serial coms on pins 2,3

EA_msg_byte packet[ESP32Ard_max_packet_size];  // coms packet from ESP32 via SoftwareSerial

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
  while((nrcvd = EA_get_packet_serial(packet, ESP32Ard_timeout_delay_ms) ) == ESP32Ard_timeout_error)
   {
    EA_log(" packet timeout ... will try again in 1500 msec");
    EA_delay_ms(1500);
    }
  if (nrcvd > 0){
    char msg[100] = {0};
    sprintf(msg, "got packet: nrcvd: %d\n",nrcvd);
    EA_log(msg);
    ercd = EA_test_packet(packet);
    if (ercd < 0){
        Serial.print(" bad packet received: code, n:");
        Serial.print(ercd);
        Serial.print(" ");
        Serial.println(nrcvd);
    }
    else {Serial.print("No packet errors!! ");
      Serial.print(nrcvd);
      Serial.println(" bytes"); }

  #define ESP2Ard_DEBUG
  #if defined(ESP2Ard_DEBUG) && defined(ARDUINO_PLATFORM)
  //
  // some debug options for viewing received packets:
  //
  #define OPTION1  1
  // #define OPTION2  2

  // 1 print it out raw byte by raw byte
  #ifdef OPTION1
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
  #ifdef OPTION2
  char* message = packet+3; // start of data
  Serial.println(message);
  #endif
  #endif //defined(ESP2Ard_DEBUG) && defined(ARDUINO_PLATFORM)
    //
    // unpack the data packet
    //
    int charcnt = packet[2];
    int hour = packet[3];
    int minute = packet[4];
    int cksum = packet[nrcvd-2];
    int eof = packet[nrcvd-1];
    Serial.println(sprintf("cnt: %d chsum: %d", charcnt, cksum));
    Serial.println(packet[3]);
    // Serial.println(sprintf("cnt: %d hour: %d min: %d  chsum: %d", charcnt, hour, minute, cksum));
    //#define TimeSetApp
  #ifdef TimeSetApp
    Serial.print("The Time Set app itself: \n cnt:");
    Serial.print(charcnt);
    Serial.print("  hour:");
    Serial.print(hour);
    Serial.print("  min:");
    Serial.print(minute);
    Serial.print("  cksum(HEX):");
    Serial.print(cksum,HEX);
    Serial.println("");
  #endif
    for (int i=0;i<ESP32Ard_max_packet_size;i++){  // clear the pkt buffer to prevent logic breaks.
      packet[i] = 0;
     }
  } // end of valid packet processing

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);        // delay xx ms in between reads for stability
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(200);        // delay xx ms in between reads for stability
}
