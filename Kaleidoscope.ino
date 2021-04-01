#include <Adafruit_NeoPixel.h>

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

#define PIN_BASE 6
#define LED_STRIPS 2
#define PIXELS_PER_STRIP 150

Adafruit_NeoPixel LED_strip[LED_STRIPS];

// statically define the 'filmstrips' to use to generate the kaleidoscope
#define TRIANGLE_COLUMNS 7    // the height of the 'viewport' triangle
#define TRIANGLE_ROWS 13      // the width of the base of the 'viewport' triange
#define TRIANGLE_COUNT 49     // the number of pixels in the 'viewport' triangle
#define JEWEL_STRIP_ROWS 28
#define JEWEL_RED 0x7b1542
#define JEWEL_GREEN 0x249b23
#define JEWEL_BLUE 0x1c5dd2
#define JEWEL_ORANGE 0xd97217
#define JEWEL_YELLOW 0xe5bb1c
#define JEWEL_PURPLE 0x8a50b7
#define JEWEL_LEAD 0x27100a

// store the strip RGB values in program memory (flash) to save SRAM
static const uint32_t PROGMEM OldJewelStrip[TRIANGLE_COLUMNS][JEWEL_STRIP_ROWS] =
    {
        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD},
        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD},

        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD},
        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD},
        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD},

        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_LEAD},
        {JEWEL_RED, JEWEL_RED, JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_BLUE, JEWEL_BLUE, JEWEL_LEAD,
         JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_GREEN, JEWEL_LEAD,
         JEWEL_PURPLE, JEWEL_PURPLE, JEWEL_RED, JEWEL_RED, JEWEL_YELLOW, JEWEL_YELLOW, JEWEL_LEAD,
         JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_ORANGE, JEWEL_LEAD}};

static const uint32_t PROGMEM JewelStrip[JEWEL_STRIP_ROWS][TRIANGLE_COLUMNS] =
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



