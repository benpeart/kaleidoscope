#include <Arduino.h>
#include <Time.h>
#include <FastLED.h>
#include <Preferences.h>
#include "RealTimeClock.h"

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

void rtc_loop()
{
#ifdef DEBUG
    // only display the time every minute
    EVERY_N_MILLISECONDS(60000)
    {
        printLocalTime();
    }
#endif
}

void mode_select_clock_face()
{
}

