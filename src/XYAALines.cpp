#include "main.h"
#include "render.h"
#include "XYAALines.h"

#ifdef DEMO

#define DEFAULT_MILLIS 1000
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// https://github.com/FastLED/FastLED/pull/202
CRGB ColorFromPaletteExtended(const CRGBPalette32 &pal, uint16_t index, uint8_t brightness, TBlendType blendType)
{
    // Extract the five most significant bits of the index as a palette index.
    uint8_t index_5bit = (index >> 11);
    // Calculate the 8-bit offset from the palette index.
    uint8_t offset = (uint8_t)(index >> 3);
    // Get the palette entry from the 5-bit index
    const CRGB *entry = &(pal[0]) + index_5bit;
    uint8_t red1 = entry->red;
    uint8_t green1 = entry->green;
    uint8_t blue1 = entry->blue;

    uint8_t blend = offset && (blendType != NOBLEND);
    if (blend)
    {
        if (index_5bit == 31)
        {
            entry = &(pal[0]);
        }
        else
        {
            entry++;
        }

        // Calculate the scaling factor and scaled values for the lower palette value.
        uint8_t f1 = 255 - offset;
        red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
        green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
        blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);

        // Calculate the scaled values for the neighbouring palette value.
        uint8_t red2 = entry->red;
        uint8_t green2 = entry->green;
        uint8_t blue2 = entry->blue;
        red2 = scale8_LEAVING_R1_DIRTY(red2, offset);
        green2 = scale8_LEAVING_R1_DIRTY(green2, offset);
        blue2 = scale8_LEAVING_R1_DIRTY(blue2, offset);
        cleanup_R1();

        // These sums can't overflow, so no qadd8 needed.
        red1 += red2;
        green1 += green2;
        blue1 += blue2;
    }
    if (brightness != 255)
    {
        nscale8x3_video(red1, green1, blue1, brightness);
    }
    return CRGB(red1, green1, blue1);
}

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
        timer.setPeriod(MAX_MILLIS - map(kaleidoscopeSpeed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        if (++effect > 5)
            effect = 0;
    }

    adjustBrightness();
    adjustSpeed();
    leds_dirty = true;
}

#endif
