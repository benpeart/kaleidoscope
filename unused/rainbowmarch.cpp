#include "main.h"
#include "Kaleidoscope.h"
#include "rainbowmarch.h"

#ifdef DEMO

#define DEFAULT_MILLIS 50
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// https://github.com/atuline/FastLED-Demos/blob/master/rainbow_march/rainbow_march.ino
void mode_kaleidoscope_rainbowMarch()
{
    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS)
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        uint8_t thisdelay = 200, deltahue = 255 / NUM_LEDS;
        uint8_t thishue = millis() * (255 - thisdelay) / 255; // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.

        // thishue = beat8(50);           // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
        // thishue = beatsin8(50,0,255);  // This can change speeds on the fly. You can also add these to each other.

        fill_kaleidoscope_rainbow(leds, thishue, deltahue);
    }

    adjustBrightness();
    adjustSpeed();
}

#endif
