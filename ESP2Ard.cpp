/*
 *  ESP32 to Arduino Communication Package
 *      Blake Hannaford   10/23
 * Note:
 *    for Arduino file must be named ESP2Ard.cpp
 *    for ESP IDF environment file must be named ESP2Ard.c
 *    ... BUT the file can be identical -- this file
 */

#include "ESP2Ard.h"

//
// Set up for platform and serial environment
#if defined(ARDUINO_PLATFORM)  // arduino on arduino IDE
#include <Arduino.h>
#endif 

#ifdef ESP32_Arduino_PLATFORM   // ESP32 on arduino IDE
#include <Arduino.h>
#endif

//
//   Serial functions for ARDUINO_SW_SERIAL
//
#ifdef ARDUINO_SW_SERIAL

#include <SoftwareSerial.h>
SoftwareSerial* gSwSerialptr;  // place to setup SwSerial object

//ARDUINO_SW_SERIAL
void EA_init_serial(int rcv, int tx){
    static SoftwareSerial sws(rcv,tx);
    sws.begin(ESP2Ard_BaudRate);
    EA_log("... sw serial initialized ");
    gSwSerialptr = &sws;
    return NULL ;
  }

//ARDUINO_SW_SERIAL
int EA_available(){
    int x = gSwSerialptr->available();
    return x;
  }

//ARDUINO_SW_SERIAL
char EA_read(){
    return gSwSerialptr->read();
    }

int EA_write(){
  return 0;   //TODO: implement arduino send
}
#endif   // ARDUINO_SW_SERIAL (line 21)


//
//    Serial functions for ESP32 on the Arduino IDE
//
#if defined(ESP32_Arduino_PLATFORM)

// ESP32_Arduino_PLATFORM
void EA_init_serial(int rcv, int tx){
  Serial2.begin(ESP2Ard_BaudRate, SERIAL_8N1, tx, rcv);
}

// ESP32_Arduino_PLATFORM
int EA_available(){
  int nbytes = Serial2.available();
  return nbytes;
}

// ESP32_Arduino_PLATFORM
char EA_read(){
}

// ESP32_Arduino_PLATFORM
int EA_write_pkt_serial(EA_msg_byte* buf, int len){
  int code = EA_test_packet(buf);
  if (code == ESP32Ard_packet_check_OK){
    EA_log("packet checked out.. sending");
    Serial2.write(buf, len);
    return len;
  }
  else {
    char msg[100];
    EA_log("somethings wrong with uart_write_bytes");
    sprintf(msg, "EA_write_pkt_serial(): trying to send a bad backet: error: %d",code);
    EA_log(msg);
    return 0;
  }
}

#endif


//
//     Serial functions for ESP32 using the IDF IDE
//
#if defined(ESP32_HW_SERIAL)

#include "driver/uart.h"
#include "soc/uart_struct.h"

#define PIN_RX 16
#define PIN_TX 17
#define BUF_SIZE (int) (ESP32Ard_max_packet_size * 2)

QueueHandle_t uart_queue; // RTOS queue for UART

uart_config_t* uartConfig;   //  keep config around
int pin_rcv = ESP_PIN_RX;
int pin_tx  = ESP_PIN_TX;


