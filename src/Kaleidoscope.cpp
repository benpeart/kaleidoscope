#include "main.h"
#include "render.h"
#include "Kaleidoscope.h"
#include "GlassDisk.h"

// we're going to blend between frames so need an array of LEDs for both frames
// then we'll blend them together into the 'output' leds array which will be displayed
CRGB leds2[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds3[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// How often to change color palettes.
#define SECONDS_PER_PALETTE (5 * 60)
CRGBPalette16 gKTargetPalette;

#define DEFAULT_MILLIS 500
#define MIN_MILLIS 0
#define MAX_MILLIS (4 * DEFAULT_MILLIS)

#define VIEWPORT_HEIGHT 10  // the height of the 'viewport' triangle
#define TRIANGLE_COLUMNS 19 // the width of the base of the 'viewport' triange

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
                
                   // qsub gives it a bit of 'black' dead space by setting a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex.
                int thisBright = qsuba(colorIndex_1, beatsin8(7, 0, 64)); 
                pixel_1 = ColorFromPalette(*disk_1->pal, colorIndex_1, thisBright);
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

                // qsub gives it a bit of 'black' dead space by setting a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex.
                int thisBright = qsuba(colorIndex_2, beatsin8(7, 0, 64)); 
                pixel_2 = ColorFromPalette(*disk_2->pal, colorIndex_2, thisBright);
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

void mode_kaleidoscope_select_speed_brightness()
{
    static boolean first_array = true;
    static int time_of_last_frame = 0;

    int time = millis();
    int ms_between_frames = MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS);

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

#define SCREENSAVER_DELAY 60000 // 1 minute = 60,000 milliseconds
void mode_kaleidoscope()
{
    static int time_to_enter_screensaver_mode = 0; // start in screensaver mode
    static boolean first_array = true;
    static int time_of_last_frame = 0;
    static boolean drawframe = true; // start by drawing the first frame
    static boolean blendframes = false;
    static bool init = false;

    // do a one time init
    if (!init)
    {
        init = true;
        chooseNextDiskPalette(gKTargetPalette);
    }

    EVERY_N_SECONDS(SECONDS_PER_PALETTE)
    {
        chooseNextDiskPalette(gKTargetPalette);
    }

    EVERY_N_MILLIS(10)
    {
        nblendPaletteTowardPalette(gCurrentDiskPalette, gKTargetPalette, 12);
    }

    int time = millis();
    int ms_between_frames = MAX_MILLIS - map(kaleidoscope_speed, KALEIDOSCOPE_MIN_SPEED, KALEIDOSCOPE_MAX_SPEED, MIN_MILLIS, MAX_MILLIS);

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
    // When changing modes (esp through 'off') the leds are flagged dirty so that they
    // get redrawn, without checking for leds_dirty here, we wouldn't redraw the
    // kaleidoscope unless there was movement (via the knobs) or if we were already in
    // screen saver mode.
    if (drawframe || leds_dirty)
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

    // adjust the brightness but don't use the knob to change the manual brightness
    // as we're using it above.
    adjustBrightness(false);
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
        set_draw_style(draw_style);
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
// test the wiring and ensure all pixels light up correctly
// Q: Why does led[300] ==> led[312] not light up?
// A: Our strips aren't the same length (156 vs 144) so the shorter strips (1 and 2)
// have extra leds[x] positions that don't have physical LEDs.
void mode_test()
{
  static int index = 0;

  EVERY_N_MILLISECONDS(50)
  {
    // erase the last pixel
    leds[index] = CRGB::Black; // off

    // move to the next pixel
    if (++index >= NUM_STRIPS * NUM_LEDS_PER_STRIP)
      index = 0;
    DB_PRINTLN(index);

    // light up the next pixel
    leds[index] = CRGB::Red;

    leds_dirty = true;
  }

  adjustBrightness();
}

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
#endif // DEBUG
