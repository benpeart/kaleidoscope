#include "main.h"
#include "XY.h"
#include "Kaleidoscope.h"

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
