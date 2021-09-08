#include "main.h"
#include "Kaleidoscope.h"
#include <Time.h>
#include <Preferences.h>
#include "RealTimeClock.h"
#include "displaynumbers.h"

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
    return CRGB::Black;
    //    return rgb.fadeToBlackBy(128);
}

void drawNullClock()
{
}

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
            displayNumbers(digit1, digit2, digit3, digit4, FadeColors);
    }
}

// Provide functions to draw different clock faces.
void (*drawClockFunc[])(void){
    drawNullClock,
    drawDigitalClock};
#define N_CLOCK_STYLES (sizeof(drawClockFunc) / sizeof(drawClockFunc[0]))
uint8_t clock_style = 0; // Index of current clock face in table

void draw_clock()
{
    drawClockFunc[clock_style]();
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
            clock_style++;
            clock_style = clock_style % N_CLOCK_STYLES;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (clock_style == 0)
                clock_style += N_CLOCK_STYLES;
            --clock_style;
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
