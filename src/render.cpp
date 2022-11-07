#include "main.h"
#include "render.h"
#include "debug.h"

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
boolean leds_dirty = true;

// led array which will be displayed
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// Create a lookup table where the index is the number of the triangle and the result is
// three pairs of {strip, index} that can be passed to MirroredSetPixelColor so they are
// mirrored to the other 3 axis. This way we only have to store 3 LED/offset pairs.
struct Pixel
{
    uint8_t strip;
    uint8_t index;
};

struct Strips
{
    Pixel strips[3];
};

// This lookup table maps from a triangular 'viewport' to the
// proper LED strip and pixel offset to provide a 3 way reflection.
// MirroredSetPixelColor can then be used to mirror that to the
// other 1/2 of the hexagon for a complete 6 way kaleidoscope.
#define DRAWPIXEL6_INDEX 100 // (VIEWPORT_HEIGHT * (TRIANGLE_COLUMNS + 1) / 2)
static const PROGMEM Strips drawPixel6LookupTable[DRAWPIXEL6_INDEX] =
    {
        {{{1, 9}, {1, 29}, {1, 10}}},
        {{{1, 47}, {1, 67}, {1, 11}}},
        {{{1, 30}, {1, 48}, {1, 28}}},
        {{{1, 8}, {1, 66}, {1, 49}}},
        {{{1, 83}, {1, 103}, {1, 12}}},
        {{{1, 68}, {1, 84}, {1, 27}}},
        {{{1, 46}, {1, 102}, {1, 50}}},
        {{{1, 31}, {1, 85}, {1, 65}}},
        {{{1, 7}, {1, 101}, {1, 86}}},
        {{{1, 117}, {1, 137}, {1, 13}}},
        {{{1, 104}, {1, 118}, {1, 26}}},
        {{{1, 82}, {1, 136}, {1, 51}}},
        {{{1, 69}, {1, 119}, {1, 64}}},
        {{{1, 45}, {1, 135}, {1, 87}}},
        {{{1, 32}, {1, 120}, {1, 100}}},
        {{{1, 6}, {1, 134}, {1, 121}}},
        {{{0, 5}, {0, 25}, {1, 14}}},
        {{{1, 138}, {0, 6}, {1, 25}}},
        {{{1, 116}, {0, 24}, {1, 52}}},
        {{{1, 105}, {0, 7}, {1, 63}}},
        {{{1, 81}, {0, 23}, {1, 88}}},
        {{{1, 70}, {0, 8}, {1, 99}}},
        {{{1, 44}, {0, 22}, {1, 122}}},
        {{{1, 33}, {0, 9}, {1, 133}}},
        {{{1, 5}, {0, 21}, {0, 10}}},
        {{{0, 35}, {0, 55}, {1, 15}}},
        {{{0, 26}, {0, 36}, {1, 24}}},
        {{{0, 4}, {0, 54}, {1, 53}}},
        {{{1, 139}, {0, 37}, {1, 62}}},
        {{{1, 115}, {0, 53}, {1, 89}}},
        {{{1, 106}, {0, 38}, {1, 98}}},
        {{{1, 80}, {0, 52}, {1, 123}}},
        {{{1, 71}, {0, 39}, {1, 132}}},
        {{{1, 43}, {0, 51}, {0, 11}}},
        {{{1, 34}, {0, 40}, {0, 20}}},
        {{{1, 4}, {0, 50}, {0, 41}}},
        {{{0, 63}, {0, 83}, {1, 16}}},
        {{{0, 56}, {0, 64}, {1, 23}}},
        {{{0, 34}, {0, 82}, {1, 54}}},
        {{{0, 27}, {0, 65}, {1, 61}}},
        {{{0, 3}, {0, 81}, {1, 90}}},
        {{{1, 140}, {0, 66}, {1, 97}}},
        {{{1, 114}, {0, 80}, {1, 124}}},
        {{{1, 107}, {0, 67}, {1, 131}}},
        {{{1, 79}, {0, 79}, {0, 12}}},
        {{{1, 72}, {0, 68}, {0, 19}}},
        {{{1, 42}, {0, 78}, {0, 42}}},
        {{{1, 35}, {0, 69}, {0, 49}}},
        {{{1, 3}, {0, 77}, {0, 70}}},
        {{{0, 89}, {0, 109}, {1, 17}}},
        {{{0, 84}, {0, 90}, {1, 22}}},
        {{{0, 62}, {0, 108}, {1, 55}}},
        {{{0, 57}, {0, 91}, {1, 60}}},
        {{{0, 33}, {0, 107}, {1, 91}}},
        {{{0, 28}, {0, 92}, {1, 96}}},
        {{{0, 2}, {0, 106}, {1, 125}}},
        {{{1, 141}, {0, 93}, {1, 130}}},
        {{{1, 113}, {0, 105}, {0, 13}}},
        {{{1, 108}, {0, 94}, {0, 18}}},
        {{{1, 78}, {0, 104}, {0, 43}}},
        {{{1, 73}, {0, 95}, {0, 48}}},
        {{{1, 41}, {0, 103}, {0, 71}}},
        {{{1, 36}, {0, 96}, {0, 76}}},
        {{{1, 2}, {0, 102}, {0, 97}}},
        {{{0, 113}, {0, 133}, {1, 18}}},
        {{{0, 110}, {0, 114}, {1, 21}}},
        {{{0, 88}, {0, 132}, {1, 56}}},
        {{{0, 85}, {0, 115}, {1, 59}}},
        {{{0, 61}, {0, 131}, {1, 92}}},
        {{{0, 58}, {0, 116}, {1, 95}}},
        {{{0, 32}, {0, 130}, {1, 126}}},
        {{{0, 29}, {0, 117}, {1, 129}}},
        {{{0, 1}, {0, 129}, {0, 14}}},
        {{{1, 142}, {0, 118}, {0, 17}}},
        {{{1, 112}, {0, 128}, {0, 44}}},
        {{{1, 109}, {0, 119}, {0, 47}}},
        {{{1, 77}, {0, 127}, {0, 72}}},
        {{{1, 74}, {0, 120}, {0, 75}}},
        {{{1, 40}, {0, 126}, {0, 98}}},
        {{{1, 37}, {0, 121}, {0, 101}}},
        {{{1, 1}, {0, 125}, {0, 122}}},
        {{{0, 135}, {0, 155}, {1, 19}}},
        {{{0, 134}, {0, 136}, {1, 20}}},
        {{{0, 112}, {0, 154}, {1, 57}}},
        {{{0, 111}, {0, 137}, {1, 58}}},
        {{{0, 87}, {0, 153}, {1, 93}}},
        {{{0, 86}, {0, 138}, {1, 94}}},
        {{{0, 60}, {0, 152}, {1, 127}}},
        {{{0, 59}, {0, 139}, {1, 128}}},
        {{{0, 31}, {0, 151}, {0, 15}}},
        {{{0, 30}, {0, 140}, {0, 16}}},
        {{{0, 0}, {0, 150}, {0, 45}}},
        {{{1, 143}, {0, 141}, {0, 46}}},
        {{{1, 111}, {0, 149}, {0, 73}}},
        {{{1, 110}, {0, 142}, {0, 74}}},
        {{{1, 76}, {0, 148}, {0, 99}}},
        {{{1, 75}, {0, 143}, {0, 100}}},
        {{{1, 39}, {0, 147}, {0, 123}}},
        {{{1, 38}, {0, 144}, {0, 124}}},
        {{{1, 0}, {0, 146}, {0, 145}}}};

