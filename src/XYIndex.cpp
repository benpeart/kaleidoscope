#include "main.h"
#include "XYIndex.h"
#include "Kaleidoscope.h"

// Params for width and height
const uint8_t kMatrixWidth = 20;
const uint8_t kMatrixHeight = 39;

const uint8_t Strip0_kMatrixWidth = 6;
const uint8_t Strip1_kMatrixWidth = 4;
const uint8_t Strip2_kMatrixWidth = 4;
const uint8_t Strip3_kMatrixWidth = 6;

uint16_t XYToIndex(uint8_t x, uint8_t y)
{
  // any out of bounds address maps to the first hidden pixel
  if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
    return NUM_LEDS_PER_STRIP * 2 - 1;

  const uint8_t XYTable[] = {
    // 
    // This array shows how the LED strips are laid out. There are 4 separate strips arranged 
    // from left to right. The left half and right half are mirrored to minimize the size of the lookup
    // tables (see drawPixel6LookupTable).
    //
    // TODO: fix XYTable by removing the right half of the columns and updating the code to compensate
    //
      255, 255, 255, 255, 255, 255, 255, 255, 255, 38, 38, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 39, 37, 37, 39, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 110, 40, 36, 36, 40, 110, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 111, 109, 41, 35, 35, 41, 109, 111, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 30, 112, 108, 42, 34, 34, 42, 108, 112, 30, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 31, 29, 113, 107, 43, 33, 33, 43, 107, 113, 29, 31, 255, 255, 255, 255,
      255, 255, 255, 86, 32, 28, 114, 106, 44, 32, 32, 44, 106, 114, 28, 32, 86, 255, 255, 255,
      255, 255, 87, 85, 33, 27, 115, 105, 45, 31, 31, 45, 105, 115, 27, 33, 85, 87, 255, 255,
      255, 134, 88, 84, 34, 26, 116, 104, 46, 30, 30, 46, 104, 116, 26, 34, 84, 88, 134, 255,
      135, 133, 89, 83, 35, 25, 117, 103, 47, 29, 29, 47, 103, 117, 25, 35, 83, 89, 133, 135,
      136, 132, 90, 82, 36, 24, 118, 102, 48, 28, 28, 48, 102, 118, 24, 36, 82, 90, 132, 136,
      137, 131, 91, 81, 37, 23, 119, 101, 49, 27, 27, 49, 101, 119, 23, 37, 81, 91, 131, 137,
      138, 130, 92, 80, 38, 22, 120, 100, 50, 26, 26, 50, 100, 120, 22, 38, 80, 92, 130, 138,
      139, 129, 93, 79, 39, 21, 121, 99, 51, 25, 25, 51, 99, 121, 21, 39, 79, 93, 129, 139,
      140, 128, 94, 78, 40, 20, 122, 98, 52, 24, 24, 52, 98, 122, 20, 40, 78, 94, 128, 140,
      141, 127, 95, 77, 41, 19, 123, 97, 53, 23, 23, 53, 97, 123, 19, 41, 77, 95, 127, 141,
      142, 126, 96, 76, 42, 18, 124, 96, 54, 22, 22, 54, 96, 124, 18, 42, 76, 96, 126, 142,
      143, 125, 97, 75, 43, 17, 125, 95, 55, 21, 21, 55, 95, 125, 17, 43, 75, 97, 125, 143,
      144, 124, 98, 74, 44, 16, 126, 94, 56, 20, 20, 56, 94, 126, 16, 44, 74, 98, 124, 144,
      145, 123, 99, 73, 45, 15, 127, 93, 57, 19, 19, 57, 93, 127, 15, 45, 73, 99, 123, 145,
      146, 122, 100, 72, 46, 14, 128, 92, 58, 18, 18, 58, 92, 128, 14, 46, 72, 100, 122, 146,
      147, 121, 101, 71, 47, 13, 129, 91, 59, 17, 17, 59, 91, 129, 13, 47, 71, 101, 121, 147,
      148, 120, 102, 70, 48, 12, 130, 90, 60, 16, 16, 60, 90, 130, 12, 48, 70, 102, 120, 148,
      149, 119, 103, 69, 49, 11, 131, 89, 61, 15, 15, 61, 89, 131, 11, 49, 69, 103, 119, 149,
      150, 118, 104, 68, 50, 10, 132, 88, 62, 14, 14, 62, 88, 132, 10, 50, 68, 104, 118, 150,
      151, 117, 105, 67, 51, 9, 133, 87, 63, 13, 13, 63, 87, 133, 9, 51, 67, 105, 117, 151,
      152, 116, 106, 66, 52, 8, 134, 86, 64, 12, 12, 64, 86, 134, 8, 52, 66, 106, 116, 152,
      153, 115, 107, 65, 53, 7, 135, 85, 65, 11, 11, 65, 85, 135, 7, 53, 65, 107, 115, 153,
      154, 114, 108, 64, 54, 6, 136, 84, 66, 10, 10, 66, 84, 136, 6, 54, 64, 108, 114, 154,
      155, 113, 109, 63, 55, 5, 137, 83, 67, 9, 9, 67, 83, 137, 5, 55, 63, 109, 113, 155,
      255, 112, 110, 62, 56, 4, 138, 82, 68, 8, 8, 68, 82, 138, 4, 56, 62, 110, 112, 255,
      255, 255, 111, 61, 57, 3, 139, 81, 69, 7, 7, 69, 81, 139, 3, 57, 61, 111, 255, 255,
      255, 255, 255, 60, 58, 2, 140, 80, 70, 6, 6, 70, 80, 140, 2, 58, 60, 255, 255, 255,
      255, 255, 255, 255, 59, 1, 141, 79, 71, 5, 5, 71, 79, 141, 1, 59, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 0, 142, 78, 72, 4, 4, 72, 78, 142, 0, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 143, 77, 73, 3, 3, 73, 77, 143, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 76, 74, 2, 2, 74, 76, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 75, 1, 1, 75, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255};

  uint16_t i = (y * kMatrixWidth) + x;
  uint16_t j = XYTable[i];
  uint8_t strip = 0;

  // if we are returning an out of bounds value for an individual strip, return the first hidden LED instead
  // to make error handling easier.
  if (j > NUM_LEDS_PER_STRIP)
    return NUM_LEDS_PER_STRIP * 2 - 1;

  if (x < Strip0_kMatrixWidth)
  {
    strip = 0;
  }
  else if (x < Strip0_kMatrixWidth + Strip1_kMatrixWidth)
  {
    strip = 1;
  }
  else if (x < Strip0_kMatrixWidth + Strip1_kMatrixWidth + Strip2_kMatrixWidth)
  {
    strip = 2;
  }
  else if (x < Strip0_kMatrixWidth + Strip1_kMatrixWidth + Strip2_kMatrixWidth + Strip3_kMatrixWidth)
  {
    strip = 3;
  }

  j += strip * NUM_LEDS_PER_STRIP;

  return j;
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
    index = XYToIndex(x, y);
    leds[index] = CRGB::Black; // off

    // move to the next pixel
    if (++x >= kMatrixWidth)
    {
      x = 0;
      if (++y >= kMatrixHeight)
        y = 0;
    }
    index = XYToIndex(x, y);

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
