#include "main.h"
#include "Kaleidoscope.h"
#include "rainbowmarch.h"

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/rainbow_march/rainbow_march.ino
void mode_kaleidoscope_rainbowMarch()
{
    EVERY_N_MILLIS_I(timer, 50)
    {
        timer.setPeriod(constrain(ms_between_frames - DEFAULT_SPEED_DELAY + 50, 0, MAX_SPEED_DELAY));
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
