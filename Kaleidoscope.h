#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H
#include "LEDStrips.h"

// statically define the 'filmstrips' to use to generate the kaleidoscope
#define TRIANGLE_ROWS 7                                             // the height of the 'viewport' triangle
#define TRIANGLE_COLUMNS 13                                         // the width of the base of the 'viewport' triange
#define TRIANGLE_COUNT (TRIANGLE_ROWS * (TRIANGLE_COLUMNS + 1) / 2) // the number of pixels in the 'viewport' triangle

#define JEWEL_STRIP_COLUMNS 28
#define JEWEL_RED 0x7b1542
#define JEWEL_GREEN 0x249b23
#define JEWEL_BLUE 0x1c5dd2
#define JEWEL_ORANGE 0xd97217
#define JEWEL_YELLOW 0xe5bb1c
#define JEWEL_PURPLE 0x8a50b7
#define JEWEL_LEAD 0x27100a

// TODO: optimize the strip array to only require 2 bytes per pixel (16 bit color)
// store the strip RGB values in program memory (flash) to save SRAM
static const uint32_t PROGMEM JewelStrip[JEWEL_STRIP_COLUMNS][TRIANGLE_ROWS] =
    {
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE},
        {JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_GREEN, JEWEL_GREEN},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED, JEWEL_RED},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD},

        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_ORANGE, JEWEL_ORANGE},
        {JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD, JEWEL_LEAD}};

#ifdef __DEBUG__
#define BLUE_STRIP_COLUMNS 1
static const uint32_t PROGMEM BlueStrip[BLUE_STRIP_COLUMNS][TRIANGLE_ROWS] =
    {{0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF}};

#define YELLOW_STRIP_COLUMNS 1
static const uint32_t PROGMEM YellowStrip[YELLOW_STRIP_COLUMNS][TRIANGLE_ROWS] =
    {{0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00}};
#endif

class Kaleidoscope
{
public:
    void setup()
    {
#ifdef __NDEBUG__
        Serial.println("Kaleidoscope.setup");
        strip_1 = BlueStrip;
        strip_1_columns = BLUE_STRIP_COLUMNS;
        rgb_strip_2 = YellowStrip;
        strip_2_columns = YELLOW_STRIP_COLUMNS;
#else
        strip_1 = JewelStrip;
        strip_1_columns = JEWEL_STRIP_COLUMNS;
        rgb_strip_2 = JewelStrip;
        strip_2_columns = JEWEL_STRIP_COLUMNS;
#endif

        // start with random offsets to provide more variety
        current_offset_1 = random(strip_1_columns);
        current_offset_2 = random(strip_2_columns);

#ifdef __NDEBUG__
        // If you uncomment out the Serial.println below, it will corrupt memory and 'we have a problem'
        // I'm not the only one that ran into this:
        // https://stackoverflow.com/questions/38923872/arduino-serial-parseint-data-read-corrupted-by-too-much-serial-print
        for (int y = 0; y < TRIANGLE_ROWS; y++)
        {
            for (int x = 0; x < strip_1_columns; x++)
            {
                uint32_t pixel_1, pixel_2;

                pixel_1 = BlueStrip[x][y];
                pixel_2 = strip_1[x][y];
                Serial.println(pixel_1, HEX);
                Serial.println(pixel_2, HEX);
                if (pixel_1 != pixel_2)
                {
                    Serial.println("Huston, we have a problem");
                    return;
                }
            }
        }
        Serial.println("It's all good");

        Serial.print("BlueStrip = ");
        Serial.println((uint32_t)BlueStrip, HEX);
        Serial.print("strip_1 = ");
        Serial.println((uint32_t)strip_1, HEX);

        Serial.print("BlueStrip[0][0] = 0x");
        Serial.println(BlueStrip[0][0], HEX);

        Serial.print("strip_1[0][0] = 0x");
        Serial.println(strip_1[0][0], HEX);
#endif
    }

