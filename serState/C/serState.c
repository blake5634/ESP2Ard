/*
       State machine for receiving serial packets (ESP2Ard)
*/

// #define  ArduinoAIDE    // for uploading to Arduino
#define  GCC            // run locally with   >gcc -o ss serState.c



//  need ths for ESP2Ard.h
#define EA_timeout_ERROR  -10

int state_machine(char , char* );

#ifdef GCC
  // just for stubs
  #define LED_BUILTIN  0
  #define OUTPUT 0
  #define HIGH   1
  #define LOW    0
  #define HEX    1

  #include <stdarg.h>
  #include <unistd.h>
  void delay(int n);
  void SerialDOTbegin(int baudrate);
  void SerialDOTprint(char* txt);
  void SerialDOTprintln(char* txt);
  void pinMode(int a1, int a2);
  void digitalWrite(int a1, int a2);
  int EA_read(char* cdata_ptr); // return 1=valid data byte, 0 = no databyte

#endif

//
//   Code for both
//

#include <stdio.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef ArduinoAIDE
  Serial.begin(9600);
#endif
#ifdef GCC
  SerialDOTbegin(9600);
#endif
  delay(2000);
#ifdef ArduinoAIDE
  Serial.println("       Starting  Serial StateMachine Test  \n\n");
#endif
#ifdef GCC
  SerialDOTprintln("       Starting  Serial StateMachine Test  \n\n");
#endif
}

int rcvpkt(char* buf, int timeout){
  int tout = timeout;
  int ptr = 0;
  int smreturn = 0;
  char c;
  char *dataptr = &c;
  while(1){
    while(EA_read(dataptr)==0) {
      tout--; delay(1);} // wait for data
    smreturn = state_machine(c, buf );
    if (smreturn > 0) { // we got a full packet
        return smreturn;
        }
//    SerialDOTprint(" in state: ");SerialDOTprint(smreturn);SerialDOTprint("  char2: ");SerialDOTprintln(c,HEX);
    delay(1);
    tout--;
    if (tout < 0) return EA_timeout_ERROR;
  }
}

int state_machine(char c, char* buf){
  static int rcv_state = -1;
  static int ptr = 0;
  static int byte_cnt = 0;  // packet byte count from msg
  static int bc = 0; // actual byte count
  switch (rcv_state){
    case -1:
      if(c==(char)0xFF) { rcv_state = -2;
        buf[ptr++] = c;
      }
      else {
        rcv_state = -1; // stay
      }
      return rcv_state;

    case -2:
      if(c==0) { rcv_state = -3;
        buf[ptr++] = c;
      }
      else {
        ptr = 0;
        rcv_state = -1; // not true pkt start
      }
      return rcv_state;

    case -3:
      byte_cnt = (int) c;
      buf[ptr++] = c;
      bc = 0;
      rcv_state = -4;
      return rcv_state;

    case -4:
      buf[ptr++] = c;
      bc++;
      // byte_count, computed by sender, does not include FF, 00, cksum and EOP
      if (bc >= byte_cnt+2){ // account for cksum byte and  packet-close byte
        return bc+2; // total packet len: (bc + {FF, 00})
      }
      else {
        rcv_state = -4;
      }
      return rcv_state;
  }
  return rcv_state;
}

// test mock
int EA_read(char* cdata_ptr){ // return 1=valid data byte, 0 = no databyte
  static int dropinterval=4; // every 4 - no byte avail
  static int dropcnt = 0;
  static int i=0;
  // tst1 has junk before and after the pckt
  char tst1[] = {9,9,9,0xFF, 0, 3, 1, 2, 3, 6, 0x0A, 0x0D, 0x0D};
  int  len1 = 13;
  // tst2 does not have junk before and after the pckt
  char tst2[] = {0xFF, 0, 3, 1, 2, 3, 6, 0x0A};
  int  len2 = 8;

  int len = 0;
  char* tbytes = tst1;
  // select a test
  tbytes = tst2;
  len = len2;
  // simulate no byte available some times
  if (dropcnt++ % dropinterval != 0){
    // print the bytes as they are "read"
    if (i<=len)  {

#ifdef ArduinoAIDE
      Serial.print(" (simread)>");Serial.println(&tbytes[i]);
#endif
#ifdef GCC
      SerialDOTprint(" (simread)>");SerialDOTprintln(&tbytes[i]);
#endif

      *cdata_ptr = tbytes[i++];
      return 1; }
    else {

#ifdef ArduinoAIDE
      Serial.println("ERROR: test bytes overrun");
#endif
#ifdef GCC
      SerialDOTprintln("ERROR: test bytes overrun");
#endif
      while(1) delay(100);
      return -500;}
    }
  else {  // simulate no bytes available


#ifdef ArduinoAIDE
    Serial.println("sim. drop");
#endif
#ifdef GCC
    SerialDOTprintln("sim. drop");
#endif

    cdata_ptr = NULL;
    return 0;
  }
}

// the loop function runs over and over again forever
void loop() {
  int retval = 0;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  // Test rcvpkt
  static char buffer[200]={0};
  retval = rcvpkt(buffer, 50);
  if (retval>0){
#ifdef ArduinoAIDE
    Serial.println("Complete packet Returned (subject to EA_test_pkt() verification");
#endif
#ifdef GCC
    SerialDOTprintln("Complete packet Returned (subject to EA_test_pkt() verification");
#endif
    while(1) delay(500);
    }
  if (retval == EA_timeout_ERROR) {
#ifdef ArduinoAIDE
    Serial.println("ERROR: packet read timeout");
#endif
#ifdef GCC
    SerialDOTprintln("ERROR: packet read timeout");
#endif
    while(1) delay(500);}
}


//  Arduino stubs for gcc

#ifdef GCC
int main(void){
  loop();
}

#include <unistd.h>
void delay(int n){
  usleep(n*1000);
}

void SerialDOTbegin(int baudrate){
}
void SerialDOTprint(char* txt){
  printf("%s",txt);
}

void SerialDOTprintln(char* txt) {
  printf("%s\n", txt);
}

void pinMode(int a1, int a2){
}

void digitalWrite(int a1, int a2){
}
#endif
