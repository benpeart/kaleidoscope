#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156

#define TRIANGLE_ROWS 10                                            // the height of the 'viewport' triangle
#define TRIANGLE_COLUMNS 19                                         // the width of the base of the 'viewport' triange
#define TRIANGLE_COUNT (TRIANGLE_ROWS * (TRIANGLE_COLUMNS + 1) / 2) // the number of pixels in the 'viewport' triangle
#define NUM_LEDS TRIANGLE_COUNT

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
extern boolean leds_dirty;

extern CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

class Kaleidoscope
{
public:
    void setup();

    void drawTwoMirrorFrame(CRGB *leds, const struct CRGBPalette16 &pal,
                            const uint8_t (*disk_1)[TRIANGLE_ROWS], const uint8_t offset_1, const uint8_t columns_1,
                            const uint8_t (*disk_2)[TRIANGLE_ROWS], const uint8_t offset_2, const uint8_t columns_2);

    // draw the requested frame as a kaleidoscope
    void drawPaletteFrame(CRGB *leds, const struct CRGBPalette16 &pal,
                          const uint8_t (*disk_1)[TRIANGLE_ROWS], const uint8_t offset_1, const uint8_t columns_1,
                          const uint8_t (*disk_2)[TRIANGLE_ROWS], const uint8_t offset_2, const uint8_t columns_2);

    // update the position of the strips and draw the kaleidoscope
    void drawFrame(CRGB *leds);

    // draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
    void drawPixel6(CRGB *leds, int index, CRGB c);

    // draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
    void drawPixel12(CRGB *leds, int index, CRGB c);

    // draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
    void drawPixel24(CRGB *leds, int index, CRGB c);

    void fill_rainbow(CRGB *leds, uint8_t initialhue, uint8_t deltahue);

    void fill_solid(CRGB *leds, const struct CRGB &color);

    void fill_gradient_RGB(CRGB *leds, uint16_t startpos, CRGB startcolor,
                           uint16_t endpos, CRGB endcolor);

private:
    // pointer to two dimensional arrays of color values for kaleidoscope effect
    // https://stackoverflow.com/questions/1052818/create-a-pointer-to-two-dimensional-array
    const uint32_t (*rgb_strip_1)[TRIANGLE_ROWS];
    uint8_t strip_1_columns;
    const uint32_t (*rgb_strip_2)[TRIANGLE_ROWS];
    uint8_t strip_2_columns;

    // offset into strips, used to provide movement for the kaleidoscope
    int offset_1;
    int offset_2;

    void MirroredSetPixelColor(CRGB *leds, int strip, int index, CRGB rgb);
};

extern Kaleidoscope kaleidoscope;

void mode_kaleidoscope_palette();
void mode_kaleidoscope_screensaver();
void mode_kaleidoscope_interactive();
void mode_kaleidoscope_select_disks();

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test();
#endif

#endif // KALEIDOSCOPE_H
