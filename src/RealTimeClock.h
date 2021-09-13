#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

void rtc_setup();

void draw_clock();
void mode_select_clock_face();

extern uint8_t clock_style; // Index of current clock face in table
#define N_CLOCK_STYLES 3
extern const PROGMEM char clockFaces[N_CLOCK_STYLES][16];

#endif // REALTIMECLOCK_H
