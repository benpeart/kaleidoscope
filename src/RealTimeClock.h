#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

void rtc_setup();

#ifdef WEATHER
#define N_CLOCK_FACES 4
#else
#define N_CLOCK_FACES 3
#endif
extern uint8_t clock_face; // Index of current clock face in table
extern const PROGMEM char clockFaces[N_CLOCK_FACES][16];

extern CRGB clockColor;

void draw_clock();
void mode_select_clock_face();
int set_clock_face(int new_face);

#endif // REALTIMECLOCK_H
