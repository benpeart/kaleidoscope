#include "main.h"
#include "Kaleidoscope.h"
#include "beatwave.h"

#ifdef DEMO

// Palette definitions
static CRGBPalette16 currentPalette;
static CRGBPalette16 targetPalette;
static TBlendType currentBlending = LINEARBLEND;

// https://github.com/atuline/FastLED-Demos/blob/master/beatwave/beatwave.ino
void beatwave()
{
    uint8_t wave1 = beatsin8(9, 0, 255); // That's the same as beatsin8(9);
    uint8_t wave2 = beatsin8(8, 0, 255);
    uint8_t wave3 = beatsin8(7, 0, 255);
    uint8_t wave4 = beatsin8(6, 0, 255);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        drawPixel(leds, i, ColorFromPalette(currentPalette, i + wave1 + wave2 + wave3 + wave4, 255, currentBlending));
    }
}

void mode_kaleidoscope_beatWave()
{
    beatwave();

    EVERY_N_MILLISECONDS(100)
    {
        uint8_t maxChanges = 24;
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
        leds_dirty = true;
    }

    EVERY_N_SECONDS(5)
    { // Change the target palette to a random one every 5 seconds.
        targetPalette = CRGBPalette16(
            CHSV(random8(), 255, random8(128, 255)), 
            CHSV(random8(), 255, random8(128, 255)), 
            CHSV(random8(), 192, random8(128, 255)), 
            CHSV(random8(), 255, random8(128, 255)));
        leds_dirty = true;
    }

    adjustBrightness();
}

#endif
