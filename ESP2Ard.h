/*
 *    ESP2Ard.h :
 *
 *          ESP32 to Arduino serial port (UART) communication
 *
 *  A basic library for sending simple packets back and forth
 *
 * Packet format: (8-bit bytes)
 * [  0]   0xff      Start Flag 1
 * [  1]   0x00      Start Flag 2
 * [  2]   (byte)L   number of payload bytes
 * [  3]   ...       start of payload
 * [3+L]  cksum      Checksum of payload (%256)
 * [4+L]  '\n'       End of packet flag
 *
 * L = # of payload bytes
 *   (1 <= L <= 255)
 *
 * Packet total length:   ESP2Ard_OVERHEAD_BYTES + L bytes
 */
 
/////////////////////////  Configuration Zone ///////////////////////////////////////////
//  ESP2Ard_DEBUG works only on Arduino for now
#define ESP2Ard_DEBUG  // more verbose debugging output

////////////////////////////////////////////
//
//  What platform and IDE are we running on?
//    uncomment just one of these:

// #define ARDUINO_PLATFORM          // arduino HW on Arduino IDE
// #define ESP32_IDF_PLATFORM        // ESP32 HW on IDF IDE
#define ESP32_Arduino_PLATFORM     // ESP32 HW on Arduino IDE


//
//  All platforms:
//
//   Configure comm parameters which should be same on both ends
//
#define ESP32Ard_max_packet_size        500
#define ESP32Ard_timeout_delay_ms        50
#define ESP2Ard_BaudRate               9600
#define ESP2Ard_OVERHEAD_BYTES            5  //len(packet) - len(payload)
#define ESP2Ard_max_payload_size  ESP32Ard_max_packet_size -  ESP2Ard_OVERHEAD_BYTES

//
//  >>> ESP32, configure serial params here
//
#define ESP_PIN_RX              16  // Receive data pin on ESP32
#define ESP_PIN_TX              17
#define ESP_UART_NUM    UART_NUM_2  //UART_NUM_2 from driver/uart.h


//
//  >>> Arduino on Arduino IDE: config serial params
//
#ifdef ARDUINO_PLATFORM
//
//   Arduino pin setup
#define ARD_PIN_RX   2
#define ARD_PIN_TX   3

//  Arduino Serial port type
#define ARDUINO_SW_SERIAL
//#define ARDUINO_HW_SERIAL  // no code written for this yet!

#endif // ARDUINO_PLATFORM

//
//  >>>  ESP32 on Arduino IDE: config serial params

#if defined(ESP2Ard_ESP32_AIDE)

#define ARD_PIN_RX 16  //  (these usually same as ESP_PIN_RX/TX)  
#define ARD_PIN_TX 17

#endif
 
/////////////////////////////////  End of Configuration Zone ////////////////////////////

////////////////////////////////////////////
//  types and function protos

#if defined(ARDUINO_PLATFORM)

void msg_2part(char* msg, int i);
typedef   byte  EA_msg_byte;

#endif

#if defined(ESP32_IDF_PLATFORM)

#include "esp_log.h"
#include "driver/uart.h"
#define ESP32_HW_SERIAL
#define TAG "ESP2Ard message:"  // used for logging
typedef   unsigned char  byte;  // used w/ arduino just in case
typedef   unsigned char  EA_msg_byte;

#endif

#if defined(ESP32_Arduino_PLATFORM)

#include <Arduino.h>
typedef   unsigned char  EA_msg_byte;

#endif
//
// defines for all platforms and ports:
//

// Error codes
#define ESP32Ard_timeout_error           -1
#define ESP32Ard_bad_pkt_header          -2
#define ESP32Ard_packet_length_overrun   -3
#define ESP32Ard_packet_length_incorrect -4
#define ESP32Ard_payload_size_zero       -5
#define ESP32Ard_packet_cksum_error      -6

#define ESP32Ard_packet_check_OK          1

//
//   Our basic coms functions (all platforms)
//
void EA_log(const char* msg);
EA_msg_byte* EA_msg_make(const char* str);
int  EA_msg_pkt_build(EA_msg_byte* pkt, char* message);
int  EA_available();
void EA_delay_ms(int dms);
char EA_read();
char EA_write_buffer(EA_msg_byte* buf, int len);
int  EA_get_packet_serial(EA_msg_byte* buf);
int  EA_test_packet(EA_msg_byte* pkt);
int  EA_pkt_build(EA_msg_byte* pkt,int pll, EA_msg_byte* payload);
int  EA_write_pkt_serial(EA_msg_byte* pkt, int len);
void EA_init_serial(int rcv, int tx);
void EA_dump_packet_bytes(EA_msg_byte *pkt);
