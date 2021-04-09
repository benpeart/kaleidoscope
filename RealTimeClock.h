#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

class RealTimeClock
{
public:
    // default constructor
    //RealTimeClock(){};

    void setup()
    {
        if (!rtc.begin())
        {
#ifdef __DEBUG__
            Serial.println("Couldn't find RTC");
            Serial.flush();
#endif
            abort();
        }

        if (!rtc.isrunning())
        {
#ifdef __DEBUG__
            Serial.println("RTC is NOT running, let's set the time!");
#endif
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
        DateTime now = rtc.now();
#ifdef __DEBUG__
        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
#endif
    }

private:
    RTC_DS1307 rtc;
#ifdef __DEBUG__
    const char PROGMEM daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#endif
};

#endif // REALTIMECLOCK_H
