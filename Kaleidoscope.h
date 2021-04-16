#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H
#include "LEDStrips.h"

// statically define the 'filmstrips' to use to generate the kaleidoscope
#define TRIANGLE_ROWS 10                                            // the height of the 'viewport' triangle
#define TRIANGLE_COLUMNS 19                                         // the width of the base of the 'viewport' triange
#define TRIANGLE_COUNT (TRIANGLE_ROWS * (TRIANGLE_COLUMNS + 1) / 2) // the number of pixels in the 'viewport' triangle

#define JEWEL_STRIP_COLUMNS 28
#define JEWEL_RED 0x770000
#define JEWEL_GREEN 0x007700
#define JEWEL_BLUE 0x000077
#define JEWEL_ORANGE 0x774000
#define JEWEL_YELLOW 0x777700
#define JEWEL_PURPLE 0x770077
#define JEWEL_LEAD 0x27100a

// TODO: optimize the strip array to only require 2 bytes per pixel (16 bit color)
// store the strip RGB values in program memory (flash) to save SRAM
static const PROGMEM uint32_t JewelStrip[JEWEL_STRIP_COLUMNS][TRIANGLE_ROWS] =
    {
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE},
        {JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD}};

#ifdef NDEBUG
#define BLUE_STRIP_COLUMNS 1
static const PROGMEM uint32_t BlueStrip[][TRIANGLE_ROWS] =
    {{0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF},
     {0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF}};

#define RED_STRIP_COLUMNS 1
static const PROGMEM uint32_t RedStrip[][TRIANGLE_ROWS] =
    {{0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000},
     {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000}};
#endif

class Kaleidoscope
{
public:
    void setup()
    {
        DB_PRINTLN(F("Kaleidoscope.setup"));
#ifdef NDEBUG
        rgb_strip_1 = BlueStrip;
        strip_1_columns = BLUE_STRIP_COLUMNS;
        rgb_strip_2 = RedStrip;
        strip_2_columns = RED_STRIP_COLUMNS;
#else
        rgb_strip_1 = JewelStrip;
        strip_1_columns = JEWEL_STRIP_COLUMNS;
        rgb_strip_2 = JewelStrip;
        strip_2_columns = JEWEL_STRIP_COLUMNS;
#endif

        // start with random offsets to provide more variety
        current_offset_1 = random(strip_1_columns);
        current_offset_2 = random(strip_2_columns);
    }

    // update the position of the strips and draw the kaleidoscope
    void loop()
    {
        draw(current_offset_1, current_offset_2);

        current_offset_1 = (++current_offset_1) % strip_1_columns;
        current_offset_2 = (++current_offset_2) % strip_2_columns;
    }

    // https://stackoverflow.com/questions/1102692/how-to-alpha-blend-rgba-unsigned-byte-color-fast
    uint32_t blendAlpha(uint32_t colora, uint32_t colorb, uint32_t alpha)
    {
        uint32_t rb1 = ((0xFF - alpha) * (colora & 0xFF00FF)) >> 8;
        uint32_t rb2 = (alpha * (colorb & 0xFF00FF)) >> 8;
        uint32_t g1 = ((0xFF - alpha) * (colora & 0x00FF00)) >> 8;
        uint32_t g2 = (alpha * (colorb & 0x00FF00)) >> 8;

        return ((rb1 + rb2) & 0xFF00FF) + ((g1 + g2) & 0x00FF00);
    }

    // draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
    void drawKaleidoscopePixel6(uint16_t index, uint32_t c)
    {
        // The index assumes we itterate through the bottom triange top to bottom, left to right.
        // To keep the LED_strips together, rotate around the hexagon in counter-clockwise order
        switch (index)
        {
        case 0:
            MirroredSetPixelColor(1, 18, c);
            MirroredSetPixelColor(1, 19, c);
            MirroredSetPixelColor(1, 20, c);
            break;
        case 1:
            MirroredSetPixelColor(1, 59, c);
            MirroredSetPixelColor(1, 58, c);
            MirroredSetPixelColor(1, 22, c);
            break;
        case 2:
            MirroredSetPixelColor(1, 17, c);
            MirroredSetPixelColor(1, 57, c);
            MirroredSetPixelColor(1, 21, c);
            break;
        case 3:
            MirroredSetPixelColor(1, 16, c);
            MirroredSetPixelColor(1, 56, c);
            MirroredSetPixelColor(1, 55, c);
            break;
        case 4:
            MirroredSetPixelColor(1, 90, c);
            MirroredSetPixelColor(1, 91, c);
            MirroredSetPixelColor(1, 24, c);
            break;
        case 5:
            MirroredSetPixelColor(1, 60, c);
            MirroredSetPixelColor(1, 92, c);
            MirroredSetPixelColor(1, 23, c);
            break;
        case 6:
            MirroredSetPixelColor(1, 61, c);
            MirroredSetPixelColor(1, 93, c);
            MirroredSetPixelColor(1, 53, c);
            break;
        case 7:
            MirroredSetPixelColor(1, 15, c);
            MirroredSetPixelColor(1, 94, c);
            MirroredSetPixelColor(1, 54, c);
            break;
        case 8:
            MirroredSetPixelColor(1, 14, c);
            MirroredSetPixelColor(1, 95, c);
            MirroredSetPixelColor(1, 96, c);
            break;
        case 9:
            MirroredSetPixelColor(1, 131, c);
            MirroredSetPixelColor(1, 130, c);
            MirroredSetPixelColor(1, 26, c);
            break;
        case 10:
            MirroredSetPixelColor(1, 89, c);
            MirroredSetPixelColor(1, 129, c);
            MirroredSetPixelColor(1, 25, c);
            break;
        case 11:
            MirroredSetPixelColor(1, 88, c);
            MirroredSetPixelColor(1, 128, c);
            MirroredSetPixelColor(1, 51, c);
            break;
        case 12:
            MirroredSetPixelColor(1, 62, c);
            MirroredSetPixelColor(1, 127, c);
            MirroredSetPixelColor(1, 52, c);
            break;
        case 13:
            MirroredSetPixelColor(1, 63, c);
            MirroredSetPixelColor(1, 126, c);
            MirroredSetPixelColor(1, 98, c);
            break;
        case 14:
            MirroredSetPixelColor(1, 13, c);
            MirroredSetPixelColor(1, 125, c);
            MirroredSetPixelColor(1, 97, c);
            break;
        case 15:
            MirroredSetPixelColor(1, 12, c);
            MirroredSetPixelColor(1, 124, c);
            MirroredSetPixelColor(1, 123, c);
            break;
        case 16:
            MirroredSetPixelColor(0, 10, c);
            MirroredSetPixelColor(0, 11, c);
            MirroredSetPixelColor(1, 28, c);
            break;
        case 17:
            MirroredSetPixelColor(1, 132, c);
            MirroredSetPixelColor(0, 12, c);
            MirroredSetPixelColor(1, 27, c);
            break;
        case 18:
            MirroredSetPixelColor(1, 133, c);
            MirroredSetPixelColor(0, 13, c);
            MirroredSetPixelColor(1, 49, c);
            break;
        case 19:
            MirroredSetPixelColor(1, 87, c);
            MirroredSetPixelColor(0, 14, c);
            MirroredSetPixelColor(1, 50, c);
            break;
        case 20:
            MirroredSetPixelColor(1, 86, c);
            MirroredSetPixelColor(0, 15, c);
            MirroredSetPixelColor(1, 100, c);
            break;
        case 21:
            MirroredSetPixelColor(1, 64, c);
            MirroredSetPixelColor(0, 16, c);
            MirroredSetPixelColor(1, 99, c);
            break;
        case 22:
            MirroredSetPixelColor(1, 65, c);
            MirroredSetPixelColor(0, 17, c);
            MirroredSetPixelColor(1, 121, c);
            break;
        case 23:
            MirroredSetPixelColor(1, 11, c);
            MirroredSetPixelColor(0, 18, c);
            MirroredSetPixelColor(1, 122, c);
            break;
        case 24:
            MirroredSetPixelColor(1, 10, c);
            MirroredSetPixelColor(0, 19, c);
            MirroredSetPixelColor(0, 20, c);
            break;
        case 25:
            MirroredSetPixelColor(0, 51, c);
            MirroredSetPixelColor(0, 50, c);
            MirroredSetPixelColor(1, 30, c);
            break;
        case 26:
            MirroredSetPixelColor(0, 9, c);
            MirroredSetPixelColor(0, 49, c);
            MirroredSetPixelColor(1, 29, c);
            break;
        case 27:
            MirroredSetPixelColor(0, 8, c);
            MirroredSetPixelColor(0, 48, c);
            MirroredSetPixelColor(1, 47, c);
            break;
        case 28:
            MirroredSetPixelColor(1, 134, c);
            MirroredSetPixelColor(0, 47, c);
            MirroredSetPixelColor(1, 48, c);
            break;
        case 29:
            MirroredSetPixelColor(1, 135, c);
            MirroredSetPixelColor(0, 46, c);
            MirroredSetPixelColor(1, 102, c);
            break;
        case 30:
            MirroredSetPixelColor(1, 85, c);
            MirroredSetPixelColor(0, 45, c);
            MirroredSetPixelColor(1, 101, c);
            break;
        case 31:
            MirroredSetPixelColor(1, 84, c);
            MirroredSetPixelColor(0, 44, c);
            MirroredSetPixelColor(1, 119, c);
            break;
        case 32:
            MirroredSetPixelColor(1, 66, c);
            MirroredSetPixelColor(0, 43, c);
            MirroredSetPixelColor(1, 120, c);
            break;
        case 33:
            MirroredSetPixelColor(1, 67, c);
            MirroredSetPixelColor(0, 42, c);
            MirroredSetPixelColor(0, 22, c);
            break;
        case 34:
            MirroredSetPixelColor(1, 9, c);
            MirroredSetPixelColor(0, 41, c);
            MirroredSetPixelColor(0, 21, c);
            break;
        case 35:
            MirroredSetPixelColor(1, 8, c);
            MirroredSetPixelColor(0, 40, c);
            MirroredSetPixelColor(0, 39, c);
            break;
        case 36:
            MirroredSetPixelColor(0, 66, c);
            MirroredSetPixelColor(0, 67, c);
            MirroredSetPixelColor(1, 32, c);
            break;
        case 37:
            MirroredSetPixelColor(0, 52, c);
            MirroredSetPixelColor(0, 68, c);
            MirroredSetPixelColor(1, 31, c);
            break;
        case 38:
            MirroredSetPixelColor(0, 53, c);
            MirroredSetPixelColor(0, 69, c);
            MirroredSetPixelColor(1, 45, c);
            break;
        case 39:
            MirroredSetPixelColor(0, 7, c);
            MirroredSetPixelColor(0, 70, c);
            MirroredSetPixelColor(1, 46, c);
            break;
        case 40:
            MirroredSetPixelColor(0, 6, c);
            MirroredSetPixelColor(0, 71, c);
            MirroredSetPixelColor(1, 104, c);
            break;
        case 41:
            MirroredSetPixelColor(1, 136, c);
            MirroredSetPixelColor(0, 72, c);
            MirroredSetPixelColor(1, 103, c);
            break;
        case 42:
            MirroredSetPixelColor(1, 137, c);
            MirroredSetPixelColor(0, 73, c);
            MirroredSetPixelColor(1, 117, c);
            break;
        case 43:
            MirroredSetPixelColor(1, 83, c);
            MirroredSetPixelColor(0, 71, c);
            MirroredSetPixelColor(1, 118, c);
            break;
        case 44:
            MirroredSetPixelColor(1, 62, c);
            MirroredSetPixelColor(0, 75, c);
            MirroredSetPixelColor(0, 24, c);
            break;
        case 45:
            MirroredSetPixelColor(1, 68, c);
            MirroredSetPixelColor(0, 76, c);
            MirroredSetPixelColor(0, 23, c);
            break;
        case 46:
            MirroredSetPixelColor(1, 69, c);
            MirroredSetPixelColor(0, 77, c);
            MirroredSetPixelColor(0, 37, c);
            break;
        case 47:
            MirroredSetPixelColor(1, 7, c);
            MirroredSetPixelColor(0, 78, c);
            MirroredSetPixelColor(0, 38, c);
            break;
        case 48:
            MirroredSetPixelColor(1, 6, c);
            MirroredSetPixelColor(0, 79, c);
            MirroredSetPixelColor(0, 80, c);
            break;
        case 49:
            MirroredSetPixelColor(0, 107, c);
            MirroredSetPixelColor(0, 106, c);
            MirroredSetPixelColor(1, 34, c);
            break;
        case 50:
            MirroredSetPixelColor(0, 65, c);
            MirroredSetPixelColor(0, 105, c);
            MirroredSetPixelColor(1, 33, c);
            break;
        case 51:
            MirroredSetPixelColor(0, 64, c);
            MirroredSetPixelColor(0, 104, c);
            MirroredSetPixelColor(1, 43, c);
            break;
        case 52:
            MirroredSetPixelColor(0, 54, c);
            MirroredSetPixelColor(0, 103, c);
            MirroredSetPixelColor(1, 44, c);
            break;
        case 53:
            MirroredSetPixelColor(0, 55, c);
            MirroredSetPixelColor(0, 102, c);
            MirroredSetPixelColor(1, 106, c);
            break;
        case 54:
            MirroredSetPixelColor(0, 5, c);
            MirroredSetPixelColor(0, 101, c);
            MirroredSetPixelColor(1, 105, c);
            break;
        case 55:
            MirroredSetPixelColor(0, 4, c);
            MirroredSetPixelColor(0, 100, c);
            MirroredSetPixelColor(1, 115, c);
            break;
        case 56:
            MirroredSetPixelColor(1, 138, c);
            MirroredSetPixelColor(0, 99, c);
            MirroredSetPixelColor(1, 116, c);
            break;
        case 57:
            MirroredSetPixelColor(1, 139, c);
            MirroredSetPixelColor(0, 98, c);
            MirroredSetPixelColor(0, 26, c);
            break;
        case 58:
            MirroredSetPixelColor(1, 81, c);
            MirroredSetPixelColor(0, 97, c);
            MirroredSetPixelColor(0, 25, c);
            break;
        case 59:
            MirroredSetPixelColor(1, 80, c);
            MirroredSetPixelColor(0, 96, c);
            MirroredSetPixelColor(0, 35, c);
            break;
        case 60:
            MirroredSetPixelColor(1, 70, c);
            MirroredSetPixelColor(0, 95, c);
            MirroredSetPixelColor(0, 36, c);
            break;
        case 61:
            MirroredSetPixelColor(1, 71, c);
            MirroredSetPixelColor(0, 94, c);
            MirroredSetPixelColor(0, 82, c);
            break;
        case 62:
            MirroredSetPixelColor(1, 5, c);
            MirroredSetPixelColor(0, 93, c);
            MirroredSetPixelColor(0, 81, c);
            break;
        case 63:
            MirroredSetPixelColor(1, 4, c);
            MirroredSetPixelColor(0, 92, c);
            MirroredSetPixelColor(0, 91, c);
            break;
        case 64:
            MirroredSetPixelColor(0, 114, c);
            MirroredSetPixelColor(0, 115, c);
            MirroredSetPixelColor(1, 36, c);
            break;
        case 65:
            MirroredSetPixelColor(0, 108, c);
            MirroredSetPixelColor(0, 116, c);
            MirroredSetPixelColor(1, 35, c);
            break;
        case 66:
            MirroredSetPixelColor(0, 109, c);
            MirroredSetPixelColor(0, 117, c);
            MirroredSetPixelColor(1, 41, c);
            break;
        case 67:
            MirroredSetPixelColor(0, 63, c);
            MirroredSetPixelColor(0, 118, c);
            MirroredSetPixelColor(1, 42, c);
            break;
        case 68:
            MirroredSetPixelColor(0, 62, c);
            MirroredSetPixelColor(0, 119, c);
            MirroredSetPixelColor(1, 108, c);
            break;
        case 69:
            MirroredSetPixelColor(0, 56, c);
            MirroredSetPixelColor(0, 120, c);
            MirroredSetPixelColor(1, 107, c);
            break;
        case 70:
            MirroredSetPixelColor(0, 57, c);
            MirroredSetPixelColor(0, 121, c);
            MirroredSetPixelColor(1, 113, c);
            break;
        case 71:
            MirroredSetPixelColor(0, 3, c);
            MirroredSetPixelColor(0, 122, c);
            MirroredSetPixelColor(1, 114, c);
            break;
        case 72:
            MirroredSetPixelColor(0, 2, c);
            MirroredSetPixelColor(0, 123, c);
            MirroredSetPixelColor(0, 28, c);
            break;
        case 73:
            MirroredSetPixelColor(1, 140, c);
            MirroredSetPixelColor(0, 124, c);
            MirroredSetPixelColor(0, 27, c);
            break;
        case 74:
            MirroredSetPixelColor(1, 141, c);
            MirroredSetPixelColor(0, 125, c);
            MirroredSetPixelColor(0, 33, c);
            break;
        case 75:
            MirroredSetPixelColor(1, 79, c);
            MirroredSetPixelColor(0, 126, c);
            MirroredSetPixelColor(0, 34, c);
            break;
        case 76:
            MirroredSetPixelColor(1, 78, c);
            MirroredSetPixelColor(0, 127, c);
            MirroredSetPixelColor(0, 84, c);
            break;
        case 77:
            MirroredSetPixelColor(1, 72, c);
            MirroredSetPixelColor(0, 128, c);
            MirroredSetPixelColor(0, 83, c);
            break;
        case 78:
            MirroredSetPixelColor(1, 73, c);
            MirroredSetPixelColor(0, 129, c);
            MirroredSetPixelColor(0, 89, c);
            break;
        case 79:
            MirroredSetPixelColor(1, 3, c);
            MirroredSetPixelColor(0, 130, c);
            MirroredSetPixelColor(0, 90, c);
            break;
        case 80:
            MirroredSetPixelColor(1, 2, c);
            MirroredSetPixelColor(0, 131, c);
            MirroredSetPixelColor(0, 132, c);
            break;
        case 81:
            MirroredSetPixelColor(0, 155, c);
            MirroredSetPixelColor(0, 154, c);
            MirroredSetPixelColor(1, 38, c);
            break;
        case 82:
            MirroredSetPixelColor(0, 113, c);
            MirroredSetPixelColor(0, 153, c);
            MirroredSetPixelColor(1, 37, c);
            break;
        case 83:
            MirroredSetPixelColor(0, 112, c);
            MirroredSetPixelColor(0, 152, c);
            MirroredSetPixelColor(1, 39, c);
            break;
        case 84:
            MirroredSetPixelColor(0, 110, c);
            MirroredSetPixelColor(0, 151, c);
            MirroredSetPixelColor(1, 40, c);
            break;
        case 85:
            MirroredSetPixelColor(0, 111, c);
            MirroredSetPixelColor(0, 150, c);
            MirroredSetPixelColor(1, 110, c);
            break;
        case 86:
            MirroredSetPixelColor(0, 61, c);
            MirroredSetPixelColor(0, 149, c);
            MirroredSetPixelColor(1, 109, c);
            break;
        case 87:
            MirroredSetPixelColor(0, 60, c);
            MirroredSetPixelColor(0, 148, c);
            MirroredSetPixelColor(1, 111, c);
            break;
        case 88:
            MirroredSetPixelColor(0, 58, c);
            MirroredSetPixelColor(0, 149, c);
            MirroredSetPixelColor(1, 112, c);
            break;
        case 89:
            MirroredSetPixelColor(0, 59, c);
            MirroredSetPixelColor(0, 146, c);
            MirroredSetPixelColor(0, 30, c);
            break;
        case 90:
            MirroredSetPixelColor(0, 1, c);
            MirroredSetPixelColor(0, 145, c);
            MirroredSetPixelColor(0, 29, c);
            break;
        case 91:
            MirroredSetPixelColor(0, 0, c);
            MirroredSetPixelColor(0, 144, c);
            MirroredSetPixelColor(0, 31, c);
            break;
        case 92:
            MirroredSetPixelColor(1, 142, c);
            MirroredSetPixelColor(0, 143, c);
            MirroredSetPixelColor(0, 32, c);
            break;
        case 93:
            MirroredSetPixelColor(1, 143, c);
            MirroredSetPixelColor(0, 142, c);
            MirroredSetPixelColor(0, 86, c);
            break;
        case 94:
            MirroredSetPixelColor(1, 77, c);
            MirroredSetPixelColor(0, 141, c);
            MirroredSetPixelColor(0, 85, c);
            break;
        case 95:
            MirroredSetPixelColor(1, 76, c);
            MirroredSetPixelColor(0, 140, c);
            MirroredSetPixelColor(0, 87, c);
            break;
        case 96:
            MirroredSetPixelColor(1, 74, c);
            MirroredSetPixelColor(0, 139, c);
            MirroredSetPixelColor(0, 88, c);
            break;
        case 97:
            MirroredSetPixelColor(1, 75, c);
            MirroredSetPixelColor(0, 136, c);
            MirroredSetPixelColor(0, 134, c);
            break;
        case 98:
            MirroredSetPixelColor(1, 1, c);
            MirroredSetPixelColor(0, 137, c);
            MirroredSetPixelColor(0, 133, c);
            break;
        case 99:
            MirroredSetPixelColor(1, 0, c);
            MirroredSetPixelColor(0, 136, c);
            MirroredSetPixelColor(0, 135, c);
            break;
        }
    }

private:
    // pointer to two dimensional arrays of color values for kaleidoscope effect
    // https://stackoverflow.com/questions/1052818/create-a-pointer-to-two-dimensional-array
    const uint32_t (*rgb_strip_1)[TRIANGLE_ROWS];
    uint8_t strip_1_columns;
    const uint32_t (*rgb_strip_2)[TRIANGLE_ROWS];
    uint8_t strip_2_columns;

