#include "main.h"
#include "Kaleidoscope.h"
#include "XY.h"
#include "XYmatrix.h"

#ifdef DEMO

#define DEFAULT_MILLIS 75
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// mode_kaleidoscope_matrix spawns new falling code in the top row and then moves them down screen.
// Since most of our top row is missing, map it to the top row available for the given column.
static uint8_t col_to_top_row [] = {
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

// https://gist.github.com/Jerware/b82ad4768f9935c8acfccc98c9211111#file-matrixeffect-ino
void mode_xy_matrix()
{
    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS) // falling speed
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
        // move code downward
        // start with lowest row to allow proper overlapping on each column
        for (int8_t row = NUM_ROWS - 1; row >= 0; row--)
        {
            for (int8_t col = 0; col < NUM_COLS; col++)
            {
                if (leds[XY(col, row)] == CRGB(175, 255, 175))
                {
                    leds[XY(col, row)] = CRGB(27, 130, 39); // create trail
                    if (row < NUM_ROWS - col_to_top_row[col] - 1)
                        leds[XY(col, row + 1)] = CRGB(175, 255, 175);
                }
            }
        }

        // fade all leds
        for (int i = 0; i < NUM_STRIPS * NUM_LEDS_PER_STRIP; i++)
        {
            if (leds[i].g != 255)
                leds[i].nscale8(192); // only fade trail
        }

        // check for empty screen to ensure code spawn
        bool emptyScreen = true;
        for (int i = 0; i < NUM_STRIPS * NUM_LEDS_PER_STRIP; i++)
        {
            if (leds[i])
            {
                emptyScreen = false;
                break;
            }
        }

        // spawn new falling code
        if (random8(3) == 0 || emptyScreen) // lower number == more frequent spawns
        {
            int8_t spawnX = random8(NUM_COLS);
            leds[XY(spawnX, pgm_read_byte_near(&col_to_top_row[spawnX]))] = CRGB(175, 255, 175);
        }

        leds_dirty = true;
    }

    adjustSpeed();
    adjustBrightness();
}

#endif
