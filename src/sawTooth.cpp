#include <Arduino.h>
#include <FastLED.h>
#include "Kaleidoscope.h"
#include "sawtooth.h"

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/sawtooth/sawtooth.ino
void mode_kaleidoscope_sawTooth()
{
    // Palette definitions
    static CRGBPalette16 currentPalette = PartyColors_p;
    static TBlendType currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND

    int bpm = 60;
    int ms_per_beat = 60000 / bpm; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
    int ms_per_led = 60000 / bpm / NUM_LEDS;

    int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (NUM_LEDS); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

    if (cur_led == 0)
        kaleidoscope.fill_solid(leds, CRGB::Black);
    else
        kaleidoscope.drawPixel(leds, cur_led, ColorFromPalette(currentPalette, 0, 255, currentBlending)); // I prefer to use palettes instead of CHSV or CRGB assignments.
}

#endif
