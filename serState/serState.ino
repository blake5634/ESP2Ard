/*
       State machine for receiving serial packets (ESP2Ard)
*/

#define EA_timeout_ERROR  -10

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  delay(2000);
  Serial.println("       Starting  Serial StateMachine Test  \n\n");
}

int rcvpkt(char buf[], int timeout){
  int tout = timeout;
  int ptr = 0;
  int smreturn = 0;
  char c;
  while(1){
    c = EA_read();
    smreturn = state_machine(c, buf );
    if (smreturn > 0) { // we got a full packet
        return smreturn;
        }
    Serial.print(" in state: ");Serial.print(smreturn);Serial.print("  char: ");Serial.println(c);
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
      if (bc >= byte_cnt+1){ // account for packet-close byte
        return bc+3; // total packet len
      }
      else {
        rcv_state = -4;
      }
      return rcv_state;
  }
  return rcv_state;
}

// test mock
char EA_read(){
  static int i=0;
  // tst1 has junk before and after the pckt
  char tst1[] = {9,9,9,0xFF, 0, 3, 1, 2, 3, 0x0A, 0x0D, 0x0D};
  int  len1 = 12;
  // tst2 does not have junk before and after the pckt
  char tst2[] = {0xFF, 0, 3, 1, 2, 3, 4, 0x0A};
  int  len2 = 8;
  int len = 0;
  char* tbytes = tst1;

  tbytes = tst2;
  len = len2;
  if (i<=len){ Serial.print(" >");Serial.println(tbytes[i],HEX); return tbytes[i++]; }
  else {
    Serial.println("ERROR: test bytes overrun");
    while(1) delay(100);
    return -500;}
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
  Serial.print("  ... test");
  retval = rcvpkt(buffer, 50);
  if (retval>0){  Serial.println("Complete packet Returned (subject to EA_test_pkt() verification"); while(1) delay(500);}
  if (retval == EA_timeout_ERROR) { Serial.println("ERROR: packet read timeout"); while(1) delay(500);}
}
