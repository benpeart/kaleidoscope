#include "main.h"
#include "Kaleidoscope.h"
#include "GlassDisk.h"

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

// Provide functions to draw the pixels mirrored and replicated to match
// the given kaleidoscope style. Update the number of corresponding
// leds in the 'viewport' so that the ported modes work as expected.
void (*drawPixelFunc[])(CRGB *leds, int index, CRGB c){
    drawPixel6,
    drawPixel12,
    drawPixel24};
#define N_DRAW_STYLES (sizeof(drawPixelFunc) / sizeof(drawPixelFunc[0]))
uint8_t draw_style = 0; // Index of current draw mode in table
uint8_t num_leds = DRAWPIXEL6_INDEX;

// simple wrapper function to abstract out the fact that we have different draw functions
void drawPixel(CRGB *leds, int index, CRGB c)
{
    drawPixelFunc[draw_style](leds, index, c);
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

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

// draw the kaleidoscope using a color palette
void drawPaletteFrame(CRGB *leds, GlassDisk *disk_1, GlassDisk *disk_2)
{
    int begin = 0, end = 0, viewport_index = 0;
    CRGB pixel_1 = CRGB::Gray, pixel_2 = CRGB::Gray;

    // draw the kaleidoscope pixels for this 'frame'
    for (int row = 0; row < VIEWPORT_HEIGHT; row++)
    {
        for (int x = begin; x <= end; x++)
        {
            int column;

            if (disk_1)
            {
                // the row number must wrap around as needed to stay within the number of rows available in the disk
                column = x + disk_1->offset;
                if (column < 0)
                    column = (column % disk_1->columns) + disk_1->columns - 1;
                else
                    column = column % disk_1->columns;

                // since the disks are stored in PROGMEM, we must read them into SRAM before using them
                uint8_t colorIndex_1 = map(pgm_read_byte_near(&disk_1->array[row * disk_1->columns + column]), 0, 15, 0, 255);
                //int thisBright_1 = qsuba(colorIndex_1, beatsin8(7, 0, 64)); // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
                pixel_1 = ColorFromPalette(*disk_1->pal, colorIndex_1 /*, thisBright_1*/);
#ifdef DEBUG
                if (column < 0 || column >= disk_1->columns || row < 0 || row >= VIEWPORT_HEIGHT)
                    DB_PRINTF("colorIndex_1 = %d; pixel_1[%d][%d] = %x\r\n", colorIndex_1, column, row, (uint32_t)pixel_1);
#endif
            }

            if (disk_2)
            {
                // the row number must wrap around as needed to stay within the number of rows available in the disk
                column = x + disk_2->offset;
                if (column < 0)
                    column = (column % disk_2->columns) + disk_2->columns - 1;
                else
                    column = column % disk_2->columns;

                // since the disks are stored in PROGMEM, we must read them into SRAM before using them
                uint8_t colorIndex_2 = map(pgm_read_byte_near(&disk_2->array[row * disk_2->columns + column]), 0, 15, 0, 255);
                //int thisBright_2 = qsuba(colorIndex_2, beatsin8(7, 0, 64)); // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
                pixel_2 = ColorFromPalette(*disk_2->pal, colorIndex_2 /*, thisBright_2*/);
#ifdef DEBUG
                if (column < 0 || column >= disk_2->columns || row < 0 || row >= VIEWPORT_HEIGHT)
                    DB_PRINTF("colorIndex_2 = %d; pixel_2[%d][%d] = %x\r\n", colorIndex_2, column, row, (uint32_t)pixel_2);
#endif
            }

            // blend the pixels from the two disks by doing 50% transparency
            CRGB pixel = blend(pixel_1, pixel_2, 127);
#ifdef NDEBUG
            DB_PRINTF("blended pixel = %x\r\n", (uint32_t)pixel);
#endif
            drawPixel(leds, viewport_index, pixel_2 == (CRGB)CRGB::Gray ? pixel_1 : pixel);
            viewport_index++;

            // bail out early if the number of unique pixels in the viewport exceeds the
            // number allowed by the current draw style
            if (viewport_index >= num_leds)
                return;
        }
        begin--;
        end++;
    }
}

//
// use the left rotary knob to adjust the speed of the screensaver
// by in/decreasing the number of milliseconds between frames
//
#define SPEED_INCREMENT 25
#define MAX_SPEED_DELAY 2000
int ms_between_frames = 500;
int adjustSpeed()
{
#ifdef ENCODER
    static int lastLeftKnob = 0;

    int knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        if (knob > lastLeftKnob)
            ms_between_frames -= SPEED_INCREMENT;
        else
            ms_between_frames += SPEED_INCREMENT;

        // don't allow this to go to zero as it causes issues when mapping the remaining
        // time for the blend function between frames
        ms_between_frames = constrain(ms_between_frames, SPEED_INCREMENT, MAX_SPEED_DELAY);
        lastLeftKnob = knob;

        DB_PRINTF("Screensaver ms between frames = %d\r\n", ms_between_frames);
    }
#endif
    return ms_between_frames;
}

