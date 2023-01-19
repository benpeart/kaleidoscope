#include "main.h"
#include "modes.h"

#include "Kaleidoscope.h"
#ifdef DEMO
#include "plasma.h"
#include "ripples.h"
#include "TwinkleFox.h"
#include "XYDistortionWaves.h"
#include "XYAALines.h"
#include "XYmatrix.h"
#include "XYpacifica.h"
#include "XYsnake.h"
#include "XYfire.h"
#endif

#ifdef TIME
#include "RealTimeClock.h"
#endif // TIME

// This look up table lists each of the display/animation drawing functions
// (which appear later in this code) in the order they're selected with
// the right button.  Some functions appear repeatedly...for example,
// we return to "mode_off" at several points in the sequence.
KaleidoscopeMode KaleidoscopeModeLUT[]{
    {mode_kaleidoscope, "Kaleidoscope", true, {0, 0}},
#ifndef WIFI // if we have WIFI, we don't need the manual settings modes
#ifdef ENCODER
    {mode_off, "off", false, {0, 0}}, // make it obvious we're entering 'setup' modes
    {mode_kaleidoscope_select_speed_brightness, "kaleidoscope_select_speed_brightness", false, {0, 0}},
    //  { mode_kaleidoscope_select_disks, "mode_kaleidoscope_select_disks", false, {0, 0}},
    {mode_kaleidoscope_select_reflection_style, "mode_kaleidoscope_select_reflection_style", false, {0, 0}},
#ifdef TIME
    {mode_select_clock_face, "select_clock_face", false, {0, 0}},
#endif // TIME
#endif // ENCODER
#endif // WIFI
#ifdef DEMO
    {mode_kaleidoscope_plasma, "Plasma", true, {0, 0}},
    {mode_kaleidoscope_ripples, "Ripples", true, {0, 0}},
    {mode_kaleidoscope_twinkle_fox, "Twinkle Fox", true, {0, 0}},
    {mode_xy_aalines, "AA Lines", true, {0, 0}},
    {mode_xy_distortion_waves, "Distortion Waves", true, {0, 0}},
    {mode_xy_matrix, "Matrix", true, {0, 0}},
    {mode_xy_pacifica, "Pacifica", true, {0, 0}},
    {mode_xy_snake, "Snake", true, {0, 0}},
    {mode_xy_fire, "Fire", true, {0, 0}},
#endif
#ifdef DEBUG
    {mode_kaleidoscope_test, "kaleidoscope_test", true, {0, 0}},
    {mode_xy_test, "xy_test", true, {0, 0}},
    {mode_test, "test", true, {0, 0}},
#endif
    {mode_off, "off", false, {0, 0}} // make it obvious we're entering 'regular' modes
};
uint8_t kaleidoscope_mode = 0;                                                               // Index of current mode in table
uint8_t kaleidoscope_modes = (sizeof(KaleidoscopeModeLUT) / sizeof(KaleidoscopeModeLUT[0])); // total number of valid modes in table

#ifdef ENCODER
// We need to save/restore the count for the rotary encoders
// when we switch between modes so that each mode doesn't impact
// the other modes.
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1
#endif // ENCODER

void setKaleidoscopeMode(int new_mode)
{
    // if the mode changed
    if (kaleidoscope_mode != new_mode)
    {
#ifdef ENCODER
        int old_mode = kaleidoscope_mode;

        // save the encoder count for the old mode and restore the new mode count
        KaleidoscopeModeLUT[old_mode].modeEncoderCounts[LEFT_ENCODER] = knobLeft.getCount();
        knobLeft.setCount(KaleidoscopeModeLUT[new_mode].modeEncoderCounts[LEFT_ENCODER]);
        KaleidoscopeModeLUT[old_mode].modeEncoderCounts[RIGHT_ENCODER] = knobLeft.getCount();
        knobLeft.setCount(KaleidoscopeModeLUT[new_mode].modeEncoderCounts[RIGHT_ENCODER]);
#endif

        // output the new mode name and clear the led strips for the new mode
        kaleidoscope_mode = new_mode;
        DB_PRINTF("setKaleidoscopeMode: %s\r\n", KaleidoscopeModeLUT[kaleidoscope_mode].modeName);
        FastLED.clear(true);
        leds_dirty = true;
    }
}

void nextKaleidoscopeMode()
{
    // check for a mode change
    uint8_t new_mode = kaleidoscope_mode;

    if (new_mode < (kaleidoscope_modes - 1))
        new_mode++; // Advance to next mode
    else
        new_mode = 0; // or "wrap around" to start

    setKaleidoscopeMode(new_mode);
}

void previousKaleidoscopeMode()
{
    // check for a mode change
    uint8_t new_mode = kaleidoscope_mode;

    if (new_mode)
        new_mode--; // Go to prior mode
    else
        new_mode = kaleidoscope_modes - 1; // or "wrap around" to last mode

    setKaleidoscopeMode(new_mode);
}

// All Pixels off
void mode_off()
{
    // nothing to see here... (the pixels got cleared by the button press)
}
