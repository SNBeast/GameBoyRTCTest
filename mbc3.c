#include "mbc3.h"

#define SAVE_BANK   0x00

#define RTC_SECONDS 0x08
#define RTC_MINUTES 0x09
#define RTC_HOURS   0x0A
#define RTC_DAYSLOW 0x0B
#define RTC_MISC    0x0C

// the call and return wastes ten bus cycles, which is 10 microseconds DMG speed and 5 microseconds CGB speed. a 4 microsecond gap is recommended between RTC register accesses
static void delayForRTC (void) {
    return;
}

static void latchRTC (void) {
    volatile uint8_t* rtcLatchAddress = (volatile uint8_t*)0x6000;
    *rtcLatchAddress = 0;
    *rtcLatchAddress = 1;
}

static uint8_t getRTCValue (unsigned char bank) {
    SWITCH_RAM(bank);
    uint8_t returnValue = _SRAM[0];
    delayForRTC();
    SWITCH_RAM(SAVE_BANK);

    return returnValue;
}

static void setRTCValue (unsigned char bank, uint8_t value) {
    SWITCH_RAM(bank);
    _SRAM[0] = value;
    delayForRTC();
    SWITCH_RAM(SAVE_BANK);
}

static void haltRTC (void) {
    setRTCValue(RTC_MISC, getRTCValue(RTC_MISC) | 0x40);
}

static void unhaltRTC (void) {
    setRTCValue(RTC_MISC, getRTCValue(RTC_MISC) & ~0x40);
}

bool rtcOverflowed (void) {
    latchRTC();
    bool returnValue = getRTCValue(RTC_MISC) & 0x80;
    latchRTC();
    return returnValue;
}

uint16_t rtcDays (void) {
    latchRTC();
    uint16_t returnValue = getRTCValue(RTC_DAYSLOW);
    returnValue |= getRTCValue(RTC_MISC) << 8;
    latchRTC();
    return returnValue & 0x1FF; // i for some reason don't fully believe that the day counter is nine-bit
}

uint8_t rtcHours (void) {
    latchRTC();
    uint8_t returnValue = getRTCValue(RTC_HOURS);
    latchRTC();
    return returnValue;
}

uint8_t rtcMinutes (void) {
    latchRTC();
    uint8_t returnValue = getRTCValue(RTC_MINUTES);
    latchRTC();
    return returnValue;
}

uint8_t rtcSeconds (void) {
    latchRTC();
    uint8_t returnValue = getRTCValue(RTC_SECONDS);
    latchRTC();
    return returnValue;
}

void blankRTC (void) {
    haltRTC();
    for (int i = RTC_SECONDS; i <= RTC_MISC; i++) {
        setRTCValue(i, 0);
    }
    // unhaltRTC(); done by clearing the relevant bit above in RTC_MISC
}
