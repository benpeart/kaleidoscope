#include "main.h"
#include "Kaleidoscope.h"
#include "XYIndex.h"
#include "XYfire.h"

#ifdef DEMO

DEFINE_GRADIENT_PALETTE(firePal_gp){
    0, 0, 0, 0,          //black
    100, 100, 0, 0,      //dark red
    128, 255, 0, 0,      //red
    220, 255, 255, 0,    //yellow
    255, 255, 255, 255}; //white

static uint8_t firePixels[NUM_STRIPS * NUM_LEDS_PER_STRIP];
static CRGBPalette16 _currentPalette = firePal_gp;
static boolean initialized = false;

void spreadFire(uint16_t src)
{
    if (firePixels[src] == 0)
    {
        firePixels[src - kMatrixWidth] = 0;
    }
    else
    {
        // Commented lines moves fire sideways as well as up, but doesn't look good on low res matrix:
        // int16_t dst = src - rand + 1;
        // firePixels[dst - kMatrixWidth] = firePixels[src] - random8(1);
        firePixels[src - kMatrixWidth] = firePixels[src] - random8(3);
    }
}

void doFire()
{
    for (uint16_t x = 0; x < kMatrixWidth; x++)
    {
        for (uint16_t y = 1; y < kMatrixHeight; y++)
        {
            spreadFire(y * kMatrixWidth + x);
        }
    }
}

void setBottomRow(uint16_t col)
{
    for (uint16_t i = 0; i < kMatrixWidth; i++)
    {
        firePixels[(kMatrixHeight - 1) * kMatrixWidth + i] = col;
    }
}

// https://github.com/s-marley/LEDMask/blob/master/MaskUpdate/Fire.h
void mode_xy_fire()
{
    // Set bottom row to highest index in palette (white)
    if (!initialized)
    {
        setBottomRow(kMatrixHeight);
        initialized = true;
    }

    doFire();
    for (int y = 0; y < kMatrixHeight; y++)
    {
        for (int x = 0; x < kMatrixWidth; x++)
        {
            int index = firePixels[kMatrixWidth * y + x];
            // Index goes from 0 -> kMatrixHeight, palette goes from 0 -> 255 so need to scale it
            uint8_t indexScale = 255 / kMatrixHeight;
            leds[XYToIndex(x, y)] = ColorFromPalette(_currentPalette, constrain(index * indexScale, 0, 255), 255, LINEARBLEND);
        }
    }

    leds_dirty = true;
}

#endif