void mode_kaleidoscope_screensaver()
{
    static boolean first_array = true;
    static int time_of_last_frame = 0;

    int time = millis();

    // draw the next frame into the correct led array
    if (time >= time_of_last_frame + ms_between_frames)
    {
#ifdef NDEBUG
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, &TestDisk, NULL);
#else
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, &TriangleDisk, &SquareDisk);
#endif
        time_of_last_frame = time;
        first_array = !first_array;

        // update our offsets
        ++TriangleDisk;
        ++SquareDisk;
    }

    // smoothly blend from one frame to the next
    EVERY_N_MILLISECONDS(5)
    {
        // ratio is the percentage of time remaining for this frame mapped to 0-255
        fract8 ratio = map(time, time_of_last_frame, time_of_last_frame + ms_between_frames, 0, 255);

        // reverse the blend ratio if we're blending from leds3 to leds2
        if (!first_array)
            ratio = 255 - ratio;

        // mix the 2 arrays together
        blend(leds2, leds3, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, ratio);
        leds_dirty = true;
    }

    adjustSpeed();
    adjustBrightness();
}

void mode_kaleidoscope_interactive()
{
    static boolean first_array = true;
    static int time_of_last_frame = 0;
    static boolean drawframe = true;
    static boolean blendframes = false;

    int time = millis();

#ifdef ENCODER
    // use the right knob to move triangle_offset
    static int lastRightKnob = 0;
    int knob = knobRight.getCount();
    if (knob != lastRightKnob)
    {
        DB_PRINTF("right knob = %d\r\n", knob);
        if (knob < lastRightKnob)
            ++TriangleDisk;
        else
            --TriangleDisk;
        DB_PRINTF("triangle offset = %d\r\n", TriangleDisk.offset);
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
            ++SquareDisk;
        else
            --SquareDisk;
        DB_PRINTF("square offset = %d\r\n", SquareDisk.offset);
        lastLeftKnob = knob;
        drawframe = true;
    }
#endif

    // draw the next frame into the correct led array
    if (drawframe)
    {
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, &TriangleDisk, &SquareDisk);
        time_of_last_frame = time;
        first_array = !first_array;
        blendframes = true;
        drawframe = false;
    }

    // smoothly blend from one frame to the next
    if (blendframes)
    {
        EVERY_N_MILLISECONDS(5)
        {
            // ratio is the percentage of time remaining for this frame mapped to 0-255
            fract8 ratio = map(time, time_of_last_frame, time_of_last_frame + ms_between_frames, 0, 255);
            if (ratio >= 250)
                blendframes = false;
            if (!first_array)
                ratio = 255 - ratio;

            // mix the 2 arrays together
            blend(leds2, leds3, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, ratio);
            leds_dirty = true;
        }
    }
}