// draw a pixel mirrored and rotated 6 times to emulate a kaleidoscope
void drawKaleidoscopePixel6(uint16_t index, uint32_t c)
{
  // The index assumes we itterate through the bottom triange top to bottom, left to right.
  // To keep the LED_strips together, rotate around the hexagon in counter-clockwise order
  switch (index)
  {
  case 0:
    LED_strip[1].setPixelColor(11, c);
    LED_strip[1].setPixelColor(10, c);
    LED_strip[1].setPixelColor(9, c);
    LED_strip[0].setPixelColor(134, c);
    LED_strip[0].setPixelColor(133, c);
    LED_strip[0].setPixelColor(132, c);
    break;
  case 1:
    LED_strip[1].setPixelColor(13, c);
    LED_strip[1].setPixelColor(37, c);
    LED_strip[1].setPixelColor(38, c);
    LED_strip[0].setPixelColor(105, c);
    LED_strip[0].setPixelColor(106, c);
    LED_strip[0].setPixelColor(130, c);
    break;
  case 2:
    LED_strip[1].setPixelColor(12, c);
    LED_strip[1].setPixelColor(36, c);
    LED_strip[1].setPixelColor(8, c);
    LED_strip[0].setPixelColor(135, c);
    LED_strip[0].setPixelColor(107, c);
    LED_strip[0].setPixelColor(131, c);
    break;
  case 3:
    LED_strip[1].setPixelColor(34, c);
    LED_strip[1].setPixelColor(35, c);
    LED_strip[1].setPixelColor(7, c);
    LED_strip[0].setPixelColor(136, c);
    LED_strip[0].setPixelColor(108, c);
    LED_strip[0].setPixelColor(109, c);
    break;
  case 4:
    LED_strip[1].setPixelColor(15, c);
    LED_strip[1].setPixelColor(58, c);
    LED_strip[1].setPixelColor(57, c);
    LED_strip[0].setPixelColor(86, c);
    LED_strip[0].setPixelColor(85, c);
    LED_strip[0].setPixelColor(128, c);
    break;
  case 5:
    LED_strip[1].setPixelColor(14, c);
    LED_strip[1].setPixelColor(89, c);
    LED_strip[1].setPixelColor(39, c);
    LED_strip[0].setPixelColor(104, c);
    LED_strip[0].setPixelColor(84, c);
    LED_strip[0].setPixelColor(129, c);
    break;
  case 6:
    LED_strip[1].setPixelColor(32, c);
    LED_strip[1].setPixelColor(60, c);
    LED_strip[1].setPixelColor(40, c);
    LED_strip[0].setPixelColor(103, c);
    LED_strip[0].setPixelColor(85, c);
    LED_strip[0].setPixelColor(111, c);
    break;
  case 7:
    LED_strip[1].setPixelColor(33, c);
    LED_strip[1].setPixelColor(61, c);
    LED_strip[1].setPixelColor(6, c);
    LED_strip[0].setPixelColor(137, c);
    LED_strip[0].setPixelColor(82, c);
    LED_strip[0].setPixelColor(110, c);
    break;
  case 8:
    LED_strip[1].setPixelColor(63, c);
    LED_strip[1].setPixelColor(62, c);
    LED_strip[1].setPixelColor(5, c);
    LED_strip[0].setPixelColor(131, c);
    LED_strip[0].setPixelColor(81, c);
    LED_strip[0].setPixelColor(80, c);
    break;
  case 9:
    LED_strip[1].setPixelColor(17, c);
    LED_strip[1].setPixelColor(85, c);
    LED_strip[1].setPixelColor(86, c);
    LED_strip[0].setPixelColor(57, c);
    LED_strip[0].setPixelColor(58, c);
    LED_strip[0].setPixelColor(126, c);
    break;
  case 10:
    LED_strip[1].setPixelColor(16, c);
    LED_strip[1].setPixelColor(84, c);
    LED_strip[1].setPixelColor(56, c);
    LED_strip[0].setPixelColor(87, c);
    LED_strip[0].setPixelColor(59, c);
    LED_strip[0].setPixelColor(127, c);
    break;
  case 11:
    LED_strip[1].setPixelColor(30, c);
    LED_strip[1].setPixelColor(83, c);
    LED_strip[1].setPixelColor(55, c);
    LED_strip[0].setPixelColor(88, c);
    LED_strip[0].setPixelColor(60, c);
    LED_strip[0].setPixelColor(113, c);
    break;
  case 12:
    LED_strip[1].setPixelColor(31, c);
    LED_strip[1].setPixelColor(82, c);
    LED_strip[1].setPixelColor(41, c);
    LED_strip[0].setPixelColor(102, c);
    LED_strip[0].setPixelColor(61, c);
    LED_strip[0].setPixelColor(112, c);
    break;
  case 13:
    LED_strip[1].setPixelColor(65, c);
    LED_strip[1].setPixelColor(81, c);
    LED_strip[1].setPixelColor(42, c);
    LED_strip[0].setPixelColor(101, c);
    LED_strip[0].setPixelColor(62, c);
    LED_strip[0].setPixelColor(78, c);
    break;
  case 14:
    LED_strip[1].setPixelColor(64, c);
    LED_strip[1].setPixelColor(80, c);
    LED_strip[1].setPixelColor(4, c);
    LED_strip[0].setPixelColor(139, c);
    LED_strip[0].setPixelColor(63, c);
    LED_strip[0].setPixelColor(79, c);
    break;
  case 15:
    LED_strip[1].setPixelColor(78, c);
    LED_strip[1].setPixelColor(79, c);
    LED_strip[1].setPixelColor(3, c);
    LED_strip[0].setPixelColor(140, c);
    LED_strip[0].setPixelColor(64, c);
    LED_strip[0].setPixelColor(65, c);
    break;
  case 16:
    LED_strip[1].setPixelColor(19, c);
    LED_strip[1].setPixelColor(98, c);
    LED_strip[1].setPixelColor(97, c);
    LED_strip[0].setPixelColor(96, c);
    LED_strip[0].setPixelColor(95, c);
    LED_strip[0].setPixelColor(124, c);
    break;
  case 17:
    LED_strip[1].setPixelColor(18, c);
    LED_strip[1].setPixelColor(99, c);
    LED_strip[1].setPixelColor(87, c);
    LED_strip[0].setPixelColor(56, c);
    LED_strip[0].setPixelColor(99, c);
    LED_strip[0].setPixelColor(125, c);
    break;
  case 18:
    LED_strip[1].setPixelColor(28, c);
    LED_strip[1].setPixelColor(100, c);
    LED_strip[1].setPixelColor(88, c);
    LED_strip[0].setPixelColor(55, c);
    LED_strip[0].setPixelColor(43, c);
    LED_strip[0].setPixelColor(115, c);
    break;
  case 19:
    LED_strip[1].setPixelColor(29, c);
    LED_strip[1].setPixelColor(101, c);
    LED_strip[1].setPixelColor(54, c);
    LED_strip[0].setPixelColor(89, c);
    LED_strip[0].setPixelColor(42, c);
    LED_strip[0].setPixelColor(114, c);
    break;
  case 20:
    LED_strip[1].setPixelColor(67, c);
    LED_strip[1].setPixelColor(102, c);
    LED_strip[1].setPixelColor(53, c);
    LED_strip[0].setPixelColor(90, c);
    LED_strip[0].setPixelColor(81, c);
    LED_strip[0].setPixelColor(76, c);
    break;
  case 21:
    LED_strip[1].setPixelColor(66, c);
    LED_strip[1].setPixelColor(103, c);
    LED_strip[1].setPixelColor(43, c);
    LED_strip[0].setPixelColor(100, c);
    LED_strip[0].setPixelColor(40, c);
    LED_strip[0].setPixelColor(77, c);
    break;
  case 22:
    LED_strip[1].setPixelColor(76, c);
    LED_strip[1].setPixelColor(104, c);
    LED_strip[1].setPixelColor(44, c);
    LED_strip[0].setPixelColor(99, c);
    LED_strip[0].setPixelColor(39, c);
    LED_strip[0].setPixelColor(67, c);
    break;
  case 23:
    LED_strip[1].setPixelColor(77, c);
    LED_strip[1].setPixelColor(105, c);
    LED_strip[1].setPixelColor(2, c);
    LED_strip[0].setPixelColor(141, c);
    LED_strip[0].setPixelColor(38, c);
    LED_strip[0].setPixelColor(66, c);
    break;
  case 24:
    LED_strip[1].setPixelColor(107, c);
    LED_strip[1].setPixelColor(106, c);
    LED_strip[1].setPixelColor(1, c);
    LED_strip[0].setPixelColor(142, c);
    LED_strip[0].setPixelColor(37, c);
    LED_strip[0].setPixelColor(36, c);
    break;
  case 25:
    LED_strip[1].setPixelColor(21, c);
    LED_strip[1].setPixelColor(125, c);
    LED_strip[1].setPixelColor(126, c);
    LED_strip[0].setPixelColor(17, c);
    LED_strip[0].setPixelColor(18, c);
    LED_strip[0].setPixelColor(122, c);
    break;
  case 26:
    LED_strip[1].setPixelColor(20, c);
    LED_strip[1].setPixelColor(124, c);
    LED_strip[1].setPixelColor(96, c);
    LED_strip[0].setPixelColor(97, c);
    LED_strip[0].setPixelColor(19, c);
    LED_strip[0].setPixelColor(123, c);
    break;
  case 27:
    LED_strip[1].setPixelColor(26, c);
    LED_strip[1].setPixelColor(123, c);
    LED_strip[1].setPixelColor(95, c);
    LED_strip[0].setPixelColor(48, c);
    LED_strip[0].setPixelColor(20, c);
    LED_strip[0].setPixelColor(117, c);
    break;
  case 28:
    LED_strip[1].setPixelColor(27, c);
    LED_strip[1].setPixelColor(122, c);
    LED_strip[1].setPixelColor(89, c);
    LED_strip[0].setPixelColor(54, c);
    LED_strip[0].setPixelColor(21, c);
    LED_strip[0].setPixelColor(116, c);
    break;
  case 29:
    LED_strip[1].setPixelColor(69, c);
    LED_strip[1].setPixelColor(121, c);
    LED_strip[1].setPixelColor(90, c);
    LED_strip[0].setPixelColor(53, c);
    LED_strip[0].setPixelColor(22, c);
    LED_strip[0].setPixelColor(74, c);
    break;
  case 30:
    LED_strip[1].setPixelColor(68, c);
    LED_strip[1].setPixelColor(120, c);
    LED_strip[1].setPixelColor(52, c);
    LED_strip[0].setPixelColor(91, c);
    LED_strip[0].setPixelColor(23, c);
    LED_strip[0].setPixelColor(75, c);
    break;
  case 31:
    LED_strip[1].setPixelColor(74, c);
    LED_strip[1].setPixelColor(119, c);
    LED_strip[1].setPixelColor(51, c);
    LED_strip[0].setPixelColor(92, c);
    LED_strip[0].setPixelColor(24, c);
    LED_strip[0].setPixelColor(69, c);
    break;
  case 32:
    LED_strip[1].setPixelColor(75, c);
    LED_strip[1].setPixelColor(118, c);
    LED_strip[1].setPixelColor(45, c);
    LED_strip[0].setPixelColor(98, c);
    LED_strip[0].setPixelColor(25, c);
    LED_strip[0].setPixelColor(68, c);
    break;
  case 33:
    LED_strip[1].setPixelColor(109, c);
    LED_strip[1].setPixelColor(117, c);
    LED_strip[1].setPixelColor(46, c);
    LED_strip[0].setPixelColor(97, c);
    LED_strip[0].setPixelColor(26, c);
    LED_strip[0].setPixelColor(34, c);
    break;
  case 34:
    LED_strip[1].setPixelColor(108, c);
    LED_strip[1].setPixelColor(116, c);
    LED_strip[1].setPixelColor(0, c);
    LED_strip[0].setPixelColor(143, c);
    LED_strip[0].setPixelColor(27, c);
    LED_strip[0].setPixelColor(35, c);
    break;
  case 35:
    LED_strip[1].setPixelColor(114, c);
    LED_strip[1].setPixelColor(115, c);
    LED_strip[0].setPixelColor(149, c);
    LED_strip[0].setPixelColor(144, c);
    LED_strip[0].setPixelColor(28, c);
    LED_strip[0].setPixelColor(29, c);
    break;
  case 36:
    LED_strip[1].setPixelColor(23, c);
    LED_strip[1].setPixelColor(130, c);
    LED_strip[1].setPixelColor(129, c);
    LED_strip[0].setPixelColor(14, c);
    LED_strip[0].setPixelColor(13, c);
    LED_strip[0].setPixelColor(120, c);
    break;
  case 37:
    LED_strip[1].setPixelColor(22, c);
    LED_strip[1].setPixelColor(131, c);
    LED_strip[1].setPixelColor(127, c);
    LED_strip[0].setPixelColor(16, c);
    LED_strip[0].setPixelColor(12, c);
    LED_strip[0].setPixelColor(121, c);
    break;
  case 38:
    LED_strip[1].setPixelColor(24, c);
    LED_strip[1].setPixelColor(132, c);
    LED_strip[1].setPixelColor(128, c);
    LED_strip[0].setPixelColor(15, c);
    LED_strip[0].setPixelColor(11, c);
    LED_strip[0].setPixelColor(119, c);
    break;
  case 39:
    LED_strip[1].setPixelColor(25, c);
    LED_strip[1].setPixelColor(133, c);
    LED_strip[1].setPixelColor(94, c);
    LED_strip[0].setPixelColor(47, c);
    LED_strip[0].setPixelColor(10, c);
    LED_strip[0].setPixelColor(118, c);
    break;
  case 40:
    LED_strip[1].setPixelColor(71, c);
    LED_strip[1].setPixelColor(134, c);
    LED_strip[1].setPixelColor(93, c);
    LED_strip[0].setPixelColor(50, c);
    LED_strip[0].setPixelColor(9, c);
    LED_strip[0].setPixelColor(72, c);
    break;
  case 41:
    LED_strip[1].setPixelColor(70, c);
    LED_strip[1].setPixelColor(135, c);
    LED_strip[1].setPixelColor(91, c);
    LED_strip[0].setPixelColor(52, c);
    LED_strip[0].setPixelColor(8, c);
    LED_strip[0].setPixelColor(73, c);
    break;
  case 42:
    LED_strip[1].setPixelColor(72, c);
    LED_strip[1].setPixelColor(136, c);
    LED_strip[1].setPixelColor(92, c);
    LED_strip[0].setPixelColor(51, c);
    LED_strip[0].setPixelColor(7, c);
    LED_strip[0].setPixelColor(70, c);
    break;
  case 43:
    LED_strip[1].setPixelColor(73, c);
    LED_strip[1].setPixelColor(137, c);
    LED_strip[1].setPixelColor(50, c);
    LED_strip[0].setPixelColor(93, c);
    LED_strip[0].setPixelColor(6, c);
    LED_strip[0].setPixelColor(70, c);
    break;
  case 44:
    LED_strip[1].setPixelColor(111, c);
    LED_strip[1].setPixelColor(138, c);
    LED_strip[1].setPixelColor(49, c);
    LED_strip[0].setPixelColor(94, c);
    LED_strip[0].setPixelColor(5, c);
    LED_strip[0].setPixelColor(32, c);
    break;
  case 45:
    LED_strip[1].setPixelColor(110, c);
    LED_strip[1].setPixelColor(139, c);
    LED_strip[1].setPixelColor(47, c);
    LED_strip[0].setPixelColor(96, c);
    LED_strip[0].setPixelColor(4, c);
    LED_strip[0].setPixelColor(33, c);
    break;
  case 46:
    LED_strip[1].setPixelColor(112, c);
    LED_strip[1].setPixelColor(140, c);
    LED_strip[1].setPixelColor(48, c);
    LED_strip[0].setPixelColor(95, c);
    LED_strip[0].setPixelColor(3, c);
    LED_strip[0].setPixelColor(31, c);
    break;
  case 47:
    LED_strip[1].setPixelColor(43, c);
    LED_strip[1].setPixelColor(181, c);
    LED_strip[0].setPixelColor(148, c);
    LED_strip[0].setPixelColor(145, c);
    LED_strip[0].setPixelColor(2, c);
    LED_strip[0].setPixelColor(30, c);
    break;
  case 48:
    LED_strip[1].setPixelColor(143, c);
    LED_strip[1].setPixelColor(142, c);
    LED_strip[0].setPixelColor(147, c);
    LED_strip[0].setPixelColor(146, c);
    LED_strip[0].setPixelColor(1, c);
    LED_strip[0].setPixelColor(0, c);
    break;
  }
}

