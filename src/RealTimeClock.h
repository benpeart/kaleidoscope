#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

time_t getTeensy3Time()
{
    return Teensy3Clock.get();
}

class RealTimeClock
{
public:
    void setup()
    {
        // set the Time library to use Teensy 3.0's RTC to keep time
        setSyncProvider(getTeensy3Time);

        DB_PRINTLN(F("RealTimeClock.setup"));

        if (timeStatus() != timeSet)
            DB_PRINTLN(F("Unable to sync with the RTC"));
    }

    void loop()
    {
        static int lastSecond = -99;

        time_t time = now();

        // only display the time every second
        if (lastSecond != second(time))
        {
            lastSecond = second(time);

            DB_PRINT(year(time), DEC);
            DB_PRINT('/');
            DB_PRINT(month(time), DEC);
            DB_PRINT('/');
            DB_PRINT(day(time), DEC);
            DB_PRINT(F(" ("));
            DB_PRINT(daysOfTheWeek[weekday(time)]);
            DB_PRINT(F(") "));
            DB_PRINT(hour(time), DEC);
            DB_PRINT(':');
            DB_PRINT(minute(time), DEC);
            DB_PRINT(':');
            DB_PRINT(second(time), DEC);
            DB_PRINTLN();
        }
    }

private:
    const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};

extern RealTimeClock clock;

void mode_select_clock_face()
{
}

void mode_set_clock()
{
}

#endif // REALTIMECLOCK_H
