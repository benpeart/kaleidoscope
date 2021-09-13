#include "main.h"
#include "Kaleidoscope.h"
#include <Time.h>
#include <Preferences.h>
#include "RealTimeClock.h"
#include "displaynumbers.h"
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include "XYIndex.h"

// example of using GFX library with custom remap function (ie XY())
// https://github.com/marcmerlin/FastLED_NeoMatrix/issues/6

// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, NUM_COLS, NUM_ROWS, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE);

// enable debugging macros
#define DEBUG
#include "debug.h"

/* Useful Constants */
#define SECS_PER_MIN ((time_t)(60UL))
#define SECS_PER_HOUR ((time_t)(3600UL))
#define SECS_PER_DAY ((time_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((time_t)(7UL))
#define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))
#define SECS_PER_YEAR ((time_t)(SECS_PER_DAY * 365UL)) // TODO: ought to handle leap years
#define SECS_YR_2000 ((time_t)(946684800UL))           // the time at the start of y2k

//
// RealTimeClock ----------------------------
//

#ifdef DEBUG
void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        DB_PRINTLN("Failed to obtain time");
        return;
    }
    DB_PRINTLN(&timeinfo, "%A, %B %d %Y %I:%M:%S %p");
}
#endif

void rtc_setup()
{
    DB_PRINTLN(F("RealTimeClock.setup"));

    // read the timezone from persistant memory
    Preferences preferences;
    preferences.begin("kaleidoscope", true);
    String tz = preferences.getString("tz", "");
    preferences.end();
    if (tz.length())
    {
        configTzTime(tz.c_str(), "us.pool.ntp.org", "time.nist.gov");
        DB_PRINTF("Current timezone is %s\r\n", tz.c_str());
    }
    else
    {
        // hard code EST because I can :)
        configTime(-5 * SECS_PER_HOUR, SECS_PER_HOUR, "us.pool.ntp.org", "time.nist.gov");
        DB_PRINTLN(F("Current Timezone is not set. Enter Config Portal to set."));
    }
#ifdef DEBUG
    printLocalTime();
#endif

    matrix->begin();
    matrix->setTextWrap(false);
    //    matrix->setBrightness(BRIGHTNESS);
    matrix->setRemapFunction(XYToIndex);
}

int ConvertMilitaryTime(int hours)
{
    if (hours == 0)
        return 12;
    if (hours > 12)
        return hours - 12;

    return hours;
}

CRGB FadeColors(CRGB rgb)
{
    //return CRGB::Black;
    return rgb.fadeToBlackBy(222);
}

void drawNullClock()
{
}

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void drawDigitalClock()
{
    struct tm timeinfo;
    static int digit1 = -1, digit2 = -1, digit3 = -1, digit4 = -1;

    if (getLocalTime(&timeinfo))
    {
        int tmp;

        // compute first digit of hours
        tmp = ConvertMilitaryTime(timeinfo.tm_hour);
        tmp /= 10;
        if (digit1 != tmp)
        {
            digit1 = tmp;
            leds_dirty = true;
        }

        // compute second digit of hours
        tmp = ConvertMilitaryTime(timeinfo.tm_hour) % 10;
        if (digit2 != tmp)
        {
            digit2 = tmp;
            leds_dirty = true;
        }

        // compute first digit of minutes
        tmp = timeinfo.tm_min;
        tmp /= 10;
        if (digit3 != tmp)
        {
            digit3 = tmp;
            leds_dirty = true;
        }

        // compute second digit of minutes
        tmp = timeinfo.tm_min % 10;
        if (digit4 != tmp)
        {
            digit4 = tmp;
            leds_dirty = true;
            DB_PRINTLN(&timeinfo, "%A, %B %d %Y %I:%M:%S %p");
        }

        if (leds_dirty)
        {
#if 0
            matrix->setTextColor(BLACK);
            matrix->setTextSize(1);
            matrix->setCursor(4, NUM_ROWS / 2 - 9);
            matrix->printf("%02d", ConvertMilitaryTime(timeinfo.tm_hour));
            matrix->setCursor(4, NUM_ROWS / 2 + 2);
            matrix->printf("%02d", timeinfo.tm_min);
#else
            displayNumbers(digit1, digit2, digit3, digit4, FadeColors);
#endif
        }
    }
}

