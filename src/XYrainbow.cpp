#include "main.h"
#include "Kaleidoscope.h"
#include "XYIndex.h"
#include "XYrainbow.h"

#ifdef DEMO

static void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
    byte lineStartHue = startHue8;
    for (byte y = 0; y < kMatrixHeight; y++)
    {
        lineStartHue += yHueDelta8;
        byte pixelHue = lineStartHue;
        for (byte x = 0; x < kMatrixWidth; x++)
        {
            pixelHue += xHueDelta8;
            leds[XYToIndex(x, y)] = CHSV(pixelHue, 255, 255);
        }
    }
    leds_dirty = true;
}

void mode_xy_rainbow()
{
    EVERY_N_MILLIS(500)
    {
        uint32_t ms = millis();
        int32_t yHueDelta32 = ((int32_t)cos16(ms * (27 / 1)) * (350 / kMatrixWidth));
        int32_t xHueDelta32 = ((int32_t)cos16(ms * (39 / 1)) * (310 / kMatrixHeight));
        drawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    }

    adjustBrightness();
}

#endif