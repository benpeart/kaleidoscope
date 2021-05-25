#include "main.h"
#include "Kaleidoscope.h"

#define VIEWPORT_HEIGHT 10  // the height of the 'viewport' triangle
#define TRIANGLE_COLUMNS 19 // the width of the base of the 'viewport' triange

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
boolean leds_dirty = true;

// we're going to blend between frames so need an array of LEDs for both frames
// then we'll blend them together into the 'output' leds array which will be displayed
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds2[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds3[NUM_STRIPS * NUM_LEDS_PER_STRIP];

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
        {{{1, 18}, {1, 19}, {1, 20}}},
        {{{1, 59}, {1, 58}, {1, 22}}},
        {{{1, 17}, {1, 57}, {1, 21}}},
        {{{1, 16}, {1, 56}, {1, 55}}},
        {{{1, 90}, {1, 91}, {1, 24}}},
        {{{1, 60}, {1, 92}, {1, 23}}},
        {{{1, 61}, {1, 93}, {1, 53}}},
        {{{1, 15}, {1, 94}, {1, 54}}},
        {{{1, 14}, {1, 95}, {1, 96}}},
        {{{1, 131}, {1, 130}, {1, 26}}},
        {{{1, 89}, {1, 129}, {1, 25}}},
        {{{1, 88}, {1, 128}, {1, 51}}},
        {{{1, 62}, {1, 127}, {1, 52}}},
        {{{1, 63}, {1, 126}, {1, 98}}},
        {{{1, 13}, {1, 125}, {1, 97}}},
        {{{1, 12}, {1, 124}, {1, 123}}},
        {{{0, 10}, {0, 11}, {1, 28}}},
        {{{1, 132}, {0, 12}, {1, 27}}},
        {{{1, 133}, {0, 13}, {1, 49}}},
        {{{1, 87}, {0, 14}, {1, 50}}},
        {{{1, 86}, {0, 15}, {1, 100}}},
        {{{1, 64}, {0, 16}, {1, 99}}},
        {{{1, 65}, {0, 17}, {1, 121}}},
        {{{1, 11}, {0, 18}, {1, 122}}},
        {{{1, 10}, {0, 19}, {0, 20}}},
        {{{0, 51}, {0, 50}, {1, 30}}},
        {{{0, 9}, {0, 49}, {1, 29}}},
        {{{0, 8}, {0, 48}, {1, 47}}},
        {{{1, 134}, {0, 47}, {1, 48}}},
        {{{1, 135}, {0, 46}, {1, 102}}},
        {{{1, 85}, {0, 45}, {1, 101}}},
        {{{1, 84}, {0, 44}, {1, 119}}},
        {{{1, 66}, {0, 43}, {1, 120}}},
        {{{1, 67}, {0, 42}, {0, 22}}},
        {{{1, 9}, {0, 41}, {0, 21}}},
        {{{1, 8}, {0, 40}, {0, 39}}},
        {{{0, 66}, {0, 67}, {1, 32}}},
        {{{0, 52}, {0, 68}, {1, 31}}},
        {{{0, 53}, {0, 69}, {1, 45}}},
        {{{0, 7}, {0, 70}, {1, 46}}},
        {{{0, 6}, {0, 71}, {1, 104}}},
        {{{1, 136}, {0, 72}, {1, 103}}},
        {{{1, 137}, {0, 73}, {1, 117}}},
        {{{1, 83}, {0, 74}, {1, 118}}},
        {{{1, 82}, {0, 75}, {0, 24}}},
        {{{1, 68}, {0, 76}, {0, 23}}},
        {{{1, 69}, {0, 77}, {0, 37}}},
        {{{1, 7}, {0, 78}, {0, 38}}},
        {{{1, 6}, {0, 79}, {0, 80}}},
        {{{0, 107}, {0, 106}, {1, 34}}},
        {{{0, 65}, {0, 105}, {1, 33}}},
        {{{0, 64}, {0, 104}, {1, 43}}},
        {{{0, 54}, {0, 103}, {1, 44}}},
        {{{0, 55}, {0, 102}, {1, 106}}},
        {{{0, 5}, {0, 101}, {1, 105}}},
        {{{0, 4}, {0, 100}, {1, 115}}},
        {{{1, 138}, {0, 99}, {1, 116}}},
        {{{1, 139}, {0, 98}, {0, 26}}},
        {{{1, 81}, {0, 97}, {0, 25}}},
        {{{1, 80}, {0, 96}, {0, 35}}},
        {{{1, 70}, {0, 95}, {0, 36}}},
        {{{1, 71}, {0, 94}, {0, 82}}},
        {{{1, 5}, {0, 93}, {0, 81}}},
        {{{1, 4}, {0, 92}, {0, 91}}},
        {{{0, 114}, {0, 115}, {1, 36}}},
        {{{0, 108}, {0, 116}, {1, 35}}},
        {{{0, 109}, {0, 117}, {1, 41}}},
        {{{0, 63}, {0, 118}, {1, 42}}},
        {{{0, 62}, {0, 119}, {1, 108}}},
        {{{0, 56}, {0, 120}, {1, 107}}},
        {{{0, 57}, {0, 121}, {1, 113}}},
        {{{0, 3}, {0, 122}, {1, 114}}},
        {{{0, 2}, {0, 123}, {0, 28}}},
        {{{1, 140}, {0, 124}, {0, 27}}},
        {{{1, 141}, {0, 125}, {0, 33}}},
        {{{1, 79}, {0, 126}, {0, 34}}},
        {{{1, 78}, {0, 127}, {0, 84}}},
        {{{1, 72}, {0, 128}, {0, 83}}},
        {{{1, 73}, {0, 129}, {0, 89}}},
        {{{1, 3}, {0, 130}, {0, 90}}},
        {{{1, 2}, {0, 131}, {0, 132}}},
        {{{0, 155}, {0, 154}, {1, 38}}},
        {{{0, 113}, {0, 153}, {1, 37}}},
        {{{0, 112}, {0, 152}, {1, 39}}},
        {{{0, 110}, {0, 151}, {1, 40}}},
        {{{0, 111}, {0, 150}, {1, 110}}},
        {{{0, 61}, {0, 149}, {1, 109}}},
        {{{0, 60}, {0, 148}, {1, 111}}},
        {{{0, 58}, {0, 147}, {1, 112}}},
        {{{0, 59}, {0, 146}, {0, 30}}},
        {{{0, 1}, {0, 145}, {0, 29}}},
        {{{0, 0}, {0, 144}, {0, 31}}},
        {{{1, 142}, {0, 143}, {0, 32}}},
        {{{1, 143}, {0, 142}, {0, 86}}},
        {{{1, 77}, {0, 141}, {0, 85}}},
        {{{1, 76}, {0, 140}, {0, 87}}},
        {{{1, 74}, {0, 139}, {0, 88}}},
        {{{1, 75}, {0, 138}, {0, 134}}},
        {{{1, 1}, {0, 137}, {0, 133}}},
        {{{1, 0}, {0, 136}, {0, 135}}}};

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
void drawPixel6(CRGB *leds, int index, CRGB c)
{
    if (index < 0 || index >= DRAWPIXEL6_INDEX)
    {
        DB_PRINT("drawPixel6 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (uint8_t x = 0; x < 3; x++)
        MirroredSetPixelColor(leds, pgm_read_byte_near(&drawPixel6LookupTable[index].strips[x].strip),
                              pgm_read_byte_near(&drawPixel6LookupTable[index].strips[x].index), c);
}

// draw a pixel mirrored and rotated 12 times to emulate a kaleidoscope
void drawPixel12(CRGB *leds, int index, CRGB c)
{
    if (index < 0 || index >= DRAWPIXEL12_INDEX)
    {
        DB_PRINT("drawPixel12 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (int column = 0; column < DRAWPIXEL12_COLUMNS; column++)
    {
        drawPixel6(leds, pgm_read_byte_near(&drawPixel12LookupTable[index][column]), c);
    }
}

// draw a pixel mirrored and rotated 24 times to emulate a kaleidoscope
void drawPixel24(CRGB *leds, int index, CRGB c)
{
    if (index < 0 || index >= DRAWPIXEL24_INDEX)
    {
        DB_PRINT("drawPixel24 index out of range ");
        DB_PRINTLN(index);
        return;
    }

    for (int column = 0; column < DRAWPIXEL24_COLUMNS; column++)
    {
        drawPixel6(leds, pgm_read_byte_near(&drawPixel24LookupTable[index][column]), c);
    }
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
    ::fill_solid(leds, NUM_LEDS_PER_STRIP, color);
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

// Provide a function to draw the pixels mirrored and replicated to match
// the given kaleidoscope style. Update the number of corresponding
// leds in the 'viewport' so that the ported modes work as expected.
uint8_t num_leds = DRAWPIXEL6_INDEX;
void (*drawPixel)(CRGB *leds, int index, CRGB c) = drawPixel6;

// These disks define color as an index into a CRGBPalette16 rather than an absolute RGB value.
// This takes less Flash to store and gives us flexability to change the palette separate from
// the disk structure.
#define TRIANGLE_DISK_COLUMNS 10
static const PROGMEM uint8_t TriangleDisk[TRIANGLE_DISK_COLUMNS][VIEWPORT_HEIGHT] =
    {
        {11, 1, 2, 3, 3, 3, 0, 4, 0, 0},
        {1, 1, 1, 2, 3, 0, 4, 4, 4, 6},
        {6, 1, 2, 2, 2, 3, 0, 4, 6, 6},
        {6, 6, 7, 2, 3, 3, 3, 4, 0, 6},
        {6, 7, 7, 7, 8, 3, 4, 4, 4, 5},
        {0, 12, 7, 8, 8, 8, 9, 4, 5, 5},
        {12, 12, 12, 13, 8, 9, 9, 9, 10, 5},
        {1, 12, 13, 13, 13, 14, 9, 10, 10, 10},
        {1, 1, 2, 13, 14, 14, 14, 15, 10, 11},
        {1, 2, 2, 2, 3, 14, 15, 15, 15, 11}};

#define SQUARE_DISK_COLUMNS 10
static const PROGMEM uint8_t SquareDisk[SQUARE_DISK_COLUMNS][VIEWPORT_HEIGHT] =
    {
        {1, 1, 1, 2, 2, 2, 3, 3, 3, 4},
        {1, 1, 1, 2, 2, 2, 3, 3, 3, 4},
        {1, 1, 1, 2, 2, 2, 3, 3, 3, 4},
        {4, 4, 5, 5, 6, 7, 7, 7, 8, 8},
        {4, 4, 5, 5, 6, 7, 7, 7, 8, 8},
        {4, 4, 5, 5, 6, 7, 7, 7, 8, 8},
        {8, 9, 9, 9, 10, 10, 10, 11, 11, 11},
        {8, 9, 9, 9, 10, 10, 10, 11, 11, 11},
        {8, 9, 9, 9, 10, 10, 10, 11, 11, 11},
        {12, 12, 12, 13, 13, 13, 14, 14, 14, 15}};

#define JEWEL_RED 0x800000
#define JEWEL_PUR 0x800080
#define JEWEL_BLU 0x000080
#define JEWEL_GRE 0x008000
#define JEWEL_YEL 0x808000
#define JEWEL_ORA 0x804000
#define JEWEL_LEA 0x27100a

extern const TProgmemRGBPalette16 JewelColors_p FL_PROGMEM =
    {
        JEWEL_RED,
        JEWEL_RED,
        JEWEL_PUR,
        JEWEL_PUR,

        JEWEL_BLU,
        JEWEL_BLU,
        JEWEL_GRE,
        JEWEL_GRE,

        JEWEL_YEL,
        JEWEL_YEL,
        JEWEL_ORA,
        JEWEL_ORA,

        JEWEL_RED,
        JEWEL_RED,
        JEWEL_LEA,
        JEWEL_LEA};

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

// draw the kaleidoscope using a color palette
void drawPaletteFrame(CRGB *leds, const struct CRGBPalette16 &pal,
                      const uint8_t (*disk_1)[VIEWPORT_HEIGHT], const uint8_t offset_1, const uint8_t rows_1,
                      const uint8_t (*disk_2)[VIEWPORT_HEIGHT], const uint8_t offset_2, const uint8_t rows_2)
{
    int begin = 0, end = 0, viewport_index = 0;

    // draw the kaleidoscope pixels for this 'frame'
    for (int column = 0; column < VIEWPORT_HEIGHT; column++)
    {
        for (int x = begin; x <= end; x++)
        {
            int row;

            // the row number must wrap around as needed to stay within the number of rows available in the disk
            row = x + offset_1;
            if (row < 0)
                row = (row % rows_1) + rows_1 - 1;
            else
                row = row % rows_1;

            // since the disks are stored in PROGMEM, we must read them into SRAM before using them
            uint8_t colorIndex_1 = map(pgm_read_byte_near(&disk_1[row][column]), 0, 15, 0, 255);
            int thisBright_1 = qsuba(colorIndex_1, beatsin8(7, 0, 64)); // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
            CRGB pixel_1 = ColorFromPalette(pal, colorIndex_1, thisBright_1);
#ifdef DEBUG
            if (row < 0 || row >= rows_1 || column < 0 || column >= VIEWPORT_HEIGHT)
                DB_PRINTF("colorIndex_1 = %d; pixel_1[%d][%d] = %x\r\n", colorIndex_1, row, column, (uint32_t)pixel_1);
#endif

            // the row number must wrap around as needed to stay within the number of rows available in the disk
            row = x + offset_2;
            if (row < 0)
                row = (row % rows_2) + rows_2 - 1;
            else
                row = row % rows_2;

            // since the disks are stored in PROGMEM, we must read them into SRAM before using them
            uint8_t colorIndex_2 = map(pgm_read_byte_near(&disk_2[row][column]), 0, 15, 0, 255);
            int thisBright_2 = qsuba(colorIndex_2, beatsin8(7, 0, 64)); // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
            CRGB pixel_2 = ColorFromPalette(pal, colorIndex_2, thisBright_2);
#ifdef DEBUG
            if (row < 0 || row >= rows_1 || column < 0 || column >= VIEWPORT_HEIGHT)
                DB_PRINTF("colorIndex_2 = %d; pixel_2[%d][%d] = %x\r\n", colorIndex_2, row, column, (uint32_t)pixel_2);
#endif

            // blend the pixels from the two disks by doing 50% transparency
            CRGB pixel = blend(pixel_1, pixel_2, 127);
#ifdef NDEBUG
            DB_PRINTF("blended pixel = %x\r\n", (uint32_t)pixel);
#endif
            drawPixel(leds, viewport_index, pixel);
            viewport_index++;
        }
        begin--;
        end++;
    }
}

#define MS_BETWEEN_FRAMES 1000
void mode_kaleidoscope_screensaver()
{
    // start with random offsets to provide more variety
    static uint8_t triangle_offset = random(TRIANGLE_DISK_COLUMNS);
    static uint8_t square_offset = random(SQUARE_DISK_COLUMNS);
    static boolean first_array = true;
    static int time_of_last_frame = 0;

    int time = millis();

    // draw the next frame into the correct led array
    if (time >= time_of_last_frame + MS_BETWEEN_FRAMES)
    {
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, ForestColors_p,
                         TriangleDisk, triangle_offset, TRIANGLE_DISK_COLUMNS,
                         SquareDisk, square_offset, SQUARE_DISK_COLUMNS);
        time_of_last_frame = time;
        first_array = !first_array;

        // update our offsets
#if 0
        --triangle_offset;
        if (triangle_offset < 0)
            triangle_offset += TRIANGLE_DISK_COLUMNS;
#else
        ++triangle_offset;
        triangle_offset = triangle_offset % TRIANGLE_DISK_COLUMNS;
#endif
#if 0
        --square_offset;
        if (square_offset < 0)
            square_offset += SQUARE_DISK_COLUMNS;
#else
        ++square_offset;
        square_offset = square_offset % SQUARE_DISK_COLUMNS;
#endif
    }

    // smoothly blend from one frame to the next
    EVERY_N_MILLISECONDS(5)
    {
        // ratio is the percentage of time remaining for this frame mapped to 0-255
        fract8 ratio = map(time, time_of_last_frame, time_of_last_frame + MS_BETWEEN_FRAMES, 0, 255);

        // reverse the blend ratio if we're blending from leds3 to leds2
        if (!first_array)
            ratio = 255 - ratio;

        // mix the 2 arrays together
        blend(leds2, leds3, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, ratio);
        leds_dirty = true;
    }

    adjustBrightness();
}

void mode_kaleidoscope_interactive()
{
    // start with random offsets to provide more variety
    static uint8_t triangle_offset = random(TRIANGLE_DISK_COLUMNS);
    static uint8_t square_offset = random(SQUARE_DISK_COLUMNS);
    static boolean first_array = true;
    static int time_of_last_frame = 0;
    static boolean drawframe = true;
    static boolean blendframes = false;

    int time = millis();

    // use the right knob to move triangle_offset
    static int lastRightKnob = 0;
    int knob = knobRight.getCount();
    if (knob != lastRightKnob)
    {
        DB_PRINTF("right knob = %d\r\n", knob);
        if (knob < lastRightKnob)
        {
            triangle_offset++;
            triangle_offset = triangle_offset % TRIANGLE_DISK_COLUMNS;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (triangle_offset == 0)
                triangle_offset += TRIANGLE_DISK_COLUMNS;
            --triangle_offset;
        }
        DB_PRINTF("triangle offset = %d\r\n", triangle_offset);
        lastRightKnob = knob;
        drawframe = true;
    }

    // use the left knob to move square_offset
    static int lastLeftKnob = 0;
    knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        DB_PRINTF("left knob = %d\r\n", knob);
        if (knob < lastLeftKnob)
        {
            square_offset++;
            square_offset = square_offset % SQUARE_DISK_COLUMNS;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (square_offset == 0)
                square_offset += SQUARE_DISK_COLUMNS;
            --square_offset;
        }
        DB_PRINTF("square offset = %d\r\n", square_offset);
        lastLeftKnob = knob;
        drawframe = true;
    }

    // draw the next frame into the correct led array
    if (drawframe)
    {
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, RainbowStripeColors_p,
                         TriangleDisk, triangle_offset, TRIANGLE_DISK_COLUMNS,
                         SquareDisk, square_offset, SQUARE_DISK_COLUMNS);
        time_of_last_frame = time;
        first_array = !first_array;
        blendframes = true;
        drawframe = false;
    }

    // smoothly blend from one frame to the next
    EVERY_N_MILLISECONDS(5)
    {
        if (blendframes)
        {
            // ratio is the percentage of time remaining for this frame mapped to 0-255
            fract8 ratio = map(time, time_of_last_frame, time_of_last_frame + MS_BETWEEN_FRAMES, 0, 255);
            if (ratio >= 250)
                blendframes = false;
            if (!first_array)
                ratio = 255 - ratio;

            // mix the 2 arrays together
            for (int i = 0; i < NUM_STRIPS * NUM_LEDS_PER_STRIP; i++)
            {
                leds[i] = blend(leds2[i], leds3[i], ratio);
            }
            leds_dirty = true;
        }
    }
}

void mode_kaleidoscope_select_disks()
{
}

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test()
{
    static int index = -1;

    EVERY_N_MILLISECONDS(100)
    {
        // erase the last pixel
        drawPixel(leds, index, CRGB::Black); // off

        // move to the next pixel
        if (++index >= DRAWPIXEL6_INDEX)
            index = 0;
        DB_PRINTLN(index);

        // light up the next pixel
        drawPixel(leds, index, CRGB::Red);
    }

    adjustBrightness();
}
#endif
