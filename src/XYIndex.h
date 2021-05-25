#include <Arduino.h>

// The width and height of the XY coordinate system. The corners outside the hexagon
// are 'missing' so can't display any values assigned to them.
#define XY_ROWS 39
#define XY_COLS 20

// function to map a x, y coordinate to the index into the led array
uint16_t XYIndex(uint8_t x, uint8_t y);

#ifdef DEBUG
// loop through all pixels via x,y coordinates making sure they all get mapped correctly
void mode_xy_test();
#endif