// This 2 way lookup table can be used in combination with the 6 way
// mapping to simulate Kaleidoscopes made with two mirrors that
// produces one central image in the middle of the viewing field.
// That central image is a dodecagon (12 sided polygon).
#define DRAWPIXEL12_INDEX 55
#define DRAWPIXEL12_COLUMNS 2
static const PROGMEM uint8_t drawPixel12LookupTable[DRAWPIXEL12_INDEX][DRAWPIXEL12_COLUMNS] =
    {
        {0, 0},
        {1, 3},
        {2, 2},
        {4, 8},
        {5, 7},
        {6, 6},
        {9, 15},
        {10, 14},
        {11, 13},
        {12, 12},
        {16, 24},
        {17, 23},
        {18, 22},
        {19, 21},
        {20, 20},
        {25, 35},
        {26, 34},
        {27, 33},
        {28, 32},
        {29, 31},
        {30, 30},
        {36, 48},
        {37, 47},
        {38, 46},
        {39, 45},
        {40, 44},
        {41, 43},
        {42, 42},
        {49, 63},
        {50, 62},
        {51, 61},
        {52, 60},
        {53, 59},
        {54, 58},
        {55, 57},
        {56, 56},
        {64, 80},
        {65, 79},
        {66, 78},
        {67, 77},
        {68, 76},
        {69, 75},
        {70, 74},
        {71, 73},
        {72, 72},
        {81, 99},
        {82, 98},
        {83, 97},
        {84, 96},
        {85, 95},
        {86, 94},
        {87, 93},
        {88, 92},
        {89, 91},
        {90, 90}};

