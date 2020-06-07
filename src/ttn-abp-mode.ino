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

// LMIC LoraWan setup
// LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = _NWKSKEY;        // secrets.h

// LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = _APPSKEY;       // secrets.h

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
static const u4_t DEVADDR = _DEVADDR;                   // secrets.h

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

// Setup the pins to be used to communicate with Semtech lx1297 chip (specific to Heltec Lora32 v2)
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,                       // reset pin
    .dio = {26, 33, 32}, 
};

void setup () {
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 1, "LoRaWan ABP test");

}

void loop () {

}