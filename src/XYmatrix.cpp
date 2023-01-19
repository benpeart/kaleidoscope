#include "main.h"
#include "render.h"
#include "XYmatrix.h"

#ifdef DEMO

#define DEFAULT_MILLIS 75
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

// https://gist.github.com/Jerware/b82ad4768f9935c8acfccc98c9211111#file-matrixeffect-ino
void mode_xy_matrix()
{
    EVERY_N_MILLIS_I(timer, DEFAULT_MILLIS) // falling speed
    {
        timer.setPeriod(MAX_MILLIS - map(kaleidoscopeSpeed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS));
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
