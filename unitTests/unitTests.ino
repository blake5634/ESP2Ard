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

void ppassfail(int test_no, int result){
    if (result == PASS)
    {
        Serial.print("       >>>    Test ");
        Serial.print(test_no);
        Serial.println(" PASSES"); }
    else
    {   Serial.print("       <<<    Test ");
        Serial.print(test_no);
        Serial.print(" FAILS");
    }
}

#define HUGE_PKT_SIZE  500
void clear_pkt(EA_msg_byte* pkt){
    for (int i=0;i<HUGE_PKT_SIZE;i++)  pkt[i] = 0;
    }

void setup(void){
    Serial.begin(9600);

    Serial.println("                 Unit tests for EA2Ard package");
    Serial.println("");
    Serial.println("");

    /////////////////////////////////////////////////////////////////////////////////////////////
    Serial.println("");
    Serial.println("     1) Testing int EA_pkt_build(EA_msg_byte* pkt, int payload_len, EA_msg_byte* payload)");
    Serial.println("");
    EA_msg_byte pkt[HUGE_PKT_SIZE]={0};
    EA_msg_byte pyld[3] = {1,2,3};

    EA_pkt_build(pkt, 3, pyld);

    int result = FAIL;
    if (pkt[0] == 0xFF &&
        pkt[1] == 0x00 &&
        pkt[2] ==   3  &&
        pkt[3] ==   1  &&
        pkt[4] ==   2  &&
        pkt[5] ==   3  &&
        pkt[6] ==   6  &&
        pkt[7] == '\n') result = PASS;

    ppassfail(1,result);

    /////////////////////////////////////////////////////////////////////////////////////////////
    Serial.println("");
    Serial.println("     2) Testing EA_msg_pkt_build(EA_msg_byte* pkt, char* message)");
    Serial.println("");
    clear_pkt(pkt);

//     char msg[] = "test message [$%^&*] 123456789";
    char msg[] = "tmsg001";
    // let's get checksum of message (the payload)

    int char_count = 7;  // not incl trailing '\0'
    int cc2 = strlen(msg);
    if (char_count != cc2) {
        Serial.println("packet char counts don't match!!");
    }
    unsigned char msg_cksum = 0;
    for (int i=0;i<cc2;i++){
        msg_cksum += msg[i];
        }
    EA_msg_pkt_build(pkt,msg);
// #define VERBOSE2
#ifdef VERBOSE2
    Serial.println("        0123456789012345678901234567890123456789");
    Serial.print("message:");
    Serial.println(msg);
//     Serial.print("    pkt:");
//     Serial.println(((char*) pkt)+3);
    Serial.print("charcnt: ");
    Serial.println(char_count);
    Serial.print ("pkt Nchar byte (hex): ");
    Serial.println(pkt[2], HEX);
    Serial.print ("payload computed chksum (hex): ");
    Serial.println(msg_cksum, HEX);
    Serial.print ("pkt chksum (hex): ");
    Serial.println(pkt[cc2+3], HEX);
#endif
    result = FAIL;
    if (pkt[0] == 0xFF &&
        pkt[1] == 0x00 &&
        pkt[2] == cc2  &&
        pkt[3] == 't'  &&
        pkt[4] == 'm'  &&
        pkt[3+cc2] == msg_cksum
    )  result = PASS;
    ppassfail(2, result);


    /////////////////////////////////////////////////////////////////////////////////////////////
    Serial.println("");
    Serial.println("     3) Testing void EA_dump_packet_bytes(EA_msg_byte* pkt)");
    Serial.println("");
    clear_pkt(pkt);

    Serial.println("Test3: Expected Correct Output: ");
    Serial.println(" [FF]  [0]  [3]  [1]  [2]  [3]  [6]  [A] ");
    Serial.println("Test3: Actual Output: ");

    EA_pkt_build(pkt, 3, pyld);  // same as test 1 payld
    EA_dump_packet_bytes(pkt);
    ppassfail(3, PASS);

    Serial.println("");
    Serial.println("     4) Testing EA_test_packet(EA_msg_byte* pkt)");
    Serial.println("");

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

    result = PASS; // unless an error detected below

    // use same packet from above, assume it's correct:
    // do we reject a good packet?
    int test_result = EA_test_packet(pkt);
    if (test_result != ESP32Ard_packet_check_OK)
        {Serial.println("  known good packet is rejected");
         Serial.print("    error code: ");Serial.println(test_result);
         EA_dump_packet_bytes(pkt);
         result = FAIL;  // no negative error codes for known-good pkt
        }

    // Now we make some specific errors and make sure they're detected.
    pkt[0] = 0xAA;
    test_result = EA_test_packet(pkt);
    if (test_result != ESP32Ard_bad_pkt_header) {
        Serial.println("  failed to detect broken pkt header[0]");
        Serial.print("    error code: ");Serial.println(test_result);
         EA_dump_packet_bytes(pkt);
        result=FAIL;
    }
    pkt[0] = 0xFF; // reset the error

    pkt[1] = 1;
    test_result = EA_test_packet(pkt);
    if (test_result != ESP32Ard_bad_pkt_header) {
        Serial.println("  failed to detect broken pkt header[1]");
        Serial.print("    error code: ");Serial.println(test_result);
         EA_dump_packet_bytes(pkt);
        result=FAIL;
    }
    pkt[1] = 0; //reset


    pkt[2] = 0; // correct val 3
    test_result = EA_test_packet(pkt);
    if (test_result != ESP32Ard_payload_size_zero) {
        Serial.println("  failed to detect zero payload length in pkt header[2]");
        Serial.print("    error code: ");Serial.println(test_result);
         EA_dump_packet_bytes(pkt);
        result=FAIL;
    }
    pkt[2] = 3; // reset

    pkt[6] = 20;  // correct: 6
    test_result = EA_test_packet(pkt);
    if (test_result != ESP32Ard_packet_cksum_error) {
        Serial.println("  failed to detect incorrect payload checksum");
        Serial.print("    error code: ");Serial.println(test_result);
         EA_dump_packet_bytes(pkt);
        result=FAIL;
    }
    pkt[6] = 6; // reset

    ppassfail(4,result);

}

void loop(void) {
    while(1){
        delay(1000);
    }
}
