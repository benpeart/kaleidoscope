#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H


class RealTimeClock
{
public:
    void setup()
    {
        DB_PRINTLN(F("RealTimeClock.setup"));
        if (!rtc.begin())
        {
            DB_PRINTLN(F("Couldn't find RTC"));
        }

        if (!rtc.isrunning())
        {
            DB_PRINTLN(F("RTC is NOT running, let's set the time!"));
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
        static int lastSecond = -99;

        DateTime now = rtc.now();

        // only display the time every second
        if (lastSecond != now.second())
        {
            lastSecond = now.second();

            DB_PRINT(now.year(), DEC);
            DB_PRINT('/');
            DB_PRINT(now.month(), DEC);
            DB_PRINT('/');
            DB_PRINT(now.day(), DEC);
            DB_PRINT(F(" ("));
            DB_PRINT(daysOfTheWeek[now.dayOfTheWeek()]);
            DB_PRINT(F(") "));
            DB_PRINT(now.hour(), DEC);
            DB_PRINT(':');
            DB_PRINT(now.minute(), DEC);
            DB_PRINT(':');
            DB_PRINT(now.second(), DEC);
            DB_PRINTLN();
        }
    }

private:
    RTC_DS1307 rtc;
    const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};

extern RealTimeClock clock;

void mode_select_clock_face()
{
    DB_PRINTLN(F("mode_select_clock_face"));
}

void mode_set_clock()
{
    DB_PRINTLN(F("mode_set_clock"));
}

#endif // REALTIMECLOCK_H