void test_drawKaleidoscopePixel6()
{
  // loop through all pixels in the source triange making sure they
  // get reflected and mirrored properly
  for (int index = 0; index < TRIANGLE_COUNT; index++)
  {
    // turn on the pixels
    drawKaleidoscopePixel6(index, LED_strip[0].Color(255, 0, 0)); // Red

    for (uint8_t x = 0; x < LED_STRIPS; x++)
      LED_strip[x].show();
    delay(500);

    // turn off the pixels
    drawKaleidoscopePixel6(index, 0); // Red
  }
}

// draw a pixel mirrored and rotated 12 times to emulate a kaleidoscope
void drawKaleidoscopePixel12(uint16_t index, uint32_t c)
{
  switch (index)
  {
  case 1:
    LED_strip[0].setPixelColor(110, c);
    LED_strip[0].setPixelColor(111, c);
    LED_strip[0].setPixelColor(112, c);
    LED_strip[1].setPixelColor(22, c);
    LED_strip[1].setPixelColor(23, c);
    LED_strip[1].setPixelColor(24, c);
    break;
  case 2:
    LED_strip[0].setPixelColor(7, c);
    break;
  case 3:
    break;
  }
}


struct Kaleidoscope
{
  Kaleidoscope(const uint32_t strip[][TRIANGLE_COLUMNS], int rows)
  {
    rgb_strip = strip;
    total_rows = rows;
    current_row = 0;
  }

