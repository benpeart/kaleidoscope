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
        firePixels[src - NUM_COLS] = 0;
    }
    else
    {
        // Commented lines moves fire sideways as well as up, but doesn't look good on low res matrix:
        // int16_t dst = src - rand + 1;
        // firePixels[dst - NUM_COLS] = firePixels[src] - random8(1);
        firePixels[src - NUM_COLS] = firePixels[src] - random8(3);
    }
}

void doFire()
{
    for (uint16_t x = 0; x < NUM_COLS; x++)
    {
        for (uint16_t y = 1; y < NUM_ROWS; y++)
        {
            spreadFire(y * NUM_COLS + x);
        }
    }
}

void setBottomRow(uint16_t col)
{
    for (uint16_t i = 0; i < NUM_COLS; i++)
    {
        firePixels[(NUM_ROWS - 1) * NUM_COLS + i] = col;
    }
}

// https://github.com/s-marley/LEDMask/blob/master/MaskUpdate/Fire.h
void mode_xy_fire()
{
    // Set bottom row to highest index in palette (white)
    if (!initialized)
    {
        setBottomRow(NUM_ROWS);
        initialized = true;
    }

    doFire();
    for (int y = 0; y < NUM_ROWS; y++)
    {
        for (int x = 0; x < NUM_COLS; x++)
        {
            int index = firePixels[NUM_COLS * y + x];
            // Index goes from 0 -> NUM_ROWS, palette goes from 0 -> 255 so need to scale it
            uint8_t indexScale = 255 / NUM_ROWS;
            leds[XYToIndex(x, y)] = ColorFromPalette(_currentPalette, constrain(index * indexScale, 0, 255), 255, LINEARBLEND);
        }
    }

    leds_dirty = true;
}

#endif
