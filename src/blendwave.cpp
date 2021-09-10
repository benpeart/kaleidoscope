#include "main.h"
#include "Kaleidoscope.h"
#include "blendwave.h"

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/blendwave/blendwave.ino
void mode_kaleidoscope_blendWave()
{
    static CRGB clr1;
    static CRGB clr2;
    static uint8_t speed;
    static uint8_t loc1;

    EVERY_N_MILLIS_I(timer, 50)
    {
        timer.setPeriod(constrain(ms_between_frames - DEFAULT_SPEED_DELAY + 50, 0, MAX_SPEED_DELAY));
        speed = beatsin8(6, 0, 255);

        clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
        clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

        loc1 = beatsin8(10, 0, NUM_LEDS - 1);

        fill_kaleidoscope_gradient_RGB(leds, 0, clr2, loc1, clr1);
        fill_kaleidoscope_gradient_RGB(leds, loc1, clr2, NUM_LEDS - 1, clr1);
    }

    adjustBrightness();
    adjustSpeed();
}

#endif