  // this will draw the kaleidoscope starting at the given offset
  void draw(uint8_t offset, uint8_t wait)
  {
    int begin = offset;
    int end = offset + TRIANGLE_ROWS;
    int counter = 0;

    for (int x = 0; x < TRIANGLE_COLUMNS; x++)
    {
      for (int y = begin; y < end; y++)
      {
        drawKaleidoscopePixel6(counter, rgb_strip[y % total_rows][x]);
        counter++;
      }
      begin++;
      end--;
    }

    for (int x = 0; x < LED_STRIPS; x++)
      LED_strip[x].show();
    delay(wait);
  }

  const uint32_t (*rgb_strip)[TRIANGLE_COLUMNS];
  uint8_t current_row, total_rows;
};

Kaleidoscope kaleidoscope(JewelStrip, JEWEL_STRIP_ROWS);

void setup()
{
  for (int x = 0; x < LED_STRIPS; x++)
  {
    // Parameter 1 = number of pixels in strip
    // Parameter 2 = Arduino pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
    LED_strip[x] = Adafruit_NeoPixel(PIXELS_PER_STRIP, PIN_BASE + x, NEO_GRB + NEO_KHZ800);
    LED_strip[x].begin();
    LED_strip[x].clear();
    LED_strip[x].setBrightness(50);
    LED_strip[x].show(); // Initialize all pixels to 'off'
  }
}

