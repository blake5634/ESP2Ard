/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * September 2023, Blake Hannaford
 *    ESP32 test code for the library ESP2Ard
 *      using example code for the Lilygo T-embed board/UI
 *
 *   For basic example use of serial coms, see
 *   1) first few lines of void app_main(void)  (initialization)
 *   2) void button_press_down_cb()             (make and send a packet)
 *
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "math.h"
#include "iot_button.h"
#include "iot_knob.h"
#include "tembed.h"
#include "apa102.h"
#include "tembed_lvgl.h"

//   set up the ESP2Ard serial coms library
#include "ESP2Ard.h"

// Set the number of LEDs to control.
const uint16_t ledCount = CONFIG_APA102_LED_COUNT;

// We define "power" in this sketch to be the product of the
// 8-bit color channel value and the 5-bit brightness register.
// The maximum possible power is 255 * 31 (7905).
const uint16_t maxPower = 255 * 31;

// The power we want to use on the first LED is 1, which
// corresponds to the dimmest possible white.
const uint16_t minPower = 1;

// This function sends a white color with the specified power,
// which should be between 0 and 7905.
void LED_ring_disp_white(const apa102_t *apa102,uint16_t power)
{
  // Choose the lowest possible 5-bit brightness that will work.
  uint8_t brightness5Bit = 1;
  while(brightness5Bit * 255 < power && brightness5Bit < 31)
  {
    brightness5Bit++;
  }

  // Uncomment this line to simulate an LED strip that does not
  // have the extra 5-bit brightness register.  You will notice
  // that roughly the first third of the LED strip turns off
  // because the brightness8Bit equals zero.
  //brightness = 31;

  // Set brightness8Bit to be power divided by brightness5Bit,
  // rounded to the nearest whole number.
  uint8_t brightness8Bit = (power + (brightness5Bit / 2)) / brightness5Bit;

  // Send the white color to the LED strip.  At this point,
  // brightness8Bit multiplied by brightness5Bit should be
  // approximately equal to power.
  apa102_sendColor(apa102,brightness8Bit, brightness8Bit, brightness8Bit, brightness5Bit);
}


void leds(tembed_t tembed) {
    ESP_LOGI(TAG, "LEDS");
// Calculate what the ratio between the powers of consecutive
// LEDs needs to be in order to reach the max power on the last
// LED of the strip.
//     float multiplier = pow(maxPower / minPower, 1.0 / (ledCount - 1));
    apa102_startFrame(&tembed->leds);
    float power = minPower;
    power = 31*255 * 0.025;
    for(uint16_t i = 0; i < ledCount; i++)
    {
        LED_ring_disp_white(&tembed->leds,power);
//         power = power * multiplier;
    }
    apa102_endFrame(&tembed->leds,ledCount);
}

// styles we need to have globally and initializers for them
lv_style_t l_style;

