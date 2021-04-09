#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include "seven-segment.h"

class RealTimeClock
{
public:
    void setup()
    {
        Serial.println("RealTimeClock.setup");
        if (!rtc.begin())
        {
#ifdef __DEBUG__
            Serial.println("Couldn't find RTC");
#endif
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
    void loop() // Emulates a seven segment clock to display the time
    {
        uint32_t color;
        //the easiest way to draw the clock is to break the time into individual numbers and draw them 1 by 1;
        int min;  //= time.getMin();
        int hour; //= time.getHour();

        int minOnes = min % 10;
        int hourOnes = hour % 10;

        int minTens;
        int hourTens = 1;

        //Unsure if there is a better way to do this but this works for now
        if (min >= 0 && min < 10)
            minTens = 0;
        if (min >= 10 && min < 20)
            minTens = 1;
        else if (min >= 20 && min < 30)
            minTens = 2;
        else if (min >= 30 && min < 40)
            minTens = 3;
        else if (min >= 40 && min < 50)
            minTens = 4;
        else if (min >= 50 && min < 60)
            minTens = 5;

        if (hour >= 10) // Checks to see if we need to draw the 1 for the hour's tens place
            drawHourTensOne(color);

        handleSegmentsHourOnes(hourOnes, color);
        handleSegmentsMinTens(minTens, color);
        handleSegmentsMinOnes(minOnes, color);
    }
    /*
    void loop()
    {
        Serial.println("RealTimeClock.loop");
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
*/
private:
    RTC_DS1307 rtc;
#ifdef __DEBUG__
    const char PROGMEM daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#endif
};

#endif // REALTIMECLOCK_H