// This 4 way lookup table can be used in combination with the 6 way
// mapping to simulate Kaleidoscopes made with three mirrors where the
// images are reflected throughout the entire field of view producing
// honeycomb-like patterns (24 sided polygon).
#define DRAWPIXEL24_INDEX 25
#define DRAWPIXEL24_COLUMNS 4
static const PROGMEM uint8_t drawPixel24LookupTable[DRAWPIXEL24_INDEX][DRAWPIXEL24_COLUMNS] =
    {
        {0, 90, 89, 91},
        {1, 71, 70, 93},
        {2, 72, 88, 92},
        {3, 73, 87, 74},
        {4, 54, 53, 95},
        {5, 55, 69, 94},
        {6, 56, 68, 76},
        {7, 57, 86, 75},
        {8, 58, 85, 59},
        {9, 39, 38, 97},
        {10, 40, 52, 96},
        {11, 41, 51, 78},
        {12, 42, 67, 77},
        {13, 43, 66, 61},
        {14, 44, 84, 60},
        {15, 45, 83, 46},
        {16, 26, 25, 99},
        {17, 27, 37, 98},
        {18, 28, 36, 80},
        {19, 29, 50, 79},
        {20, 30, 49, 63},
        {21, 31, 65, 62},
        {22, 32, 64, 48},
        {23, 33, 82, 47},
        {24, 34, 81, 35}};

void MirroredSetPixelColor(CRGB *leds, int strip, int index, CRGB rgb)
{
    if (index < 0 || index >= NUM_LEDS_PER_STRIP)
    {
        DB_PRINT(F("MirroredSetPixelColor: requested index ("));
        DB_PRINT(index);
        DB_PRINTLN(F(") exceeds number of LEDs in a strip."));
        return;
    }

    switch (strip)
    {
    case 0:
        leds[0 * NUM_LEDS_PER_STRIP + index] = rgb;
        leds[3 * NUM_LEDS_PER_STRIP + index] = rgb;
        leds_dirty = true;
        break;

    case 1:
        leds[1 * NUM_LEDS_PER_STRIP + index] = rgb;
        leds[2 * NUM_LEDS_PER_STRIP + index] = rgb;
        leds_dirty = true;
        break;

    default:
        DB_PRINT(F("MirroredSetPixelColor: invalid strip ID = "));
        DB_PRINTLN(strip);
        break;
    }
}

// draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
void drawPixel6(CRGB *leds, int index, CRGB color)
{
    if (index < 0 || index >= DRAWPIXEL6_INDEX)
    {
        DB_PRINT("drawPixel6 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (uint8_t x = 0; x < 3; x++)
        MirroredSetPixelColor(leds, pgm_read_byte_near(&drawPixel6LookupTable[index].strips[x].strip),
                              pgm_read_byte_near(&drawPixel6LookupTable[index].strips[x].index), color);
}

// draw a pixel mirrored and rotated 12 times to emulate a kaleidoscope
void drawPixel12(CRGB *leds, int index, CRGB color)
{
    if (index < 0 || index >= DRAWPIXEL12_INDEX)
    {
        DB_PRINT("drawPixel12 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (int column = 0; column < DRAWPIXEL12_COLUMNS; column++)
    {
        drawPixel6(leds, pgm_read_byte_near(&drawPixel12LookupTable[index][column]), color);
    }
}

// draw a pixel mirrored and rotated 24 times to emulate a kaleidoscope
void drawPixel24(CRGB *leds, int index, CRGB color)
{
    if (index < 0 || index >= DRAWPIXEL24_INDEX)
    {
        DB_PRINT("drawPixel24 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (int column = 0; column < DRAWPIXEL24_COLUMNS; column++)
    {
        drawPixel6(leds, pgm_read_byte_near(&drawPixel24LookupTable[index][column]), color);
    }
}

// Provide functions to draw the pixels mirrored and replicated to match
// the given kaleidoscope style. Update the number of corresponding
// leds in the 'viewport' so that the ported modes work as expected.
void (*drawPixelFunc[])(CRGB *leds, int index, CRGB color){
    drawPixel6,
    drawPixel12,
    drawPixel24};
//#define N_DRAW_STYLES (sizeof(drawPixelFunc) / sizeof(drawPixelFunc[0]))
uint8_t draw_style = 0; // Index of current draw mode in table
uint8_t num_leds = DRAWPIXEL6_INDEX;

const PROGMEM char drawStyles[N_DRAW_STYLES][16] =
    {
        "Six way",
        "Twelve way",
        "Twenty four way"};

int set_draw_style(int new_draw_style)
{
    if (draw_style != new_draw_style)
    {
        draw_style = new_draw_style;
        DB_PRINTLN(drawStyles[draw_style]);
        switch (draw_style)
        {
        case 0:
            num_leds = DRAWPIXEL6_INDEX;
            break;

        case 1:
            num_leds = DRAWPIXEL12_INDEX;
            break;

        case 2:
            num_leds = DRAWPIXEL24_INDEX;
            break;
        }
        leds_dirty = true;
    }

    return draw_style;
}

// simple wrapper function to abstract out the fact that we have different draw functions
void drawPixel(CRGB *leds, int index, CRGB color)
{
    drawPixelFunc[draw_style](leds, index, color);
}

void fill_kaleidoscope_rainbow(CRGB *leds, uint8_t initialhue, uint8_t deltahue)
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        drawPixel(leds, i, hsv);
        hsv.hue += deltahue;
    }
}

void fill_kaleidoscope_solid(CRGB *leds, const struct CRGB &color)
{
    leds_dirty = true;
    ::fill_solid(leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, color);
}

void fill_kaleidoscope_gradient_RGB(CRGB *leds, uint16_t startpos, CRGB startcolor,
                                    uint16_t endpos, CRGB endcolor)
{
    // if the points are in the wrong order, straighten them
    if (endpos < startpos)
    {
        uint16_t t = endpos;
        CRGB tc = endcolor;
        endcolor = startcolor;
        endpos = startpos;
        startpos = t;
        startcolor = tc;
    }

    saccum87 rdistance87;
    saccum87 gdistance87;
    saccum87 bdistance87;

    rdistance87 = (endcolor.r - startcolor.r) << 7;
    gdistance87 = (endcolor.g - startcolor.g) << 7;
    bdistance87 = (endcolor.b - startcolor.b) << 7;

    uint16_t pixeldistance = endpos - startpos;
    int16_t divisor = pixeldistance ? pixeldistance : 1;

    saccum87 rdelta87 = rdistance87 / divisor;
    saccum87 gdelta87 = gdistance87 / divisor;
    saccum87 bdelta87 = bdistance87 / divisor;

    rdelta87 *= 2;
    gdelta87 *= 2;
    bdelta87 *= 2;

    accum88 r88 = startcolor.r << 8;
    accum88 g88 = startcolor.g << 8;
    accum88 b88 = startcolor.b << 8;
    for (uint16_t i = startpos; i <= endpos; ++i)
    {
        drawPixel(leds, i, CRGB(r88 >> 8, g88 >> 8, b88 >> 8));
        r88 += rdelta87;
        g88 += gdelta87;
        b88 += bdelta87;
    }
}


//
// I have implemented two different X,Y mapping modes. The first pairs adjacent strips of LEDs into a single x coordinate
// as they are paired in the triangle columns. The second (WIDERTHANTALLER) maps each strip to a separate x coordinate.
//
#ifndef WIDERTHANTALLER
#define STRIP_0_NUM_COLS 6
#define STRIP_1_NUM_COLS 4
#define STRIP_2_NUM_COLS 4
#define STRIP_3_NUM_COLS 6

// mode_kaleidoscope_matrix spawns new falling code in the top row and then moves them down screen.
// Since most of our top row is missing, map it to the top row available for the given column.
const PROGMEM uint8_t col_to_top_row[NUM_COLS] = {
  9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#else
#define STRIP_0_NUM_COLS 12
#define STRIP_1_NUM_COLS 8
#define STRIP_2_NUM_COLS 8
#define STRIP_3_NUM_COLS 12

// Since most of our top row is missing, provide a map to the top row available for the given column.
const PROGMEM uint8_t col_to_top_row[NUM_COLS] = {
  5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5};
#endif

uint16_t XY(uint16_t x, uint16_t y)
{
  // any out of bounds address maps to the first hidden pixel
  if ((x < 0) || (x >= NUM_COLS) || (y < 0) || (y >= NUM_ROWS))
    return OUTOFBOUNDS;

#ifndef WIDERTHANTALLER
  const uint8_t XYTable[NUM_ROWS * NUM_COLS / 2] = {
      //
      // This array helps translate from a (x,y) coordinate to the correct offset in the leds[] array.
      // To get the best visual results, we are combining columns as the triangles are interleaved
      // vertically. This gives us an effective resolution of (20 x 39) pixels with missing corners
      // where the hexaqgon shape doesn't fill the square. Any (x, y) coordinates that aren't valid
      // LED positions, return a value that is within the leds[] array but doesn't have a corresponding
      // physical LED to make error handling simpler.
      //
      255, 255, 255, 255, 255, 255, 255, 255, 255,  19,
      255, 255, 255, 255, 255, 255, 255, 255,  57,  20,
      255, 255, 255, 255, 255, 255, 255,  93,  58,  18,
      255, 255, 255, 255, 255, 255, 127,  94,  56,  21,
      255, 255, 255, 255, 255,  15, 128,  92,  59,  17,
      255, 255, 255, 255,  45,  16, 126,  95,  55,  22,
      255, 255, 255,  73,  46,  14, 129,  91,  60,  16,
      255, 255,  99,  74,  44,  17, 125,  96,  54,  23,
      255, 123, 100,  72,  47,  13, 130,  90,  61,  15,
      145, 124,  98,  75,  43,  18, 124,  97,  53,  24,
      146, 122, 101,  71,  48,  12, 131,  89,  62,  14,
      144, 125,  97,  76,  42,  19, 123,  98,  52,  25,
      147, 121, 102,  70,  49,  11, 132,  88,  63,  13,
      143, 126,  96,  77,  41,  20, 122,  99,  51,  26,
      148, 120, 103,  69,  50,  10, 133,  87,  64,  12,
      142, 127,  95,  78,  40,  21, 121, 100,  50,  27,
      149, 119, 104,  68,  51,   9, 134,  86,  65,  11,
      141, 128,  94,  79,  39,  22, 120, 101,  49,  28,
      150, 118, 105,  67,  52,   8, 135,  85,  66,  10,
      140, 129,  93,  80,  38,  23, 119, 102,  48,  29,
      151, 117, 106,  66,  53,   7, 136,  84,  67,   9,
      139, 130,  92,  81,  37,  24, 118, 103,  47,  30,
      152, 116, 107,  65,  54,   6, 137,  83,  68,   8,
      138, 131,  91,  82,  36,  25, 117, 104,  46,  31,
      153, 115, 108,  64,  55,   5, 138,  82,  69,   7,
      137, 132,  90,  83,  35,  26, 116, 105,  45,  32,
      154, 114, 109,  63,  56,   4, 139,  81,  70,   6,
      136, 133,  89,  84,  34,  27, 115, 106,  44,  33,
      155, 113, 110,  62,  57,   3, 140,  80,  71,   5,
      135, 134,  88,  85,  33,  28, 114, 107,  43,  34,
      255, 112, 111,  61,  58,   2, 141,  79,  72,   4,
      255, 255,  87,  86,  32,  29, 113, 108,  42,  35,
      255, 255, 255,  60,  59,   1, 142,  78,  73,   3,
      255, 255, 255, 255,  31,  30, 112, 109,  41,  36,
      255, 255, 255, 255, 255,   0, 143,  77,  74,   2,
      255, 255, 255, 255, 255, 255, 111, 110,  40,  37,
      255, 255, 255, 255, 255, 255, 255,  76,  75,   1,
      255, 255, 255, 255, 255, 255, 255, 255,  39,  38,
      255, 255, 255, 255, 255, 255, 255, 255, 255,   0};
#else
  //
  // This array helps translate from a (x,y) coordinate to the correct offset in the leds[] array.
  // This gives us an effective resolution of (40 x 20) pixels with missing corners
  // where the hexaqgon shape doesn't fill the square. Any (x, y) coordinates that aren't valid
  // LED positions, return a value that is within the leds[] array but doesn't have a corresponding
  // physical LED to make error handling simpler.
  //
  const uint8_t XYTable[NUM_ROWS * NUM_COLS / 2] = {
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  19,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  93,  58,  57,  20,  18,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  15, 128, 127,  94,  92,  59,  56,  21,  17,
      255, 255, 255, 255, 255, 255, 255,  73,  46,  45,  16,  14, 129, 126,  95,  91,  60,  55,  22,  16,
      255, 255, 255, 123, 100,  99,  74,  72,  47,  44,  17,  13, 130, 125,  96,  90,  61,  54,  23,  15,
      146, 145, 124, 122, 101,  98,  75,  71,  48,  43,  18,  12, 131, 124,  97,  89,  62,  53,  24,  14,
      147, 144, 125, 121, 102,  97,  76,  70,  49,  42,  19,  11, 132, 123,  98,  88,  63,  52,  25,  13,
      148, 143, 126, 120, 103,  96,  77,  69,  50,  41,  20,  10, 133, 122,  99,  87,  64,  51,  26,  12,
      149, 142, 127, 119, 104,  95,  78,  68,  51,  40,  21,   9, 134, 121, 100,  86,  65,  50,  27,  11,
      150, 141, 128, 118, 105,  94,  79,  67,  52,  39,  22,   8, 135, 120, 101,  85,  66,  49,  28,  10,
      151, 140, 129, 117, 106,  93,  80,  66,  53,  38,  23,   7, 136, 119, 102,  84,  67,  48,  29,   9,
      152, 139, 130, 116, 107,  92,  81,  65,  54,  37,  24,   6, 137, 118, 103,  83,  68,  47,  30,   8,
      153, 138, 131, 115, 108,  91,  82,  64,  55,  36,  25,   5, 138, 117, 104,  82,  69,  46,  31,   7,
      154, 137, 132, 114, 109,  90,  83,  63,  56,  35,  26,   4, 139, 116, 105,  81,  70,  45,  32,   6,
      155, 136, 133, 113, 110,  89,  84,  62,  57,  34,  27,   3, 140, 115, 106,  80,  71,  44,  33,   5,
      255, 135, 134, 112, 111,  88,  85,  61,  58,  33,  28,   2, 141, 114, 107,  79,  72,  43,  34,   4,
      255, 255, 255, 255, 255,  87,  86,  60,  59,  32,  29,   1, 142, 113, 108,  78,  73,  42,  35,   3,
      255, 255, 255, 255, 255, 255, 255, 255, 255,  31,  30,   0, 143, 112, 109,  77,  74,  41,  36,   2,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 111, 110,  76,  75,  40,  37,   1,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  39,  38,   0};
#endif

  // Compute the index into the lookup table taking into account the fact that the table
  // only contains 1/2 the columns to save space. The right have are just mirrored from
  // the left half.
  uint16_t tableIndex = (y * NUM_COLS / 2);
  if (x < NUM_COLS / 2)
  {
    tableIndex += x;
  }
  else
  {
    tableIndex += NUM_COLS - x - 1;
  }
  uint16_t stripOffset = XYTable[tableIndex];
  uint8_t strip = 0;

  // if we are returning an out of bounds value for an individual strip, return the first hidden LED instead
  // to make error handling easier.
  if (stripOffset >= NUM_LEDS_PER_STRIP)
    return OUTOFBOUNDS;

  if (x < STRIP_0_NUM_COLS)
  {
    strip = 0;
  }
  else if (x < STRIP_0_NUM_COLS + STRIP_1_NUM_COLS)
  {
    strip = 1;
  }
  else if (x < STRIP_0_NUM_COLS + STRIP_1_NUM_COLS + STRIP_2_NUM_COLS)
  {
    strip = 2;
  }
  else if (x < STRIP_0_NUM_COLS + STRIP_1_NUM_COLS + STRIP_2_NUM_COLS + STRIP_3_NUM_COLS)
  {
    strip = 3;
  }

  return strip * NUM_LEDS_PER_STRIP + stripOffset;
}

#ifdef DEBUG
// loop through all pixels via x,y coordinates making sure they all get mapped correctly
void mode_xy_test()
{
  static int x = -1, y = 0;
  int index;

  EVERY_N_MILLISECONDS(75)
  {
    // erase the last pixel
    index = XY(x, y);
    leds[index] = CRGB::Black; // off

    // move to the next pixel
    if (++x >= NUM_COLS)
    {
      x = 0;
      if (++y >= NUM_ROWS)
        y = 0;
    }
    index = XY(x, y);

    DB_PRINT("x = ");
    DB_PRINT(x);
    DB_PRINT(" y = ");
    DB_PRINT(y);
    DB_PRINT(" index = ");
    DB_PRINTLN(index);

    // light up the next pixel
    leds[index] = CRGB::Red;

    leds_dirty = true;
  }
}
#endif
