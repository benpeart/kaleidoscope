#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

// https://github.com/FastLED/FastLED
//#define FASTLED_ESP32_I2S true     // causes white flashes across several strips at the same time
//#define FASTLED_RMT_MAX_CHANNELS 4 // ESP32 support is provided using the RMT peripheral device, use 4 channels (one per strip)
//#define FASTLED_ESP32_FLASH_LOCK 1 // TODO: hack to enable OTA that doesn't work
#include <FastLED.h>

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
extern boolean leds_dirty;

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156
extern CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

//
// NUM_LEDS is the number of LEDs in the viewport that will be reflected and mirrored by drawPixel.
//
#define NUM_LEDS num_leds
extern uint8_t num_leds;
void drawPixel(CRGB *leds, int index, CRGB color);

void fill_kaleidoscope_rainbow(CRGB *leds, uint8_t initialhue, uint8_t deltahue);
void fill_kaleidoscope_solid(CRGB *leds, const struct CRGB &color);
void fill_kaleidoscope_gradient_RGB(CRGB *leds, uint16_t startpos, CRGB startcolor,
                                    uint16_t endpos, CRGB endcolor);

void mode_kaleidoscope();
void mode_kaleidoscope_select_speed_brightness();
//void mode_kaleidoscope_select_disks();
void mode_kaleidoscope_select_reflection_style();

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test();
#endif

#endif // KALEIDOSCOPE_H
