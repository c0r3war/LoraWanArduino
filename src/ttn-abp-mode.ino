// --------------------------------------------------------------------------------------------------
// ttn-abp-mode.ino
// The Things Network basic ABP test sample using Heltec Lora32 V2 board
// v1.0
// --------------------------------------------------------------------------------------------------

#include <U8x8lib.h>
#include <stdlib.h>
#include <Arduino.h>
// Secrets that must not bet uploaded to GIT repo
#include "secrets.h" 
// LoraWan LMIC inlcudes
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>


// Just for Intellisense complains
#ifndef ARDUINO
#define ARDUINO
#endif

//How to get internal temperature (warning: sensor has been removed on the last version of ESP 32)
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();


// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);


void setup () {
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 1, "LoRaWan ABP test");

}

void loop () {

}