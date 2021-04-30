#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include <Time.h>

/* Useful Constants */
#define SECS_PER_MIN  ((time_t)(60UL))
#define SECS_PER_HOUR ((time_t)(3600UL))
#define SECS_PER_DAY  ((time_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((time_t)(7UL))
#define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))
#define SECS_PER_YEAR ((time_t)(SECS_PER_DAY * 365UL)) // TODO: ought to handle leap years
#define SECS_YR_2000  ((time_t)(946684800UL)) // the time at the start of y2k

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %I:%M:%S %p");
}

class RealTimeClock
{
public:
    void setup()
    {
        DB_PRINTLN(F("RealTimeClock.setup"));

        configTime(-5 * SECS_PER_HOUR, SECS_PER_HOUR, "us.pool.ntp.org");
    }

    void loop()
    {
        // only display the time every second
        EVERY_N_MILLISECONDS (1000)
        {
            printLocalTime();
        }
    }
};

void mode_select_clock_face()
{
}

void mode_set_clock()
{
}
#endif // REALTIMECLOCK_H