lv_style_t setup_l_style(){
    lv_style_t tstyle;
    lv_style_init(&tstyle);
    lv_style_set_width(&tstyle, 75);
    lv_style_set_height(&tstyle, 40);
    lv_style_set_text_color(&tstyle, lv_color_white());
    lv_style_set_text_align(&tstyle, LV_TEXT_ALIGN_CENTER);
    lv_style_set_bg_color(&tstyle, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_opa(&tstyle,LV_OPA_COVER);
    return tstyle;
}

lv_style_t l_styleWFocusNext;
lv_style_t l_styleWFocusClicked;

lv_style_t setup_l_styleOutlined(int outlineColor_hx){
    /// Add outline
    lv_style_t tstyle;
    lv_style_init(&tstyle);
    lv_style_set_width(&tstyle, 75);
    lv_style_set_height(&tstyle, 40);
    lv_style_set_outline_width(&tstyle,  2);
    lv_style_set_outline_color(&tstyle, lv_color_hex(outlineColor_hx));
    lv_style_set_radius(&tstyle, 4);
    lv_style_set_outline_pad(&tstyle, 8);
    return tstyle;
}

//
//   declarations for the graphics in our app
//
enum levels {LEVEL_TOP, LEVEL_BUTTON};  // select button? or in a button?
enum levels level = LEVEL_TOP;
lv_obj_t *count_label;
lv_obj_t *name_label;
#define NNAMES 4
char* names[] = {"Jane", "Mary", "Zhang", "Kyle"};

#define NFOCI  3
lv_obj_t *foci_buttons[NFOCI];
lv_obj_t *currentFocus;
int foci_idx = 0;

// change time in HH:MM format up and down using the knob

int start_hour=06;  // start time for dawn
int start_min=30;

static void change_HHMM(int dial)
{

    static int startupflag = 1;
    static int knobPstate = 0;
    if (startupflag) {
        knobPstate = dial;
        startupflag = 0;
    }
    int del = dial-knobPstate;
    if (del < 0){
        start_min--;
        if (start_min<0){
            start_min=59;
            start_hour--;}
    }
    else if(del>0){
        start_min++;
        if (start_min>59){
            start_min=0;
            start_hour++;
            }
    }
    if (start_hour>24){
        start_hour=0;}
    if (start_hour<0){
        start_hour=24;}

    knobPstate = dial; // ready for next time
}


//
//  This is the logic for T-embed UI elements: turning knob and pressing the button
//

//  graphics object labels for buttons
lv_obj_t *SetTime = NULL;
lv_obj_t *SetName = NULL;

static void knob_turn_cb(void *arg, void *data)
{
    int xkn = iot_knob_get_count_value((knob_handle_t)arg);
    if (level == LEVEL_BUTTON){
       /*
        *    change value in the button
        */
        ESP_LOGI(TAG, "LEVEL_BUTTON: KNOB: Change Button value: KNOB_TURN Count is %d", xkn);
        if (currentFocus == SetTime) {
            /*
             * change HH:MM time setting
             */
            ESP_LOGI(TAG, "  KNOB: change start time KNOB_TURN Count is %d", xkn);
            change_HHMM(xkn);
            lv_label_set_text_fmt(SetTime, "%02d:%02d", start_hour, start_min);
        }
        if (currentFocus == SetName){
            lv_label_set_text_fmt(count_label,"%d", xkn);
            int idx = (NNAMES*100+xkn)%NNAMES; // avoid neg values (hack)
            char* namestr = names[idx];
            lv_label_set_text_fmt(name_label,namestr);
        }
    }
    else if (level == LEVEL_TOP){
        //
        //   move focus highlight
        //

        ESP_LOGI(TAG, "LEVEL_TOP: KNOB: Change Button value: KNOB_TURN Count is %d", xkn);

        lv_obj_remove_style(currentFocus, &l_styleWFocusNext, LV_PART_MAIN);
        lv_obj_add_style(currentFocus,   &l_style, LV_PART_MAIN);

        //  change focus and move highlight border(foci[foci_idx])
        foci_idx = (NFOCI*100+xkn)%NFOCI;   //  prevent negative indices
        currentFocus = foci_buttons[foci_idx];
        // set highlight border
        lv_obj_remove_style(currentFocus, &l_style,   LV_PART_MAIN);
        lv_obj_add_style(currentFocus,  &l_styleWFocusNext, LV_PART_MAIN);

        }
}


static void button_press_down_cb(void *arg, void *data) {
    ESP_LOGI(TAG, "a click (down)...");
    if (level == LEVEL_BUTTON){
       /*
        *    back to top, icon selection, level
        */
       ESP_LOGI(TAG, "Button - exiting back to top level");
       level = LEVEL_TOP;
       lv_obj_remove_style(currentFocus, &l_styleWFocusClicked, LV_PART_MAIN);
       lv_obj_add_style(currentFocus,  &l_styleWFocusNext, LV_PART_MAIN);

       if (currentFocus==SetTime){
           // we also send new time on exit from button level
            EA_msg_byte pkt[ESP32Ard_max_packet_size]={'\0'};
            EA_msg_byte payld[2] = {'\0'}; // for this application, sending hour & min
            payld[0] = (EA_msg_byte) start_hour;
            payld[1] = (EA_msg_byte) start_min;
            ESP_LOGI(TAG,"building and sending packet");
            int pktL = EA_pkt_build(pkt, 2, payld);  // 2 = payload length
            EA_dump_packet_bytes(pkt);
            ESP_LOGI(TAG," built a packet of length: %d  ",pktL);

//             send_to_Arduino(start_hour, start_min);
            EA_write_pkt_serial(pkt,pktL);

        //        send_to_Arduino(start_hour, start_min);

            ESP_LOGI(TAG,"sent hour/min to Arduino (the NEW way)");
       }
    }
    else if (level == LEVEL_TOP){
        /*
         *  select focus
         */
        ESP_LOGI(TAG, "Button - go into button level");
        level = LEVEL_BUTTON;
        lv_obj_remove_style(currentFocus, &l_styleWFocusNext, LV_PART_MAIN);
        lv_obj_add_style(currentFocus,  &l_styleWFocusClicked, LV_PART_MAIN);    }
}

// set some styles, objects, and colors

void lvgl_demo_ui(lv_disp_t *disp) {

    // set some global styles
    l_style       = setup_l_style();
    l_styleWFocusNext    = setup_l_styleOutlined(0xFF0000);  // red outline
    l_styleWFocusClicked = setup_l_styleOutlined(0x80FF80);  // green outline

    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_disp_get_scr_act(disp), lv_color_hex(0x99CC99), LV_PART_MAIN);

    /*   styles are now global so can be swapped */

    /*Create a display container with ROW flex direction*/
    lv_obj_t* cont_row = lv_obj_create(lv_disp_get_scr_act(disp));
    lv_obj_set_size(cont_row, 300, 75);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_color(cont_row, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);

    lv_obj_t* label;

    /*Add items to the row*/
    label = lv_label_create(cont_row);
    lv_label_set_text_static(label, "Start Time");
    lv_obj_add_style(label, &l_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(label, lv_color_hex(0xff0000), LV_PART_MAIN);
    lv_obj_center(label);
    foci_buttons[0] = label;

    label = lv_label_create(cont_row);
    lv_label_set_text_static(label, "HH:MM");
    lv_obj_add_style(label, &l_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(label, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_center(label);
    foci_buttons[1] = label;
    SetTime = label;


/*
 *     "Test Area" container row
 */
    /*Create a display container For "Test area" */
    lv_obj_t * cont_TA = lv_obj_create(lv_disp_get_scr_act(disp));
    lv_obj_set_size(cont_TA, 250, 50);
    lv_obj_align(cont_TA, LV_ALIGN_BOTTOM_MID, 0, -25);
    lv_obj_set_flex_flow(cont_TA, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_color(cont_TA, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);

    /*Add items to the row*/
    label = lv_label_create(cont_TA);
    lv_obj_set_size(label, 150, 20);
    lv_label_set_text_static(label, "Set Name");
    lv_obj_add_style(label, &l_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(label, lv_color_hex(0xff0000), LV_PART_MAIN);
    lv_obj_center(label);
    foci_buttons[2] = label;
    SetName = label;
    currentFocus = label;

    /*
     *   label at bottom of main window for scroll demo
     */
    // Create a white label, set its text and align it to the center
    count_label = lv_label_create(lv_disp_get_scr_act(disp));
    lv_label_set_text(count_label, "0");
    lv_obj_set_style_text_color(count_label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, -20, 0);
    lv_obj_set_style_bg_color(count_label, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(count_label, LV_OPA_COVER, LV_PART_MAIN);

    // Create a label for the scrolling name, set its text and align it to the center
    name_label = lv_label_create(lv_disp_get_scr_act(disp));
    lv_label_set_text(name_label, "_name_");
    lv_obj_set_style_text_color(name_label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(name_label, LV_ALIGN_BOTTOM_MID, 50, 0);
    lv_obj_set_style_bg_color(name_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(name_label, LV_OPA_COVER, LV_PART_MAIN);
}



void app_main(void)
{

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%uMB %s flash\n", flash_size / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    // Initialize the T-Embed
    tembed_t tembed = tembed_init(notify_lvgl_flush_ready, &lvgl_disp_drv);

    leds(tembed);

    iot_button_register_cb(tembed->dial.btn, BUTTON_PRESS_DOWN, button_press_down_cb, NULL);

    // much better to have one callback for both directions!
    iot_knob_register_cb(tembed->dial.knob, KNOB_LEFT , knob_turn_cb, NULL);
    iot_knob_register_cb(tembed->dial.knob, KNOB_RIGHT, knob_turn_cb, NULL);
//     iot_knob_register_cb(tembed->dial.knob, KNOB_RIGHT, knob_right_cb, NULL);


    // set up lvgl to handle the screen
    lv_disp_t *lvgl_disp = tembed_lvgl_init(tembed);
    ESP_LOGI(TAG, "Display LVGL");
    lvgl_demo_ui(lvgl_disp);

    ESP_LOGI(TAG,"starting time delay");
    EA_delay_ms(2000);
    ESP_LOGI(TAG,"Initializing Serial Port 2");

// legacy arduino
//     SerialPort.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
// legacy non library
//    SerialPort2init();
    EA_init_serial(ESP_PIN_RX, ESP_PIN_TX);  // ESP_PIN_XX configured in ESP2Ard.h

    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }
}
