// --------------------------------------------------------------------------------------------------
// ttn-abp-mode.ino
// The Things Network basic ABP test sample using Heltec Lora32 V2 board
// v1.0
// --------------------------------------------------------------------------------------------------

#include <U8x8lib.h>
#include <stdlib.h>
#include <stdarg.h>
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

// TX interval in seconds (minimum interval, may be longer)
const unsigned TX_INTERVAL = 60;

// Data buffer
static uint8_t mydata[] = "Hello, world!";
// osjob_t structure
static osjob_t sendjob;

// Simple print screen
void oledPrint (char* format, ...) {
    char buffer[255];
    va_list argptr;
    va_start (argptr, format);
    sprintf (buffer, format, argptr);
    u8x8.drawString(0, 1, buffer);
    va_end (argptr);
}
// LMIC event handler
void onEvent (ev_t ev) {
    // Serial.print(os_getTime());
    // Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            oledPrint("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            oledPrint("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            oledPrint("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            oledPrint("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            oledPrint("EV_JOINING");
            break;
        case EV_JOINED:
            oledPrint("EV_JOINED");
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     oledPrint(F("EV_RFU1"));
        ||     break;
        */
        case EV_JOIN_FAILED:
            oledPrint("EV_JOIN_FAILED");
            break;
        case EV_REJOIN_FAILED:
            oledPrint("EV_REJOIN_FAILED");
            break;
        case EV_TXCOMPLETE:
            oledPrint("EV_TXCOMPLETE (includes waiting for RX windows)");
            if (LMIC.txrxFlags & TXRX_ACK)
              oledPrint("Received ack");
            if (LMIC.dataLen) {
              oledPrint("Rec. %b bytes", LMIC.dataLen);
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            oledPrint("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            oledPrint("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            oledPrint("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            oledPrint("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            oledPrint("EV_LINK_ALIVE");
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    oledPrint(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            oledPrint("EV_TXSTART");
            break;
        case EV_TXCANCELED:
            oledPrint("EV_TXCANCELED");
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            oledPrint("EV_JOIN_TXCOMPLETE: no JoinAccept");
            break;
        default:
            oledPrint("Unknown event: %u", (unsigned) ev);
            break;
    }
}

// do_send
void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        oledPrint("OP_TXRXPEND, not sending");
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        oledPrint("Packet queued");
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup () {
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    oledPrint("LoRaWan ABP test");
    
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession (0x13, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_eu868)
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job
    do_send(&sendjob);
}

void loop () {
    os_runloop_once();
}