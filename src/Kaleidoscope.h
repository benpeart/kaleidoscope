#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

void mode_off();
void mode_kaleidoscope();
void mode_kaleidoscope_select_speed_brightness();
//void mode_kaleidoscope_select_disks();
void mode_kaleidoscope_select_reflection_style();

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test();
#endif

#endif // KALEIDOSCOPE_H
