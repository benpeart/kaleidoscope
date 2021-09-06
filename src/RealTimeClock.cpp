#include <Arduino.h>
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
    printLocalTime();
}

void mode_select_clock_face()
{
}

CRGB FadeColors(CRGB rgb)
{
    return CRGB::Black;
    //    return rgb.fadeToBlackBy(64);
}

void draw_clock()
{
    struct tm timeinfo;
    static int digit1 = 0, digit2 = 0, digit3 = 0, digit4 = 0;

    if (getLocalTime(&timeinfo))
    {
        int tmp;

        // compute first digit of hours
        tmp = timeinfo.tm_hour;
        while (tmp >= 10)
            tmp /= 10;
        if (digit1 != tmp)
        {
            digit1 = tmp;
            leds_dirty = true;
        }

        // compute second digit of hours
        tmp = timeinfo.tm_hour % 10;
        if (digit2 != tmp)
        {
            digit2 = tmp;
            leds_dirty = true;
        }

        // compute first digit of minutes
        tmp = timeinfo.tm_min;
        while (tmp >= 10)
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