#define SCREENSAVER_DELAY 60000 // 1 minute = 60,000 milliseconds
void mode_kaleidoscope()
{
    static int time_to_enter_screensaver_mode = 0; // start in screensaver mode
    static boolean first_array = true;
    static int time_of_last_frame = 0;
    static boolean drawframe = true;
    static boolean blendframes = false;

    int time = millis();

    // if it is time to enter screen saver mode
    if (time >= time_to_enter_screensaver_mode)
    {
        // if it is time to draw the next frame
        if (time >= time_of_last_frame + ms_between_frames)
        {
            // update our offsets
            ++TriangleDisk;
            ++SquareDisk;
            drawframe = true;
        }
    }

#ifdef ENCODER
    // use the right knob to move triangle_offset
    static int lastRightKnob = 0;
    int knob = knobRight.getCount();
    if (knob != lastRightKnob)
    {
        // reset the screensaver timeout
        time_to_enter_screensaver_mode = time + SCREENSAVER_DELAY;

        DB_PRINTF("right knob = %d\r\n", knob);
        if (knob < lastRightKnob)
            ++TriangleDisk;
        else
            --TriangleDisk;
        DB_PRINTF("triangle offset = %d\r\n", TriangleDisk.offset);
        lastRightKnob = knob;
        drawframe = true;
    }

    // use the left knob to move square_offset
    static int lastLeftKnob = 0;
    knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        // reset the screensaver timeout
        time_to_enter_screensaver_mode = time + SCREENSAVER_DELAY;

        DB_PRINTF("left knob = %d\r\n", knob);
        if (knob < lastLeftKnob)
            ++SquareDisk;
        else
            --SquareDisk;
        DB_PRINTF("square offset = %d\r\n", SquareDisk.offset);
        lastLeftKnob = knob;
        drawframe = true;
    }
#endif

    // draw the next frame into the correct led array
    if (drawframe)
    {
        // draw the next frame of the kaleidoscope
        drawPaletteFrame(first_array ? leds2 : leds3, &TriangleDisk, &SquareDisk);
        time_of_last_frame = time;
        first_array = !first_array;
        blendframes = true;
        drawframe = false;
    }

    // smoothly blend from one frame to the next
    if (blendframes)
    {
        EVERY_N_MILLISECONDS(5)
        {
            // ratio is the percentage of time remaining for this frame mapped to 0-255
            fract8 ratio = map(time, time_of_last_frame, time_of_last_frame + ms_between_frames, 0, 255);
            if (ratio >= 250)
                blendframes = false;
            if (!first_array)
                ratio = 255 - ratio;

            // mix the 2 arrays together
            blend(leds2, leds3, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, ratio);
            leds_dirty = true;
        }
    }
}

void mode_kaleidoscope_select_disks()
{
    // select disk with left rotary encoder
    // select color palette with right rotary encoder
}

void mode_kaleidoscope_select_reflection_style()
{
    static boolean drawStyleChanged = false;
#ifdef ENCODER
    // use the left knob to select kaleidoscope draw style
    static int lastLeftKnob = 0;
    int knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        if (knob < lastLeftKnob)
        {
            draw_style++;
            draw_style = draw_style % N_DRAW_STYLES;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (draw_style == 0)
                draw_style += N_DRAW_STYLES;
            --draw_style;
        }
        lastLeftKnob = knob;
        drawStyleChanged = true;
    }
#endif

    // redraw the frame of the kaleidoscope with the new style
    if (drawStyleChanged)
    {
        switch (draw_style)
        {
        case 0:
            num_leds = DRAWPIXEL6_INDEX;
            DB_PRINTLN("reflection_style is 6 way reflection");
            break;

        case 1:
            num_leds = DRAWPIXEL12_INDEX;
            DB_PRINTLN("reflection_style is 12 way reflection");
            break;

        case 2:
            num_leds = DRAWPIXEL24_INDEX;
            DB_PRINTLN("reflection_style is 24 way reflection");
            break;
        }
        drawStyleChanged = false;
    }

    // draw this on a fixed schedule so that when you enter the mode, you aren't
    // faced with a black screen until you rotate the left knob
    EVERY_N_MILLISECONDS(100)
    {
        drawPaletteFrame(leds, &TriangleDisk, &SquareDisk);
    }

    adjustBrightness();
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
        if (++index >= num_leds)
            index = 0;
        DB_PRINTLN(index);

        // light up the next pixel
        drawPixel(leds, index, CRGB::Red);
    }

    adjustBrightness();
}
#endif