void displayHands(int hours, int minutes, int seconds, getColor color)
{
#ifdef DEBUG
    // do some sanity checking
    if (NULL == color)
    {
        DB_PRINTLN("displayHands called with NULL color function pointer");
        return;
    }

    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
    {
        DB_PRINTF("\rdisplayHands called with time that is out of range: %d:%d:%d\r\n", hours, minutes, seconds);
        return;
    }
#endif

    //
    // https://opencv-tutorials-hub.blogspot.com/2015/07/code-to-draw-analog-wall-clock-using-opencv-c-synchronised-with-system-time.html
    //
    float sec_angle, min_angle, hour_angle;
    int perimeter_x, perimeter_y;
    int radius = NUM_COLS / 2;
    int center_x = NUM_COLS / 2;
    int center_y = NUM_ROWS / 2;

    // Convert seconds to angle
    sec_angle = (seconds * 6) + 270;
    if (sec_angle > 360)
        sec_angle = sec_angle - 360;

    // Convert minutes to angle
    min_angle = minutes * 6 + 270;
    if (min_angle > 360)
        min_angle = min_angle - 360;

    // Convert hours to angle
    if (hours > 12)
        hours = hours - 12;
    hour_angle = (hours * 30) + (minutes * 0.5) + 270;
    if (hour_angle > 360)
        hour_angle = hour_angle - 360;

    // Find out the coordinates in the circle perimeter for second and draw the line from center
    perimeter_x = (int)(center_x + (radius - 0) * cos(sec_angle * 3.14159 / 180.0));
    perimeter_y = (int)(center_y + (radius - 0) * sin(sec_angle * 3.14159 / 180.0));
    matrix->drawLine(center_x, center_y, perimeter_x, perimeter_y, BLACK);

    // Find out the coordinates on the circle perimeter for minute and draw the line from center
    perimeter_x = (int)(center_x + (radius - 1) * cos(min_angle * 3.14159 / 180.0));
    perimeter_y = (int)(center_y + (radius - 1) * sin(min_angle * 3.14159 / 180.0));
    matrix->drawLine(center_x, center_y, perimeter_x, perimeter_y, BLACK);

    // Find out the coordinates on the circle perimeter for hour and draw the line from center
    perimeter_x = (int)(center_x + (radius - 2) * cos(hour_angle * 3.14159 / 180.0));
    perimeter_y = (int)(center_y + (radius - 2) * sin(hour_angle * 3.14159 / 180.0));
    matrix->drawLine(center_x, center_y, perimeter_x, perimeter_y, BLACK);

#if 0
    switch (hours)
    {
    case 0:
    case 12:
        matrix->drawLine(NUM_COLS / 2, 0, NUM_COLS / 2, NUM_ROWS / 2, BLACK);
        break;
    case 3:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS, NUM_ROWS / 2, BLACK);
        break;
    case 6:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS / 2, NUM_ROWS, BLACK);
        break;
    case 9:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, 0, NUM_ROWS / 2, BLACK);
        break;
    }

    switch (minutes)
    {
    case 0:
        matrix->drawLine(NUM_COLS / 2, 0, NUM_COLS / 2, NUM_ROWS / 2, BLUE);
        break;
    case 15:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS, NUM_ROWS / 2, BLUE);
        break;
    case 30:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS / 2, NUM_ROWS, BLUE);
        break;
    case 45:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, 0, NUM_ROWS / 2, BLUE);
        break;
    }

    switch (seconds)
    {
    case 0:
        matrix->drawLine(NUM_COLS / 2, 0, NUM_COLS / 2, NUM_ROWS / 2, RED);
        break;
    case 15:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS, NUM_ROWS / 2, RED);
        break;
    case 30:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, NUM_COLS / 2, NUM_ROWS, RED);
        break;
    case 45:
        matrix->drawLine(NUM_COLS / 2, NUM_ROWS / 2, 0, NUM_ROWS / 2, RED);
        break;
    }
#endif
    matrix->show();
}

void drawAnalogClock()
{
    struct tm timeinfo;
    static int hours = -1, minutes = -1, seconds = -1;

    if (getLocalTime(&timeinfo))
    {
        int tmp;

        // compute hours
        tmp = ConvertMilitaryTime(timeinfo.tm_hour);
        if (hours != tmp)
        {
            hours = tmp;
            leds_dirty = true;
        }

        // compute minutes
        tmp = timeinfo.tm_min;
        if (minutes != tmp)
        {
            minutes = tmp;
            leds_dirty = true;
        }

        // compute seconds
        tmp = timeinfo.tm_sec;
        if (seconds != tmp)
        {
            seconds = tmp;
            leds_dirty = true;
            DB_PRINTLN(&timeinfo, "%A, %B %d %Y %I:%M:%S %p");
        }

        if (leds_dirty)
            displayHands(hours, minutes, seconds, FadeColors);
    }
}

// Provide functions to draw different clock faces.
void (*drawClockFunc[N_CLOCK_FACES])(void){
    drawNullClock,
    drawDigitalClock,
    drawAnalogClock};
//#define N_CLOCK_FACES (sizeof(drawClockFunc) / sizeof(drawClockFunc[0]))
uint8_t clock_face = 0; // Index of current clock face in table

const PROGMEM char clockFaces[N_CLOCK_FACES][16] =
    {
        "None",
        "Digital",
        "Analog"};

void draw_clock()
{
    drawClockFunc[clock_face]();
}

void mode_select_clock_face()
{
    static boolean drawClockChanged = false;
#ifdef ENCODER
    // use the left knob to select kaleidoscope draw style
    static int lastLeftKnob = 0;
    int knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        if (knob < lastLeftKnob)
        {
            clock_face++;
            clock_face = clock_face % N_CLOCK_FACES;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (clock_face == 0)
                clock_face += N_CLOCK_FACES;
            --clock_face;
        }
        lastLeftKnob = knob;
        drawClockChanged = true;
    }
#endif

    // redraw the frame of the kaleidoscope with the new style
    if (drawClockChanged)
    {
        leds_dirty = true;
        drawClockChanged = false;
    }

    // draw this on a fixed schedule so that when you enter the mode, you aren't
    // faced with a black screen until you rotate the left knob
    EVERY_N_MILLISECONDS(100)
    {
        fill_kaleidoscope_solid(leds, CRGB::CornflowerBlue);
    }

    adjustBrightness();
}

int set_clock_face(int new_face)
{
    if (clock_face != new_face)
    {
        clock_face = new_face;
        leds_dirty = true;
    }

    return clock_face;
}
