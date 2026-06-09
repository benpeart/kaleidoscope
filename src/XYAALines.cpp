#include "main.h"
#include "settings.h"
#include "render.h"
#include "XYAALines.h"

#ifdef DEMO

#define DEFAULT_MILLIS 1000
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// https://wokwi.com/arduino/projects/284541784463245837
CRGBPalette16 currentPalette = {
    0xFF0000, 0x7F0000, 0xAB5500, 0x552A00, 0xABAB00, 0x555500, 0x00FF00, 0x007F00,
    0x00AB55, 0x00552A, 0x0000FF, 0x00007F, 0x5500AB, 0x2A0055, 0xAB0055, 0x55002A};

void mode_xy_aalines()
{
    uint32_t ms = millis();

    // draw the background animation, just like the XYmatrix example
    // but with added distortion
    uint32_t yHueDelta = (int32_t)sin16(ms * 11) * 3;
    uint32_t xHueDelta = (int32_t)cos16(ms * 11) * 3;
    uint32_t startHue = ms << 8;
    uint32_t lineStartHue = startHue - (HEIGHT + 1) / 2 * yHueDelta;
    int16_t yd2 = sin16(ms * 3) / 4;
    int16_t xd2 = sin16(ms * 7) / 4;
    for (byte y = 0; y < HEIGHT; y++)
    {
        uint32_t pixelHue = lineStartHue - (WIDTH + 1) / 2 * xHueDelta;
        uint32_t xhd = xHueDelta;
        lineStartHue += yHueDelta;
        yHueDelta += yd2;
        for (byte x = 0; x < WIDTH; x++)
        {
            leds[XY(x, y)] = ColorFromPaletteExtended(currentPalette, pixelHue >> 7, 255, LINEARBLEND);
            pixelHue += xhd;
            xhd += xd2;
        }
    }

    // change `effect` every second repeating: 0,1,2,3,4,5,0,1,2...
    static uint8_t effect = 0;
    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS)
    {
        timer.setPeriod(MAX_MILLIS - map(settings.speed, MIN_SPEED, MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        if (++effect > 5)
            effect = 0;
    }

    adjustBrightness();
    adjustSpeed();
    leds_dirty = true;
}

#endif