// ESP32_HW_SERIAL
void EA_init_serial(int rcv, int tx){
    uart_config_t uart_config = {
        .baud_rate = ESP2Ard_BaudRate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,    //UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    uartConfig = &uart_config;
    pin_rcv = rcv;
    pin_tx  = tx;

    //  create RTOS queue for serial data
    xQueueCreate( ESP32Ard_max_packet_size*2,   1 );

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, PIN_TX, PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_LOGI(TAG,"ESP serial port 2 initialized");
    }


// ESP32_HW_SERIAL
int EA_available(){
  ESP_LOGI(TAG, "ESP2Ard error:  EA_available() not allowed on ESP_HW_SERIAL platform. use EA_read()");
  // die here...
  while(1)  EA_delay_ms(2000);
  }

// ESP32_HW_SERIAL
//  read one byte if available.
//  read one byte if available.
char EA_read(){
    EA_msg_byte buf[3]={'\0'};
#define NUM_BYTES 1 // for compatibility w/ Arduino code
    int n =  uart_read_bytes(ESP_UART_NUM, buf, NUM_BYTES, pdMS_TO_TICKS(ESP32Ard_timeout_delay_ms));
    if (n< 0) n=0;
    return n;
    }

// ESP32_HW_SERIAL
int EA_write_pkt_serial(EA_msg_byte* buf, int len){
  int code = EA_test_packet(buf);
  if (code == ESP32Ard_packet_check_OK){
    EA_log("packet checked out.. sending");
    uart_write_bytes(ESP_UART_NUM, buf, len);
    return len;   //TODO: implement arduino send
  }
  else {
    char msg[100];
    EA_log("somethings wrong with uart_write_bytes");
    sprintf(msg, "EA_write_pkt_serial(): trying to send a bad backet: error: %d",code);
    EA_log(msg);
    return 0;
  }
}

#endif  // ESP_HW_SERIAL (line 54)






//  Platform agnostic functions

void EA_delay_ms(int dms){
#if defined(ARDUINO_PLATFORM) || defined(ESP32_Arduino_PLATFORM)
    delay(dms); // ms
#endif
#ifdef ESP32_IDF_PLATFORM
    vTaskDelay(pdMS_TO_TICKS(dms));
    // 1ms Free/RTOS delay
#endif
}

int EA_get_packet_serial(EA_msg_byte* buf){
  int pidx = 0;
  char c = ' ';
  pidx = 0;
  int timeoutms = ESP32Ard_timeout_delay_ms;
  while(1){
    timeoutms--;
    if (EA_available()  == 0 )  return 0;
   // Serial.print("\n");
    while (EA_available() > 0) {
        c = EA_read();
        buf[pidx] = c;
#if   defined(ARDUINO_PLATFORM) && defined(ESP2Ard_DEBUG)
   //   Serial.println( sprintf(">> %d, %c", pidx, packet[pidx]) );
        Serial.print(">> ");
        Serial.print(pidx);
        Serial.print("  ");
        Serial.print((EA_msg_byte)  c , HEX);
        char rep = ' ';// print space if non-printable char
        if (c >= 32 && c <= 126) rep = c;
        Serial.print(",  ");
        Serial.print((char)rep);  // print it if printable
        Serial.println("");
#endif
        if (c == '\n')  break; // == 0xA
        pidx++;
      if (c == '\n') break; // 0xA == end of message
      EA_delay_ms(1);
    } // end of message
    if (timeoutms <= 0) { pidx = 0; break;}
    else {
      timeoutms = ESP32Ard_timeout_delay_ms; // reset timout timer
      return pidx;  // num of bytes read
      }
  }
  return ESP32Ard_timeout_error;  // approx timeoutms ms.
}

EA_msg_byte* EA_msg_make(const char* str){
  return (EA_msg_byte*) str;
}

// Build a packet from a text payload (eg. quoted string constant)
int EA_msg_pkt_build(EA_msg_byte* pkt, char* message){
  //int message_len = strlen(message) + 1 ; // we are going to include the trailing zero
  int msg_len = 0;
 // Serial.print(" -- I'm here -- ");Serial.println(message);
  for (int i=0;i<ESP2Ard_max_payload_size;i++){
    if (message[i] == 0)
        { msg_len = i ;
          break; }
   // Serial.print("msg byte: ");Serial.println(message[i],HEX);
    }
  //msg_len += 1; // include the trailing zero in message payload

  EA_msg_byte* payload = EA_msg_make(message); // proper type conversion
  int pktlen = EA_pkt_build(pkt, msg_len, payload);
  return pktlen;
}

// build a packet given a binary byte payload
int EA_pkt_build(EA_msg_byte* pkt, int payload_len, EA_msg_byte* payload){
  pkt[0] = 0xFF;  // packet header0
  pkt[1] = 0x00;  // packet header1
  if (payload_len > 255) {
    EA_log("payload must be 255 bytes or less");
    while(1) EA_delay_ms(2000);  //freeze here.
  }
  pkt[2] = (char) payload_len; // payload byte count
  byte cksum = 0;
  int end_of_payload = 3;
  for (int i=0;i<payload_len;i++){ // copy payload into packet
    pkt[3+i] = payload[i];
    cksum += payload[i];
    end_of_payload++;
  }
  if (end_of_payload > ESP32Ard_max_packet_size){
    EA_log("packet is too big for local buffers: increase ESP32Ard_max_packet_size");
    while(1)  EA_delay_ms(2000);
    }
  pkt[end_of_payload] = cksum;
  pkt[end_of_payload+1] = '\n';  // end of packet flag
  int pktLen = payload_len + ESP2Ard_OVERHEAD_BYTES;
  return pktLen; // total pkt length
  }

void EA_dump_packet_bytes(EA_msg_byte* pkt){
    // print it out raw for user
#if defined(ARDUINO_PLATFORM) || defined(ESP32_Arduino_PLATFORM)

    Serial.println(">> packet bytes (hex): ");
    Serial.println("-------");
    for (int i=0;;i++){
      Serial.print(" [");
      Serial.print( (EA_msg_byte) pkt[i], HEX);
      Serial.print("] ");
      if(pkt[i] ==  '\n') break;
      if(i>ESP32Ard_max_packet_size) break;
      }
    Serial.println("");
    Serial.println("-------");
#endif

#if defined(ESP32_IDF_PLATFORM)

    ESP_LOGI(TAG,">> packet bytes (hex): ");
    ESP_LOGI(TAG,"--------");
    for (int i=0;;i++){
      ESP_LOGI(TAG,"%2d [0x%02x]",i,pkt[i]);
      if(pkt[i] ==  '\n') break;
      if(i>ESP32Ard_max_packet_size) break;
      }
    ESP_LOGI(TAG,"-------");

#endif

}
//
//  log method for functions that work on all platforms
//
void EA_log(const char* msg){

  #if defined(ESP32_HW_SERIAL)
    printf(msg);
  #endif

  #if defined(ARDUINO_PLATFORM) || defined(ESP32_Arduino_PLATFORM)
    Serial.println(msg);
  #endif

}

/*
 *  These are the error code returns
 *
    #define ESP32Ard_bad_pkt_header          -2
    #define ESP32Ard_packet_length_overrun   -3
    #define ESP32Ard_packet_length_incorrect -4
    #define ESP32Ard_payload_size_zero       -5
    #define ESP32Ard_packet_cksum_error      -6

    #define ESP32Ard_packet_check_OK          1
*/

void msg2part(char* msg, int i){

#if defined(ARDUINO_PLATFORM) || defined(ESP32_Arduino_PLATFORM)
  Serial.print(msg); Serial.println(i);
#endif

#if defined(ESP32_HW_SERIAL)
  printf(msg); printf(" %d",i);
#endif

}

//#define VERBOSE_EA_test_packet

int EA_test_packet(EA_msg_byte* pkt){

#if defined(VERBOSE_EA_test_packet)

  Serial.println("\n .. test a packet ...");

#endif

if (pkt[0] != 0xFF  ){
#ifdef VERBOSE_EA_test_packet

    Serial.println("--------------- I caught a bad header[0]");
    msg2part("       pkt[0]: ", pkt[0]);
    msg2part("       pkt[1]: ", pkt[1]);
    msg2part("       pkt[2]: ", pkt[2]);

#endif
    return ESP32Ard_bad_pkt_header; }
  if (pkt[1] != 0x00  ){
#ifdef VERBOSE_EA_test_packet

//     Serial.println("--------------- I caught a bad header[1]");
//     msg2part("       pkt[0]: ", pkt[0]);
//     msg2part("       pkt[1]: ", pkt[1]);
//     msg2part("       pkt[2]: ", pkt[2]);

#endif
     return ESP32Ard_bad_pkt_header; }

  int pkt_payload_size = (int)pkt[2];
#ifdef VERBOSE_EA_test_packet
  Serial.print("              pkt payld size: ");Serial.println(pkt_payload_size);
#endif
  if (pkt_payload_size == 0) return ESP32Ard_payload_size_zero;
  //
  //  compute length and payload checksum
  //
  int len_payload=0;
  int len_packet=0;
  byte rcksum = 0;
  //
  //  compute length and payload checksum
  //
  for (int i=0; i<ESP32Ard_max_packet_size ; i++){
    len_packet++;
    if (pkt[i] == 0xA) break;
    if (i>= ESP32Ard_max_packet_size)  return ESP32Ard_packet_length_overrun;
    }
    len_payload = len_packet-5;
    // get payload checksum
    for (int i=0; i<len_payload; i++){ // actually add up the payload
      rcksum += pkt[i+3];
    }
  int pkt_cksum_idx = len_payload+3;
#if defined(VERBOSE_EA_test_packet)
  Serial.print("  PLL>");
  Serial.print(len_payload);
  Serial.print("  PkL>");
  Serial.print(len_packet);
  Serial.print("  rcksum>");
  Serial.print(rcksum);
  //Serial.print("  >");

  Serial.println("");
  Serial.print("cksum test: ");
  Serial.print((int)rcksum);
  Serial.print("  <");
  Serial.print((int) pkt[pkt_cksum_idx]);
  Serial.println(">");
#endif

  if (rcksum != (byte) pkt[pkt_cksum_idx])  return ESP32Ard_packet_cksum_error;
  if (len_payload != pkt_payload_size)      return ESP32Ard_packet_length_incorrect;
  return ESP32Ard_packet_check_OK;
} // end of EA_test_packet()


