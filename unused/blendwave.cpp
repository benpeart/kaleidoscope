#include "main.h"
#include "render.h"
#include "blendwave.h"

#ifdef DEMO

#define DEFAULT_MILLIS 50
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// https://github.com/atuline/FastLED-Demos/blob/master/blendwave/blendwave.ino
void mode_kaleidoscope_blendWave()
{
    static CRGB clr1;
    static CRGB clr2;
    static uint8_t speed;
    static uint8_t loc1;

    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS)
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
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
