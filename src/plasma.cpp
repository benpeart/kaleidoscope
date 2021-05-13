#include <Arduino.h>
#include <FastLED.h>
#include "Kaleidoscope.h"
#include "plasma.h"

#ifdef DEMO

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

// https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino
void mode_kaleidoscope_plasma()
{
    static CRGBPalette16 currentPalette = ForestColors_p; // Palette definitions
    static CRGBPalette16 targetPalette;
    static TBlendType currentBlending = LINEARBLEND;

    EVERY_N_MILLISECONDS(50)
    {
        int thisPhase = beatsin8(6, -64, 64); // Setting phase change for a couple of waves.
        int thatPhase = beatsin8(7, -64, 64);

        for (int k = 0; k < NUM_LEDS; k++)
        {
            // For each of the LED's in the strand, set a brightness based on a wave as follows:
            int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2; // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
            int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

            kaleidoscope.drawPixel(leds, k, ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending)); // Let's now add the foreground colour.
        }
    }

    EVERY_N_MILLISECONDS(100)
    {
        uint8_t maxChanges = 24;
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
    }

    EVERY_N_SECONDS(5)
    {                              // Change the target palette to a random one every 5 seconds.
        uint8_t baseC = random8(); // You can use this as a baseline colour if you want similar hues in the next line.
        targetPalette = CRGBPalette16(CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)), CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)));
    }
}

#endif
