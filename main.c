#include "mbc3.h"
#include <stdio.h>
#include <string.h>

const char* magic = "rtcTest";

int main (void) {
    ENABLE_RAM;
    if (memcmp((void*)_SRAM, magic, 8)) {
        memcpy((void*)_SRAM, magic, 8);
        blankRTC();
        puts("Initialized data.\n\nEvery load, the time elapsed since the last RTC check will be shown.\n\nEvery check requires a reset.");
    }
    else {
        if (rtcOverflowed()) {
            puts("The RTC is signaling an overflow. The following information is likely wrong.\n\n");
        }

        // this has to be done with these particular semantics because of the broken printf implementation
        // and because of having to do some permutation to make the broken printf work
        uint16_t days = rtcDays();
        uint8_t hours = rtcHours();
        uint8_t minutes = rtcMinutes();
        uint8_t seconds = rtcSeconds();
        printf("It has been %u days, %hu hours, %hu minutes, and %hu seconds since the RTC was last checked.", (uint16_t)days, (uint8_t)hours, (uint8_t)minutes, (uint8_t)seconds);

        blankRTC();
    }
    DISABLE_RAM;

    return 0;
}
