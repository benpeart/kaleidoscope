#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

class RealTimeClock
{
public:
    void setup()
    {
        DB_PRINTLN("RealTimeClock.setup");
        if (!rtc.begin())
        {
            DB_PRINTLN("Couldn't find RTC");
        }

        if (!rtc.isrunning())
        {
            DB_PRINTLN("RTC is NOT running, let's set the time!");
            // When time needs to be set on a new device, or after a power loss, the
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            // This line sets the RTC with an explicit date & time, for example to set
            // January 21, 2014 at 3am you would call:
            // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        }
    }

    void loop()
    {
        DB_PRINTLN("RealTimeClock.loop");
        DateTime now = rtc.now();

        DB_PRINT(now.year(), DEC);
        DB_PRINT('/');
        DB_PRINT(now.month(), DEC);
        DB_PRINT('/');
        DB_PRINT(now.day(), DEC);
        DB_PRINT(" (");
        DB_PRINT(daysOfTheWeek[now.dayOfTheWeek()]);
        DB_PRINT(") ");
        DB_PRINT(now.hour(), DEC);
        DB_PRINT(':');
        DB_PRINT(now.minute(), DEC);
        DB_PRINT(':');
        DB_PRINT(now.second(), DEC);
        DB_PRINTLN();
    }

private:
    RTC_DS1307 rtc;
#ifdef DEBUG
    const char PROGMEM daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#endif
};

#endif // REALTIMECLOCK_H
