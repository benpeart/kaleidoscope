#include "main.h"
#include "Kaleidoscope.h"
#include "doom.h"

void mode_doom()
{
    //    int r = 255;
    //    int g = 24;
    //    int b = 0;

    //FastLED.clear(true);
    //    uint8_t wave = beatsin8(9, 200, 255);
    static CRGB primaryRedColor(255, 24, 0);

    if (leds_dirty)
    {
        //top left symbol
        leds[0 * NUM_LEDS_PER_STRIP + 46] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 72] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 74] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 99] = primaryRedColor;

        //top right symbol
        leds[(3) * NUM_LEDS_PER_STRIP + 14] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 16] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 17] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 44] = primaryRedColor;

        //bottom left symbol
        leds[0 * NUM_LEDS_PER_STRIP + 32] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 58] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 59] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 61] = primaryRedColor;
        leds[0 * NUM_LEDS_PER_STRIP + 86] = primaryRedColor;

        //bottom right symbol
        leds[(3) * NUM_LEDS_PER_STRIP + 1] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 2] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 29] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 32] = primaryRedColor;

        //main symbol
        leds[(0) * NUM_LEDS_PER_STRIP + 10] = primaryRedColor;
        leds[(0) * NUM_LEDS_PER_STRIP + 20] = primaryRedColor;
        leds[(0) * NUM_LEDS_PER_STRIP + 41] = primaryRedColor;

        for (int i = 1; i <= 13; i++)
        {
            leds[(1 + 1) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        for (int i = 26; i <= 37; i++)
        {
            leds[(1 + 1) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        leds[(1) * NUM_LEDS_PER_STRIP + 42] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 43] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 72] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 107] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 108] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 79] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 114] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 50] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 51] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 63] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 64] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 87] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 88] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 100] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 121] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 132] = primaryRedColor;
        leds[(1) * NUM_LEDS_PER_STRIP + 133] = primaryRedColor;

        for (int i = 3; i <= 13; i++)
        {
            leds[(2) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        for (int i = 26; i <= 35; i++)
        {
            leds[(2) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        for (int i = 82; i <= 86; i++)
        {
            leds[(2) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        for (int i = 101; i <= 104; i++)
        {
            leds[(2) * NUM_LEDS_PER_STRIP + i] = primaryRedColor;
        }

        leds[(2) * NUM_LEDS_PER_STRIP + 117] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 118] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 137] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 44] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 45] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 69] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 70] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 51] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 63] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 88] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 97] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 132] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 133] = primaryRedColor;
        leds[(2) * NUM_LEDS_PER_STRIP + 122] = primaryRedColor;

        leds[(3) * NUM_LEDS_PER_STRIP + 10] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 11] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 20] = primaryRedColor;
        leds[(3) * NUM_LEDS_PER_STRIP + 40] = primaryRedColor;
    }
}
