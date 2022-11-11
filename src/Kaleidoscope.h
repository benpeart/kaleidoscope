#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

void mode_off();
void mode_kaleidoscope();
void mode_kaleidoscope_select_speed_brightness();
//void mode_kaleidoscope_select_disks();
void mode_kaleidoscope_select_reflection_style();

#ifdef DEBUG
// test the wiring and ensure all pixels light up correctly
// Q: Why does led[300] ==> led[312] not light up?
// A: Our strips aren't the same length (156 vs 144) so the shorter strips (1 and 2)
// have extra leds[x] positions that don't have physical LEDs.
void mode_test();

// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test();
#endif

#endif // KALEIDOSCOPE_H