void loop()
{
#if 0
  static int current_offset = 0;

  test_drawKaleidoscopePixel6();

  kaleidoscope.draw(current_offset++, 100);
  current_offset = current_offset % kaleidoscope.total_rows;
#else
  // why can't it display white?
  LED_strip[0].fill(0xffffffff, 0, 16);
  LED_strip[0].show();
  delay(5000);

  // Some example procedures showing how to display to the pixels:
  for (long x = 0; x < 65535; x += 100)
  {
    LED_strip[0].fill(LED_strip[0].ColorHSV(x), 0, 16);
    LED_strip[0].show();
    delay(50);
  }

#if 0
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*0/6)), 50); // Red
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*1/6)), 50); // Yellow
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*2/6)), 50); // Green
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*3/6)), 50); // Cyan
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*4/6)), 50); // Blue
  colorWipe(LED_strip[0].gamma32(LED_strip[0].ColorHSV(65536*5/6)), 50); // Magenta

  colorWipe(LED_strip[0].Color(255, 0, 0), 50); // Red
  colorWipe(LED_strip[0].Color(0, 255, 0), 50); // Green
  colorWipe(LED_strip[0].Color(0, 0, 255), 50); // Blue

  // Send a theater pixel chase in...
  theaterChase(LED_strip[0].Color(127, 127, 127), 50); // White
  theaterChase(LED_strip[0].Color(127, 0, 0), 50);     // Red
  theaterChase(LED_strip[0].Color(0, 0, 127), 50);     // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
