#include "main.h"
#include "render.h"
#include "XYfire.h"

#ifdef DEMO

#define DEFAULT_MILLIS 100
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

void mode_xy_fire()
{
    static byte scale = 64;
    static byte speed = 92;
    static uint32_t t;

    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS)
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        t += speed;
        for (byte x = 0; x < NUM_COLS; x++)
        {
            for (byte y = 0; y < NUM_ROWS; y++)
            {

                int16_t Bri = inoise8(x * scale, (y * scale) - t) - (y * (255 / NUM_ROWS));
                byte Col = Bri; // inoise8(x * scale, (y * scale) - t) - (y * (255 / NUM_ROWS));
                if (Bri < 0)
                    Bri = 0;
                if (Bri != 0)
                    Bri = 256 - (Bri * 0.2);
                nblend(leds[XY(x, NUM_ROWS - y)], ColorFromPalette(HeatColors_p, Col, Bri), speed);
            }
        }

        leds_dirty = true;
    }

    adjustBrightness();
    adjustSpeed();
}

#endif