    // offset into strips, used to provide movement for the kaleidoscope
    int current_offset_1;
    int current_offset_2;

    // this will blend two colors from the given stips/offsets and draw the kaleidoscope
    void draw(uint8_t offset_1, uint8_t offset_2)
    {
        int begin = 0, end = 0, viewport_index = 0;

        // draw the kaleidoscope pixels for this 'frame'
        for (int row = 0; row < TRIANGLE_ROWS; row++)
        {
            for (int x = begin; x <= end; x++)
            {
                int column;

                // the column number must wrap around as needed to stay within the number
                // of columns available in the strip
                column = x + offset_1;
                if (column < 0)
                    column = (column % strip_1_columns) + strip_1_columns - 1;
                else
                    column = column % strip_1_columns;

                // since the strips are stored in PROGMEM, we must read them into SRAM before using them
                uint32_t pixel_1 = pgm_read_dword_near(&rgb_strip_1[column][row]);
#ifdef NDEBUG
                DB_PRINT(F("pixel_1["));
                DB_PRINT(column);
                DB_PRINT(F("]["));
                DB_PRINT(row);
                DB_PRINT(F("] = 0x"));
                DB_PRINTLN(pixel_1, HEX);
#endif

                // the column number must wrap around as needed to stay within the number
                // of columns available in the strip
                column = x + offset_2;
                if (column < 0)
                    column = (column % strip_2_columns) + strip_2_columns - 1;
                else
                    column = column % strip_2_columns;

                // since the strips are stored in PROGMEM, we must read them into SRAM before using them
                uint32_t pixel_2 = pgm_read_dword_near(&rgb_strip_2[column][row]);
#ifdef NDEBUG
                DB_PRINT(F("pixel_2["));
                DB_PRINT(column);
                DB_PRINT(F("]["));
                DB_PRINT(row);
                DB_PRINT(F("] = 0x"));
                DB_PRINTLN(pixel_2, HEX);
#endif

                // blend the pixels from the two strips by doing 50% transparency
                uint32_t pixel = blendAlpha(pixel_1, pixel_2, 0x7f);
#ifdef NDEBUG
                DB_PRINT(F("blended pixel = 0x"));
                DB_PRINTLN(pixel, HEX);
#endif
                drawKaleidoscopePixel6(viewport_index, pixel);
                viewport_index++;
            }
            begin--;
            end++;
        }
    }