    // update the position of the strips and draw the kaleidoscope
    void loop()
    {
        Serial.println("Kaleidoscope.loop");
#ifdef __NDEBUG__
        Serial.print("current_oiffset_1 = ");
        Serial.println(current_offset_1, DEC);
        Serial.println("test_drawKaleidoscopePixel6");
        test_drawKaleidoscopePixel6();
#endif

#ifdef __NDEBUG__
        Serial.print("kaleidoscope.draw(");
        Serial.print(current_offset_1, DEC);
        Serial.print(", ");
        Serial.print(current_offset_2, DEC);
        Serial.println(");");
#endif
        draw(current_offset_1, current_offset_2, 200);

        current_offset_1 = (++current_offset_1 /*+ random(10)*/) % strip_1_columns;
        current_offset_2 = (++current_offset_2 /*+ random(10)*/) % strip_2_columns;
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

private:
    // pointer to two dimensional arrays of color values for kaleidoscope effect
    // https://stackoverflow.com/questions/1052818/create-a-pointer-to-two-dimensional-array
    const uint32_t (*strip_1)[TRIANGLE_ROWS];
    uint8_t strip_1_columns;
    const uint32_t (*rgb_strip_2)[TRIANGLE_ROWS];
    uint8_t strip_2_columns;

    // offset into strips, used to provide movement for the kaleidoscope
    int current_offset_1;
    int current_offset_2;

    // this will draw the kaleidoscope starting at the given offset
    void draw(uint8_t offset_1, uint8_t offset_2, uint8_t wait)
    {
        int begin, end, viewport_index = 0;

        begin = end = offset_1;

#ifdef __NDEBUG__
        Serial.print("offset_1 = ");
        Serial.println((uint32_t)offset_1);

        uint32_t pixel1, pixel2;

        pixel1 = BlueStrip[0][0];
        pixel2 = strip_1[0][0];

        Serial.print("BlueStrip = ");
        Serial.println((uint32_t)BlueStrip, HEX);
        Serial.print("strip_1 = ");
        Serial.println((uint32_t)strip_1, HEX);

        Serial.print("BlueStrip(0, 0) = 0x");
        Serial.println(pixel1, HEX);

        Serial.print("strip_1(0, 0) = 0x");
        Serial.println(pixel2, HEX);
#endif

        // draw the kaleidoscope pixels for this 'frame'
        for (int row = 0; row < TRIANGLE_ROWS; row++)
        {
            for (int x = begin; x <= end; x++)
            {
                int column = x;
                if (column < 0)
                    column += strip_1_columns;
                else if (column >= strip_1_columns)
                    column -= strip_1_columns;
                uint32_t pixel_1 = strip_1[column][row];
#ifdef __NDEBUG__
                Serial.print("pixel_1[");
                Serial.print(column);
                Serial.print("][");
                Serial.print(row);
                Serial.print("] = 0x");
                Serial.println(pixel_1, HEX);
#endif

                column = x + offset_2;
                if (column < 0)
                    column += strip_2_columns;
                else if (column >= strip_2_columns)
                    column -= strip_2_columns;
                uint32_t pixel_2 = rgb_strip_2[column][row];
#ifdef __NDEBUG__
                Serial.print("pixel_2[");
                Serial.print(column);
                Serial.print("][");
                Serial.print(row);
                Serial.print("] = 0x");
                Serial.println(pixel_2, HEX);
#endif

                // blend the pixels from the two strips by doing 50% transparency
                uint32_t pixel = blendAlpha(pixel_1, pixel_2, 0x7f);
#ifdef __NDEBUG__
                Serial.print("blended pixel = 0x");
                Serial.println(pixel, HEX);
#endif
                drawKaleidoscopePixel6(viewport_index, pixel);
                viewport_index++;
            }
            begin--;
            end++;
        }

        for (int x = 0; x < LED_STRIPS; x++)
            LEDs.strip[x].show();
        delay(wait);

        // erase the kaleidoscope pixels
        for (int x = 0; x < LED_STRIPS; x++)
            LEDs.strip[x].clear();
    }

    // draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
    void drawKaleidoscopePixel6(uint16_t index, uint32_t c)
    {
        // The index assumes we itterate through the bottom triange top to bottom, left to right.
        // To keep the LED_strips together, rotate around the hexagon in counter-clockwise order
        switch (index)
        {
        case 0:
            LEDs.strip[1].setPixelColor(11, c);
            LEDs.strip[1].setPixelColor(10, c);
            LEDs.strip[1].setPixelColor(9, c);
            LEDs.strip[0].setPixelColor(134, c);
            LEDs.strip[0].setPixelColor(133, c);
            LEDs.strip[0].setPixelColor(132, c);
            break;
        case 1:
            LEDs.strip[1].setPixelColor(13, c);
            LEDs.strip[1].setPixelColor(37, c);
            LEDs.strip[1].setPixelColor(38, c);
            LEDs.strip[0].setPixelColor(105, c);
            LEDs.strip[0].setPixelColor(106, c);
            LEDs.strip[0].setPixelColor(130, c);
            break;
        case 2:
            LEDs.strip[1].setPixelColor(12, c);
            LEDs.strip[1].setPixelColor(36, c);
            LEDs.strip[1].setPixelColor(8, c);
            LEDs.strip[0].setPixelColor(135, c);
            LEDs.strip[0].setPixelColor(107, c);
            LEDs.strip[0].setPixelColor(131, c);
            break;
        case 3:
            LEDs.strip[1].setPixelColor(34, c);
            LEDs.strip[1].setPixelColor(35, c);
            LEDs.strip[1].setPixelColor(7, c);
            LEDs.strip[0].setPixelColor(136, c);
            LEDs.strip[0].setPixelColor(108, c);
            LEDs.strip[0].setPixelColor(109, c);
            break;
        case 4:
            LEDs.strip[1].setPixelColor(15, c);
            LEDs.strip[1].setPixelColor(58, c);
            LEDs.strip[1].setPixelColor(57, c);
            LEDs.strip[0].setPixelColor(86, c);
            LEDs.strip[0].setPixelColor(85, c);
            LEDs.strip[0].setPixelColor(128, c);
            break;
        case 5:
            LEDs.strip[1].setPixelColor(14, c);
            LEDs.strip[1].setPixelColor(89, c);
            LEDs.strip[1].setPixelColor(39, c);
            LEDs.strip[0].setPixelColor(104, c);
            LEDs.strip[0].setPixelColor(84, c);
            LEDs.strip[0].setPixelColor(129, c);
            break;
        case 6:
            LEDs.strip[1].setPixelColor(32, c);
            LEDs.strip[1].setPixelColor(60, c);
            LEDs.strip[1].setPixelColor(40, c);
            LEDs.strip[0].setPixelColor(103, c);
            LEDs.strip[0].setPixelColor(85, c);
            LEDs.strip[0].setPixelColor(111, c);
            break;
        case 7:
            LEDs.strip[1].setPixelColor(33, c);
            LEDs.strip[1].setPixelColor(61, c);
            LEDs.strip[1].setPixelColor(6, c);
            LEDs.strip[0].setPixelColor(137, c);
            LEDs.strip[0].setPixelColor(82, c);
            LEDs.strip[0].setPixelColor(110, c);
            break;
        case 8:
            LEDs.strip[1].setPixelColor(63, c);
            LEDs.strip[1].setPixelColor(62, c);
            LEDs.strip[1].setPixelColor(5, c);
            LEDs.strip[0].setPixelColor(131, c);
            LEDs.strip[0].setPixelColor(81, c);
            LEDs.strip[0].setPixelColor(80, c);
            break;
        case 9:
            LEDs.strip[1].setPixelColor(17, c);
            LEDs.strip[1].setPixelColor(85, c);
            LEDs.strip[1].setPixelColor(86, c);
            LEDs.strip[0].setPixelColor(57, c);
            LEDs.strip[0].setPixelColor(58, c);
            LEDs.strip[0].setPixelColor(126, c);
            break;
        case 10:
            LEDs.strip[1].setPixelColor(16, c);
            LEDs.strip[1].setPixelColor(84, c);
            LEDs.strip[1].setPixelColor(56, c);
            LEDs.strip[0].setPixelColor(87, c);
            LEDs.strip[0].setPixelColor(59, c);
            LEDs.strip[0].setPixelColor(127, c);
            break;
        case 11:
            LEDs.strip[1].setPixelColor(30, c);
            LEDs.strip[1].setPixelColor(83, c);
            LEDs.strip[1].setPixelColor(55, c);
            LEDs.strip[0].setPixelColor(88, c);
            LEDs.strip[0].setPixelColor(60, c);
            LEDs.strip[0].setPixelColor(113, c);
            break;
        case 12:
            LEDs.strip[1].setPixelColor(31, c);
            LEDs.strip[1].setPixelColor(82, c);
            LEDs.strip[1].setPixelColor(41, c);
            LEDs.strip[0].setPixelColor(102, c);
            LEDs.strip[0].setPixelColor(61, c);
            LEDs.strip[0].setPixelColor(112, c);
            break;
        case 13:
            LEDs.strip[1].setPixelColor(65, c);
            LEDs.strip[1].setPixelColor(81, c);
            LEDs.strip[1].setPixelColor(42, c);
            LEDs.strip[0].setPixelColor(101, c);
            LEDs.strip[0].setPixelColor(62, c);
            LEDs.strip[0].setPixelColor(78, c);
            break;
        case 14:
            LEDs.strip[1].setPixelColor(64, c);
            LEDs.strip[1].setPixelColor(80, c);
            LEDs.strip[1].setPixelColor(4, c);
            LEDs.strip[0].setPixelColor(139, c);
            LEDs.strip[0].setPixelColor(63, c);
            LEDs.strip[0].setPixelColor(79, c);
            break;
        case 15:
            LEDs.strip[1].setPixelColor(78, c);
            LEDs.strip[1].setPixelColor(79, c);
            LEDs.strip[1].setPixelColor(3, c);
            LEDs.strip[0].setPixelColor(140, c);
            LEDs.strip[0].setPixelColor(64, c);
            LEDs.strip[0].setPixelColor(65, c);
            break;
        case 16:
            LEDs.strip[1].setPixelColor(19, c);
            LEDs.strip[1].setPixelColor(98, c);
            LEDs.strip[1].setPixelColor(97, c);
            LEDs.strip[0].setPixelColor(96, c);
            LEDs.strip[0].setPixelColor(95, c);
            LEDs.strip[0].setPixelColor(124, c);
            break;
        case 17:
            LEDs.strip[1].setPixelColor(18, c);
            LEDs.strip[1].setPixelColor(99, c);
            LEDs.strip[1].setPixelColor(87, c);
            LEDs.strip[0].setPixelColor(56, c);
            LEDs.strip[0].setPixelColor(99, c);
            LEDs.strip[0].setPixelColor(125, c);
            break;
        case 18:
            LEDs.strip[1].setPixelColor(28, c);
            LEDs.strip[1].setPixelColor(100, c);
            LEDs.strip[1].setPixelColor(88, c);
            LEDs.strip[0].setPixelColor(55, c);
            LEDs.strip[0].setPixelColor(43, c);
            LEDs.strip[0].setPixelColor(115, c);
            break;
        case 19:
            LEDs.strip[1].setPixelColor(29, c);
            LEDs.strip[1].setPixelColor(101, c);
            LEDs.strip[1].setPixelColor(54, c);
            LEDs.strip[0].setPixelColor(89, c);
            LEDs.strip[0].setPixelColor(42, c);
            LEDs.strip[0].setPixelColor(114, c);
            break;
        case 20:
            LEDs.strip[1].setPixelColor(67, c);
            LEDs.strip[1].setPixelColor(102, c);
            LEDs.strip[1].setPixelColor(53, c);
            LEDs.strip[0].setPixelColor(90, c);
            LEDs.strip[0].setPixelColor(81, c);
            LEDs.strip[0].setPixelColor(76, c);
            break;
        case 21:
            LEDs.strip[1].setPixelColor(66, c);
            LEDs.strip[1].setPixelColor(103, c);
            LEDs.strip[1].setPixelColor(43, c);
            LEDs.strip[0].setPixelColor(100, c);
            LEDs.strip[0].setPixelColor(40, c);
            LEDs.strip[0].setPixelColor(77, c);
            break;
        case 22:
            LEDs.strip[1].setPixelColor(76, c);
            LEDs.strip[1].setPixelColor(104, c);
            LEDs.strip[1].setPixelColor(44, c);
            LEDs.strip[0].setPixelColor(99, c);
            LEDs.strip[0].setPixelColor(39, c);
            LEDs.strip[0].setPixelColor(67, c);
            break;
        case 23:
            LEDs.strip[1].setPixelColor(77, c);
            LEDs.strip[1].setPixelColor(105, c);
            LEDs.strip[1].setPixelColor(2, c);
            LEDs.strip[0].setPixelColor(141, c);
            LEDs.strip[0].setPixelColor(38, c);
            LEDs.strip[0].setPixelColor(66, c);
            break;
        case 24:
            LEDs.strip[1].setPixelColor(107, c);
            LEDs.strip[1].setPixelColor(106, c);
            LEDs.strip[1].setPixelColor(1, c);
            LEDs.strip[0].setPixelColor(142, c);
            LEDs.strip[0].setPixelColor(37, c);
            LEDs.strip[0].setPixelColor(36, c);
            break;
        case 25:
            LEDs.strip[1].setPixelColor(21, c);
            LEDs.strip[1].setPixelColor(125, c);
            LEDs.strip[1].setPixelColor(126, c);
            LEDs.strip[0].setPixelColor(17, c);
            LEDs.strip[0].setPixelColor(18, c);
            LEDs.strip[0].setPixelColor(122, c);
            break;
        case 26:
            LEDs.strip[1].setPixelColor(20, c);
            LEDs.strip[1].setPixelColor(124, c);
            LEDs.strip[1].setPixelColor(96, c);
            LEDs.strip[0].setPixelColor(97, c);
            LEDs.strip[0].setPixelColor(19, c);
            LEDs.strip[0].setPixelColor(123, c);
            break;
        case 27:
            LEDs.strip[1].setPixelColor(26, c);
            LEDs.strip[1].setPixelColor(123, c);
            LEDs.strip[1].setPixelColor(95, c);
            LEDs.strip[0].setPixelColor(48, c);
            LEDs.strip[0].setPixelColor(20, c);
            LEDs.strip[0].setPixelColor(117, c);
            break;
        case 28:
            LEDs.strip[1].setPixelColor(27, c);
            LEDs.strip[1].setPixelColor(122, c);
            LEDs.strip[1].setPixelColor(89, c);
            LEDs.strip[0].setPixelColor(54, c);
            LEDs.strip[0].setPixelColor(21, c);
            LEDs.strip[0].setPixelColor(116, c);
            break;
        case 29:
            LEDs.strip[1].setPixelColor(69, c);
            LEDs.strip[1].setPixelColor(121, c);
            LEDs.strip[1].setPixelColor(90, c);
            LEDs.strip[0].setPixelColor(53, c);
            LEDs.strip[0].setPixelColor(22, c);
            LEDs.strip[0].setPixelColor(74, c);
            break;
        case 30:
            LEDs.strip[1].setPixelColor(68, c);
            LEDs.strip[1].setPixelColor(120, c);
            LEDs.strip[1].setPixelColor(52, c);
            LEDs.strip[0].setPixelColor(91, c);
            LEDs.strip[0].setPixelColor(23, c);
            LEDs.strip[0].setPixelColor(75, c);
            break;
        case 31:
            LEDs.strip[1].setPixelColor(74, c);
            LEDs.strip[1].setPixelColor(119, c);
            LEDs.strip[1].setPixelColor(51, c);
            LEDs.strip[0].setPixelColor(92, c);
            LEDs.strip[0].setPixelColor(24, c);
            LEDs.strip[0].setPixelColor(69, c);
            break;
        case 32:
            LEDs.strip[1].setPixelColor(75, c);
            LEDs.strip[1].setPixelColor(118, c);
            LEDs.strip[1].setPixelColor(45, c);
            LEDs.strip[0].setPixelColor(98, c);
            LEDs.strip[0].setPixelColor(25, c);
            LEDs.strip[0].setPixelColor(68, c);
            break;
        case 33:
            LEDs.strip[1].setPixelColor(109, c);
            LEDs.strip[1].setPixelColor(117, c);
            LEDs.strip[1].setPixelColor(46, c);
            LEDs.strip[0].setPixelColor(97, c);
            LEDs.strip[0].setPixelColor(26, c);
            LEDs.strip[0].setPixelColor(34, c);
            break;
        case 34:
            LEDs.strip[1].setPixelColor(108, c);
            LEDs.strip[1].setPixelColor(116, c);
            LEDs.strip[1].setPixelColor(0, c);
            LEDs.strip[0].setPixelColor(143, c);
            LEDs.strip[0].setPixelColor(27, c);
            LEDs.strip[0].setPixelColor(35, c);
            break;
        case 35:
            LEDs.strip[1].setPixelColor(114, c);
            LEDs.strip[1].setPixelColor(115, c);
            LEDs.strip[0].setPixelColor(149, c);
            LEDs.strip[0].setPixelColor(144, c);
            LEDs.strip[0].setPixelColor(28, c);
            LEDs.strip[0].setPixelColor(29, c);
            break;
        case 36:
            LEDs.strip[1].setPixelColor(23, c);
            LEDs.strip[1].setPixelColor(130, c);
            LEDs.strip[1].setPixelColor(129, c);
            LEDs.strip[0].setPixelColor(14, c);
            LEDs.strip[0].setPixelColor(13, c);
            LEDs.strip[0].setPixelColor(120, c);
            break;
        case 37:
            LEDs.strip[1].setPixelColor(22, c);
            LEDs.strip[1].setPixelColor(131, c);
            LEDs.strip[1].setPixelColor(127, c);
            LEDs.strip[0].setPixelColor(16, c);
            LEDs.strip[0].setPixelColor(12, c);
            LEDs.strip[0].setPixelColor(121, c);
            break;
        case 38:
            LEDs.strip[1].setPixelColor(24, c);
            LEDs.strip[1].setPixelColor(132, c);
            LEDs.strip[1].setPixelColor(128, c);
            LEDs.strip[0].setPixelColor(15, c);
            LEDs.strip[0].setPixelColor(11, c);
            LEDs.strip[0].setPixelColor(119, c);
            break;
        case 39:
            LEDs.strip[1].setPixelColor(25, c);
            LEDs.strip[1].setPixelColor(133, c);
            LEDs.strip[1].setPixelColor(94, c);
            LEDs.strip[0].setPixelColor(47, c);
            LEDs.strip[0].setPixelColor(10, c);
            LEDs.strip[0].setPixelColor(118, c);
            break;
        case 40:
            LEDs.strip[1].setPixelColor(71, c);
            LEDs.strip[1].setPixelColor(134, c);
            LEDs.strip[1].setPixelColor(93, c);
            LEDs.strip[0].setPixelColor(50, c);
            LEDs.strip[0].setPixelColor(9, c);
            LEDs.strip[0].setPixelColor(72, c);
            break;
        case 41:
            LEDs.strip[1].setPixelColor(70, c);
            LEDs.strip[1].setPixelColor(135, c);
            LEDs.strip[1].setPixelColor(91, c);
            LEDs.strip[0].setPixelColor(52, c);
            LEDs.strip[0].setPixelColor(8, c);
            LEDs.strip[0].setPixelColor(73, c);
            break;
        case 42:
            LEDs.strip[1].setPixelColor(72, c);
            LEDs.strip[1].setPixelColor(136, c);
            LEDs.strip[1].setPixelColor(92, c);
            LEDs.strip[0].setPixelColor(51, c);
            LEDs.strip[0].setPixelColor(7, c);
            LEDs.strip[0].setPixelColor(70, c);
            break;
        case 43:
            LEDs.strip[1].setPixelColor(73, c);
            LEDs.strip[1].setPixelColor(137, c);
            LEDs.strip[1].setPixelColor(50, c);
            LEDs.strip[0].setPixelColor(93, c);
            LEDs.strip[0].setPixelColor(6, c);
            LEDs.strip[0].setPixelColor(70, c);
            break;
        case 44:
            LEDs.strip[1].setPixelColor(111, c);
            LEDs.strip[1].setPixelColor(138, c);
            LEDs.strip[1].setPixelColor(49, c);
            LEDs.strip[0].setPixelColor(94, c);
            LEDs.strip[0].setPixelColor(5, c);
            LEDs.strip[0].setPixelColor(32, c);
            break;
        case 45:
            LEDs.strip[1].setPixelColor(110, c);
            LEDs.strip[1].setPixelColor(139, c);
            LEDs.strip[1].setPixelColor(47, c);
            LEDs.strip[0].setPixelColor(96, c);
            LEDs.strip[0].setPixelColor(4, c);
            LEDs.strip[0].setPixelColor(33, c);
            break;
        case 46:
            LEDs.strip[1].setPixelColor(112, c);
            LEDs.strip[1].setPixelColor(140, c);
            LEDs.strip[1].setPixelColor(48, c);
            LEDs.strip[0].setPixelColor(95, c);
            LEDs.strip[0].setPixelColor(3, c);
            LEDs.strip[0].setPixelColor(31, c);
            break;
        case 47:
            LEDs.strip[1].setPixelColor(43, c);
            LEDs.strip[1].setPixelColor(181, c);
            LEDs.strip[0].setPixelColor(148, c);
            LEDs.strip[0].setPixelColor(145, c);
            LEDs.strip[0].setPixelColor(2, c);
            LEDs.strip[0].setPixelColor(30, c);
            break;
        case 48:
            LEDs.strip[1].setPixelColor(143, c);
            LEDs.strip[1].setPixelColor(142, c);
            LEDs.strip[0].setPixelColor(147, c);
            LEDs.strip[0].setPixelColor(146, c);
            LEDs.strip[0].setPixelColor(1, c);
            LEDs.strip[0].setPixelColor(0, c);
            break;
        }
    }

#ifdef __NDEBUG__
    void test_drawKaleidoscopePixel6()
    {
        // loop through all pixels in the source triange making sure they
        // get reflected and mirrored properly
        for (int index = 0; index < TRIANGLE_COUNT; index++)
        {
            // turn on the pixels
            drawKaleidoscopePixel6(index, LEDs.strip[0].Color(255, 0, 0)); // Red

            for (uint8_t x = 0; x < LED_STRIPS; x++)
                LEDs.strip[x].show();
            delay(1000);

            // turn off the pixels
            drawKaleidoscopePixel6(index, 0);
        }
    }
#endif
};

#endif // KALEIDOSCOPE_H
