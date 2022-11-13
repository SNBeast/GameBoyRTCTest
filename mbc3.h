#include <gb/gb.h>

#include <stdbool.h>
#include <stdint.h>

extern bool rtcOverflowed (void);
extern uint16_t rtcDays (void);
extern uint8_t rtcHours (void);
extern uint8_t rtcMinutes (void);
extern uint8_t rtcSeconds (void);
extern void blankRTC (void);