    void MirroredSetPixelColor(int strip, int index, uint32_t c)
    {
#ifdef DEBUG
        if (index >= LEDS_PER_STRIP)
        {
            DB_PRINT(F("MysetPixelColor: requested index ("));
            DB_PRINT(index);
            DB_PRINTLN(F(") exceeds number of LEDs in a strip."));
            return;
        }
#endif
        switch (strip)
        {
        case 0:
            leds.strip[0][index] = c;
            // FIX THIS (when we have more space)!
            //            leds.strip[3][index] = c;
            break;

        case 1:
            // FIX THIS (when we have more space)!
            //            leds.strip[1][index] = c;
            //            leds.strip[2][index] = c;
            break;

        default:
            DB_PRINT(F("MysetPixelColor: invalid strip ID = "));
            DB_PRINTLN(strip);
            break;
        }
    }
};

extern Kaleidoscope kaleidoscope;

void mode_kaleidoscope_screensaver()
{
    EVERY_N_MILLISECONDS(250)
    {
        // animate and draw the kaleidoscope
        kaleidoscope.loop();
    }
}

void mode_kaleidoscope_interactive()
{
    kaleidoscope.loop();
}

#ifdef DEBUG
// loop through all pixels in the source triange making sure they
// get reflected and mirrored properly
void mode_kaleidoscope_test()
{
    static uint32_t index = -1;

    EVERY_N_MILLISECONDS(100)
    {
        // erase the last pixel
        kaleidoscope.drawKaleidoscopePixel6(index, 0); // off

        // move to the next pixel
        if (++index >= TRIANGLE_COUNT)
            index = 0;
        DB_PRINTLN(index);

        // light up the next pixel
        kaleidoscope.drawKaleidoscopePixel6(index, 0xFF0000); // Red
    }
}
#endif

void mode_kaleidoscope_select_disks()
{
    DB_PRINTLN(F("mode_kaleidoscope_select_disks"));
}

#endif // KALEIDOSCOPE_H
