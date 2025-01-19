#ifndef RENDER_H
#define RENDER_H

// https://github.com/FastLED/FastLED
// #define FASTLED_ESP32_I2S true // causes white flashes across several strips at the same time
#define FASTLED_ESP32_FLASH_LOCK 1   // force flash operations to wait until the show() is done. (doesn't fix the hang when updating the firmware while displaying the kaleidoscope)
// #define FASTLED_ALL_PINS_HARDWARE_SPI
#include <FastLED.h>

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
extern boolean leds_dirty;

#define N_DRAW_STYLES 3
extern const PROGMEM char drawStylesLUT[N_DRAW_STYLES][16];
int setDrawStyle(int drawStyle);

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156
extern CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

//
// NUM_LEDS is the number of LEDs in the viewport that will be reflected and mirrored by drawPixel according to the current drawStyle.
//
#define NUM_LEDS num_leds
extern uint8_t num_leds;
void drawPixel(CRGB *leds, int index, CRGB color);

void fill_kaleidoscope_rainbow(CRGB *leds, uint8_t initialhue, uint8_t deltahue);
void fill_kaleidoscope_solid(CRGB *leds, const struct CRGB &color);
void fill_kaleidoscope_gradient_RGB(CRGB *leds, uint16_t startpos, CRGB startcolor, uint16_t endpos, CRGB endcolor);

// The width and height of the XY coordinate system. The corners outside the hexagon
// are 'missing' so can't display any values assigned to them.
#ifndef WIDERTHANTALLER
#define NUM_COLS 20
#define NUM_ROWS 39
#else
#define NUM_COLS 40
#define NUM_ROWS 20
#endif
#define WIDTH NUM_COLS
#define HEIGHT NUM_ROWS
#define OUTOFBOUNDS (156 * 2 - 1) // NUM_LEDS_PER_STRIP * 2 - 1

extern const PROGMEM uint8_t col_to_top_row[NUM_COLS];

// function to map a x, y coordinate to the index into the led array
uint16_t XY(uint16_t x, uint16_t y);

#ifdef DEBUG
// loop through all pixels via x,y coordinates making sure they all get mapped correctly
void mode_xy_test();
#endif

#endif // RENDER_H
