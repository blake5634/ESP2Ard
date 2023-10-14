#include "ESP2Ard.h"


/*    Unit tests of common API elements
 *   (must build and run with Arduino IDE for either target)
 *
 *   We will test the following functions:
 *
// Build a packet from a text payload (eg. quoted string constant)
int EA_msg_pkt_build(EA_msg_byte* pkt, char* message){

// build a packet given a binary byte payload
int EA_pkt_build(EA_msg_byte* pkt, int payload_len, EA_msg_byte* payload){


void EA_dump_packet_bytes(EA_msg_byte* pkt){
    // print it out raw for user


int EA_test_packet(EA_msg_byte* pkt){

*
*/

#define FAIL -1
#define PASS  1

void ppassfail(int result){
    if (result == PASS) Serial.println("       >>>    Test PASSES");
    else                Serial.println("       <<<    Test FAILURE");
    }

void setup(void){
    Serial.begin(9600);

    Serial.println("              Unit tests for EA2Ard package");
    Serial.println("");

    Serial.println("     1) Testing EA_msg_pkt_build(EA_msg_byte* pkt, char* message)");
    Serial.println("");

    EA_msg_byte pkt[500]={0};
//     char msg[] = "test message [$%^&*] 123456789";
    char msg[] = "tmsg001";
    // let's get checksum of message (the payload)

    unsigned char msg_cksum = 0;
    for (int i=0;i<ESP2Ard_max_payload_size;i++){
        msg_cksum += msg[i];
        }
    int char_count = 7;  // not incl trailing '\0'
    int cc2 = strlen(msg);
    if (char_count != cc2) {
        Serial.println("packet char counts don't match!!");
    }
    EA_msg_pkt_build(pkt,msg);
    Serial.println("        0123456789012345678901234567890123456789");
    Serial.print("message:");
    Serial.println(msg);
//     Serial.print("    pkt:");
//     Serial.println(((char*) pkt)+3);
    Serial.print("charcnt: ");
    Serial.println(char_count);
    Serial.print ("pkt Nchar byte (hex): ");
    Serial.println(pkt[2], HEX);
    int result = FAIL;
    if (pkt[0] == 0xFF &&
        pkt[1] == 0x00 &&
        pkt[2] == cc2  &&
        pkt[3] == 't'  &&
        pkt[4] == 'm' // &&
//         pkt[2+cc2] == msg_cksum
    )  result = PASS;
    ppassfail(result);

/*
    Serial.println("");
    Serial.println("     2) Testing int EA_pkt_build(EA_msg_byte* pkt, int payload_len, EA_msg_byte* payload)");
    Serial.println("");

    Serial.println("");
    Serial.println("     3) Testing void EA_dump_packet_bytes(EA_msg_byte* pkt)");
    Serial.println("");

    Serial.println("");
    Serial.println("     4) Testing EA_test_packet(EA_msg_byte* pkt)");
    Serial.println("");

    Serial.println("");
    Serial.println("              Unit tests for EA2Ard package");
    Serial.println("");
  */
}

void loop(void) {
    while(1){
        delay(1000);
    }
}
