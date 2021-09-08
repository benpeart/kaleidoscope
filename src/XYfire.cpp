#include "main.h"
#include "Kaleidoscope.h"
#include "XYIndex.h"
#include "XYfire.h"

#ifdef DEMO

byte scale = 64;
byte speed = 92;
static uint32_t t;
void mode_xy_fire()
{
    EVERY_N_MILLISECONDS(100)
    {
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
                nblend(leds[XYToIndex(x, NUM_ROWS - y)], ColorFromPalette(HeatColors_p, Col, Bri), speed);
            }
        }

        leds_dirty = true;
    }

    adjustBrightness();
}

#endif
