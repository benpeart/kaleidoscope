#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

// https://github.com/FastLED/FastLED
#define FASTLED_RMT_MAX_CHANNELS 2 // TODO: why 2 channels instead of 4 (one per strip?)
//#define FASTLED_ESP32_FLASH_LOCK 1 // TODO: hack to enable OTA that doesn't work
#include <FastLED.h>

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
extern boolean leds_dirty;

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156
extern CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

//
// Change the NUM_LEDS and drawPixel #define below to test the different types of Kaleidoscopes
//
#define NUM_LEDS num_leds
extern uint8_t num_leds;
extern void (*drawPixel)(CRGB *leds, int index, CRGB c);

void fill_kaleidoscope_rainbow(CRGB *leds, uint8_t initialhue, uint8_t deltahue);
void fill_kaleidoscope_solid(CRGB *leds, const struct CRGB &color);
void fill_kaleidoscope_gradient_RGB(CRGB *leds, uint16_t startpos, CRGB startcolor,
                                    uint16_t endpos, CRGB endcolor);

void mode_kaleidoscope_screensaver();
void mode_kaleidoscope_interactive();
void mode_kaleidoscope_select_disks();

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test();
#endif

#endif // KALEIDOSCOPE_H
