#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

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
// TODO: I was unsuccessful in finding a way to make these CRGB structures
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

// create a lookup table where the index is the number of the triangle and the result is
// three pairs of {strip, index} that can be passed to MirroredSetPixelColor so they are
// mirrored to the other 3 axis
struct Pixel
{
    uint8_t strip;
    uint8_t index;
};

struct Strips
{
    Pixel strips[3];
};

static const PROGMEM Strips KaleidoscopeLookupTable[TRIANGLE_COUNT] =
    {
        {{{1, 18}, {1, 19}, {1, 20}}},
        {{{1, 59}, {1, 58}, {1, 22}}},
        {{{1, 17}, {1, 57}, {1, 21}}},
        {{{1, 16}, {1, 56}, {1, 55}}},
        {{{1, 90}, {1, 91}, {1, 24}}},
        {{{1, 60}, {1, 92}, {1, 23}}},
        {{{1, 61}, {1, 93}, {1, 53}}},
        {{{1, 15}, {1, 94}, {1, 54}}},
        {{{1, 14}, {1, 95}, {1, 96}}},
        {{{1, 131}, {1, 130}, {1, 26}}},
        {{{1, 89}, {1, 129}, {1, 25}}},
        {{{1, 88}, {1, 128}, {1, 51}}},
        {{{1, 62}, {1, 127}, {1, 52}}},
        {{{1, 63}, {1, 126}, {1, 98}}},
        {{{1, 13}, {1, 125}, {1, 97}}},
        {{{1, 12}, {1, 124}, {1, 123}}},
        {{{0, 10}, {0, 11}, {1, 28}}},
        {{{1, 132}, {0, 12}, {1, 27}}},
        {{{1, 133}, {0, 13}, {1, 49}}},
        {{{1, 87}, {0, 14}, {1, 50}}},
        {{{1, 86}, {0, 15}, {1, 100}}},
        {{{1, 64}, {0, 16}, {1, 99}}},
        {{{1, 65}, {0, 17}, {1, 121}}},
        {{{1, 11}, {0, 18}, {1, 122}}},
        {{{1, 10}, {0, 19}, {0, 20}}},
        {{{0, 51}, {0, 50}, {1, 30}}},
        {{{0, 9}, {0, 49}, {1, 29}}},
        {{{0, 8}, {0, 48}, {1, 47}}},
        {{{1, 134}, {0, 47}, {1, 48}}},
        {{{1, 135}, {0, 46}, {1, 102}}},
        {{{1, 85}, {0, 45}, {1, 101}}},
        {{{1, 84}, {0, 44}, {1, 119}}},
        {{{1, 66}, {0, 43}, {1, 120}}},
        {{{1, 67}, {0, 42}, {0, 22}}},
        {{{1, 9}, {0, 41}, {0, 21}}},
        {{{1, 8}, {0, 40}, {0, 39}}},
        {{{0, 66}, {0, 67}, {1, 32}}},
        {{{0, 52}, {0, 68}, {1, 31}}},
        {{{0, 53}, {0, 69}, {1, 45}}},
        {{{0, 7}, {0, 70}, {1, 46}}},
        {{{0, 6}, {0, 71}, {1, 104}}},
        {{{1, 136}, {0, 72}, {1, 103}}},
        {{{1, 137}, {0, 73}, {1, 117}}},
        {{{1, 83}, {0, 74}, {1, 118}}},
        {{{1, 82}, {0, 75}, {0, 24}}},
        {{{1, 68}, {0, 76}, {0, 23}}},
        {{{1, 69}, {0, 77}, {0, 37}}},
        {{{1, 7}, {0, 78}, {0, 38}}},
        {{{1, 6}, {0, 79}, {0, 80}}},
        {{{0, 107}, {0, 106}, {1, 34}}},
        {{{0, 65}, {0, 105}, {1, 33}}},
        {{{0, 64}, {0, 104}, {1, 43}}},
        {{{0, 54}, {0, 103}, {1, 44}}},
        {{{0, 55}, {0, 102}, {1, 106}}},
        {{{0, 5}, {0, 101}, {1, 105}}},
        {{{0, 4}, {0, 100}, {1, 115}}},
        {{{1, 138}, {0, 99}, {1, 116}}},
        {{{1, 139}, {0, 98}, {0, 26}}},
        {{{1, 81}, {0, 97}, {0, 25}}},
        {{{1, 80}, {0, 96}, {0, 35}}},
        {{{1, 70}, {0, 95}, {0, 36}}},
        {{{1, 71}, {0, 94}, {0, 82}}},
        {{{1, 5}, {0, 93}, {0, 81}}},
        {{{1, 4}, {0, 92}, {0, 91}}},
        {{{0, 114}, {0, 115}, {1, 36}}},
        {{{0, 108}, {0, 116}, {1, 35}}},
        {{{0, 109}, {0, 117}, {1, 41}}},
        {{{0, 63}, {0, 118}, {1, 42}}},
        {{{0, 62}, {0, 119}, {1, 108}}},
        {{{0, 56}, {0, 120}, {1, 107}}},
        {{{0, 57}, {0, 121}, {1, 113}}},
        {{{0, 3}, {0, 122}, {1, 114}}},
        {{{0, 2}, {0, 123}, {0, 28}}},
        {{{1, 140}, {0, 124}, {0, 27}}},
        {{{1, 141}, {0, 125}, {0, 33}}},
        {{{1, 79}, {0, 126}, {0, 34}}},
        {{{1, 78}, {0, 127}, {0, 84}}},
        {{{1, 72}, {0, 128}, {0, 83}}},
        {{{1, 73}, {0, 129}, {0, 89}}},
        {{{1, 3}, {0, 130}, {0, 90}}},
        {{{1, 2}, {0, 131}, {0, 132}}},
        {{{0, 155}, {0, 154}, {1, 38}}},
        {{{0, 113}, {0, 153}, {1, 37}}},
        {{{0, 112}, {0, 152}, {1, 39}}},
        {{{0, 110}, {0, 151}, {1, 40}}},
        {{{0, 111}, {0, 150}, {1, 110}}},
        {{{0, 61}, {0, 149}, {1, 109}}},
        {{{0, 60}, {0, 148}, {1, 111}}},
        {{{0, 58}, {0, 147}, {1, 112}}},
        {{{0, 59}, {0, 146}, {0, 30}}},
        {{{0, 1}, {0, 145}, {0, 29}}},
        {{{0, 0}, {0, 144}, {0, 31}}},
        {{{1, 142}, {0, 143}, {0, 32}}},
        {{{1, 143}, {0, 142}, {0, 86}}},
        {{{1, 77}, {0, 141}, {0, 85}}},
        {{{1, 76}, {0, 140}, {0, 87}}},
        {{{1, 74}, {0, 139}, {0, 88}}},
        {{{1, 75}, {0, 136}, {0, 134}}},
        {{{1, 1}, {0, 137}, {0, 133}}},
        {{{1, 0}, {0, 136}, {0, 135}}}};

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
        offset_1 = random(strip_1_columns);
        offset_2 = random(strip_2_columns);
    }

    // update the position of the strips and draw the kaleidoscope
    void loop()
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

        offset_1 = (++offset_1) % strip_1_columns;
        offset_2 = (++offset_2) % strip_2_columns;
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
    void drawKaleidoscopePixel6(int index, CRGB c)
    {
#ifdef NDEBUG
        DB_PRINT(F("drawKaleidoscopePixel6("));
        DB_PRINT(index);
        DB_PRINT(F(", 0x"));
        DB_PRINT(c, HEX);
        DB_PRINTLN(")");
#endif
        if (index < 0 || index >= TRIANGLE_COUNT)
            return;

        for (uint8_t x = 0; x < 3; x++)
            MirroredSetPixelColor(pgm_read_byte_near(&KaleidoscopeLookupTable[index].strips[x].strip),
                                  pgm_read_byte_near(&KaleidoscopeLookupTable[index].strips[x].index), c);
    }

    void fill_kaleidoscope_rainbow(uint8_t initialhue, uint8_t deltahue)
    {
        CHSV hsv;
        hsv.hue = initialhue;
        hsv.val = 255;
        hsv.sat = 240;
        for (int i = 0; i < TRIANGLE_COUNT; ++i)
        {
            drawKaleidoscopePixel6(i, hsv);
            hsv.hue += deltahue;
        }
    }

    void fill_kaleidoscope_solid(const struct CRGB &color)
    {
        for (int i = 0; i < TRIANGLE_COUNT; ++i)
        {
            drawKaleidoscopePixel6(i, color);
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
    int offset_1;
    int offset_2;

    void MirroredSetPixelColor(int strip, int index, CRGB rgb)
    {
        if (index < 0 || index >= NUM_LEDS_PER_STRIP)
        {
            DB_PRINT(F("MysetPixelColor: requested index ("));
            DB_PRINT(index);
            DB_PRINTLN(F(") exceeds number of LEDs in a strip."));
            return;
        }

        switch (strip)
        {
        case 0:
            leds[0 * NUM_LEDS_PER_STRIP + index] = rgb;
            leds[3 * NUM_LEDS_PER_STRIP + index] = rgb;
            break;

        case 1:
            leds[1 * NUM_LEDS_PER_STRIP + index] = rgb;
            leds[2 * NUM_LEDS_PER_STRIP + index] = rgb;
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
    static int index = -1;

    EVERY_N_MILLISECONDS(100)
    {
        // erase the last pixel
        kaleidoscope.drawKaleidoscopePixel6(index, CRGB::Black); // off

        // move to the next pixel
        if (++index >= TRIANGLE_COUNT)
            index = 0;
        DB_PRINTLN(index);

        // light up the next pixel
        kaleidoscope.drawKaleidoscopePixel6(index, CRGB::Red);
    }
}
#endif

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/rainbow_march/rainbow_march.ino
void mode_kaleidoscope_rainbowMarch()
{
    //DB_PRINTLN(F("mode_kaleidoscope_rainbowMarch"));

    uint8_t thisdelay = 200, deltahue = 255 / TRIANGLE_COUNT;
    uint8_t thishue = millis() * (255 - thisdelay) / 255; // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.

    // thishue = beat8(50);           // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
    // thishue = beatsin8(50,0,255);  // This can change speeds on the fly. You can also add these to each other.

    kaleidoscope.fill_kaleidoscope_rainbow(thishue, deltahue);
}

// https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

void mode_kaleidoscope_plasma()
{
    //DB_PRINTLN(F("mode_kaleidoscope_plasma"));

    static CRGBPalette16 currentPalette = ForestColors_p; // Palette definitions
    static CRGBPalette16 targetPalette;
    static TBlendType currentBlending = LINEARBLEND;

    EVERY_N_MILLISECONDS(50)
    {                                         // FastLED based non-blocking delay to update/display the sequence.
        int thisPhase = beatsin8(6, -64, 64); // Setting phase change for a couple of waves.
        int thatPhase = beatsin8(7, -64, 64);

        for (int k = 0; k < TRIANGLE_COUNT; k++)
        {
            // For each of the LED's in the strand, set a brightness based on a wave as follows:
            int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2; // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
            int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

            kaleidoscope.drawKaleidoscopePixel6(k, ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending)); // Let's now add the foreground colour.
        }
    }

    EVERY_N_MILLISECONDS(100)
    {
        uint8_t maxChanges = 24;
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
    }

    EVERY_N_SECONDS(5)
    {                              // Change the target palette to a random one every 5 seconds.
        uint8_t baseC = random8(); // You can use this as a baseline colour if you want similar hues in the next line.
        targetPalette = CRGBPalette16(CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)), CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)));
    }
}

// https://github.com/atuline/FastLED-Demos/blob/master/sawtooth/sawtooth.ino
void mode_kaleidoscope_sawTooth()
{
    //DB_PRINTLN(F("mode_kaleidoscope_sawTooth"));

    // Palette definitions
    static CRGBPalette16 currentPalette = PartyColors_p;
    static TBlendType currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND

    int bpm = 60;
    int ms_per_beat = 60000 / bpm; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
    int ms_per_led = 60000 / bpm / TRIANGLE_COUNT;

    int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (TRIANGLE_COUNT); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

    if (cur_led == 0)
        kaleidoscope.fill_kaleidoscope_solid(CRGB::Black);
    else
        kaleidoscope.drawKaleidoscopePixel6(cur_led, ColorFromPalette(currentPalette, 0, 255, currentBlending)); // I prefer to use palettes instead of CHSV or CRGB assignments.
}

#endif

void mode_kaleidoscope_select_disks()
{
    DB_PRINTLN(F("mode_kaleidoscope_select_disks"));
}

#endif // KALEIDOSCOPE_H
