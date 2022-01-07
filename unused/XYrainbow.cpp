#include "main.h"
#include "Kaleidoscope.h"
#include "XY.h"
#include "XYrainbow.h"

#ifdef DEMO

#define DEFAULT_MILLIS 500
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

static void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
    byte lineStartHue = startHue8;
    for (byte y = 0; y < NUM_ROWS; y++)
    {
        lineStartHue += yHueDelta8;
        byte pixelHue = lineStartHue;
        for (byte x = 0; x < NUM_COLS; x++)
        {
            pixelHue += xHueDelta8;
            leds[XY(x, y)] = CHSV(pixelHue, 255, 255);
        }
    }
    leds_dirty = true;
}

void mode_xy_rainbow()
{
    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS)
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        uint32_t ms = millis();
        int32_t yHueDelta32 = ((int32_t)cos16(ms * (27 / 1)) * (350 / NUM_COLS));
        int32_t xHueDelta32 = ((int32_t)cos16(ms * (39 / 1)) * (310 / NUM_ROWS));
        drawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    }

    adjustBrightness();
    adjustSpeed();
}

#endif