#endif
#endif
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < LED_strip[0].numPixels(); i++)
  {
    LED_strip[0].setPixelColor(i, c);
    LED_strip[0].show();
    delay(wait);
  }
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < LED_strip[0].numPixels(); i++)
    {
      LED_strip[0].setPixelColor(i, Wheel((i + j) & 255));
    }
    LED_strip[0].show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  // 5 cycles of all colors on wheel
  for (j = 0; j < 256 * 5; j++)
  {
    for (i = 0; i < LED_strip[0].numPixels(); i++)
    {
      LED_strip[0].setPixelColor(i, Wheel(((i * 256 / LED_strip[0].numPixels()) + j) & 255));
    }
    LED_strip[0].show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
  // do 10 cycles of chasing
  for (int j = 0; j < 10; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < LED_strip[0].numPixels(); i = i + 3)
      {
        LED_strip[0].setPixelColor(i + q, c);
      }
      LED_strip[0].show();

      delay(wait);

      // turn every third pixel off
      for (uint16_t i = 0; i < LED_strip[0].numPixels(); i = i + 3)
      {
        LED_strip[0].setPixelColor(i + q, 0);
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
  // cycle all 256 colors in the wheel
  for (int j = 0; j < 256; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < LED_strip[0].numPixels(); i = i + 3)
      {
        LED_strip[0].setPixelColor(i + q, Wheel((i + j) % 255));
      }
      LED_strip[0].show();

      delay(wait);

      // turn every third pixel off
      for (uint16_t i = 0; i < LED_strip[0].numPixels(); i = i + 3)
      {
        LED_strip[0].setPixelColor(i + q, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return LED_strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return LED_strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return LED_strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

