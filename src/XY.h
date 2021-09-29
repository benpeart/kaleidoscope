// The width and height of the XY coordinate system. The corners outside the hexagon
// are 'missing' so can't display any values assigned to them.
#ifndef WIDERTHANTALLER
#define NUM_COLS 20
#define NUM_ROWS 39
#else
#define NUM_COLS 40
#define NUM_ROWS 20
#endif
#define WIDTH NUM_COLS
#define HEIGHT NUM_ROWS
#define OUTOFBOUNDS (156 * 2 - 1) // NUM_LEDS_PER_STRIP * 2 - 1

extern const PROGMEM uint8_t col_to_top_row[NUM_COLS];

// function to map a x, y coordinate to the index into the led array
uint16_t XY(uint16_t x, uint16_t y);

#ifdef DEBUG
// loop through all pixels via x,y coordinates making sure they all get mapped correctly
void mode_xy_test();
#endif
