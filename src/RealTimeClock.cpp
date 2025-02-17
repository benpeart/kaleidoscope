#include "main.h"
#include "debug.h"
#include "settings.h"
#include "render.h"
#include <Time.h>
#include "RealTimeClock.h"
#include "displaynumbers.h"
#ifdef WEATHER
#include "weather.h"
#endif

// example of using GFX library with custom remap function (ie XY())
// https://github.com/marcmerlin/FastLED_NeoMatrix/issues/6

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
    String tz = preferences.getString("tz", "EST5EDT,M3.2.0/2,M11.1.0/2");
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

CRGB SetColorToBlack(CRGB rgb)
{
    return CRGB::Black;
}

CRGB FadeColors(CRGB rgb)
{
    return rgb.fadeToBlackBy(222);
}

CRGB BlendColors(CRGB rgb)
{
    return blend(rgb, settings.clockColor, 200);
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
            displayNumbers(digit1, digit2, digit3, digit4, BlendColors);
    }
}

#ifdef WEATHER

// show the time for 25 seconds, then the temp for 5 seconds
#define SECONDS_AS_CLOCK 25
#define SECONDS_AS_TEMP 5

void drawTimeTempClock()
{
    static bool display_time = true;
    static int digit1 = -1, digit2 = -1;

    EVERY_N_SECONDS_I(timer, SECONDS_AS_CLOCK)
    {
        display_time = !display_time;
        timer.setPeriod(display_time ? SECONDS_AS_CLOCK : SECONDS_AS_TEMP);
    }

    if (display_time)
    {
        drawDigitalClock();
    }
    else
    {
        // compute first digit of tempreture
        int tmp = current_weather.temp;
        tmp /= 10;
        if (digit1 != tmp)
        {
            digit1 = tmp;
            leds_dirty = true;
        }

        // compute second digit of tempreture
        tmp = current_weather.temp % 10;
        if (digit2 != tmp)
        {
            digit2 = tmp;
            leds_dirty = true;
        }

        if (leds_dirty)
            displayNumbers(digit1, digit2, -1, -1, BlendColors);
    }
}
#endif

// https://wokwi.com/arduino/projects/286985034843292172
#include "wuLineAA.h"

// https://mathopenref.com/coordparamellipse.html
void wuVectorAA(const uint16_t x, const uint16_t y, const uint16_t a, const uint16_t b, const uint16_t theta, CRGB *col)
{
    int16_t dx, dy;
    dx = (a * (int32_t)cos16(theta)) / 32768;
    dy = (b * (int32_t)sin16(theta)) / 32768;
    wuLineAA(x, y, x + dx, y + dy, col);
}

// https://wokwi.com/arduino/projects/286985034843292172
void displayHands(int hours, int minutes, int seconds, CRGB color)
{
#ifdef DEBUG
    // do some sanity checking
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
    {
        DB_PRINTF("\rdisplayHands called with time that is out of range: %d:%d:%d\r\n", hours, minutes, seconds);
        return;
    }
#endif

    // everything is fixed-point, with 8-bits of fraction
    uint16_t centrex = WIDTH * 128 - 128;
    uint16_t centrey = HEIGHT * 128 - 128;
    uint16_t a = WIDTH * 128;
    uint16_t b = HEIGHT * 128;
    uint16_t base_theta = 65536 * 3 / 4;

// Turn off the second hand as it is hard to differentiate from the minute hand
    // second hand with sweep action
    uint16_t theta = seconds * 65536 / 60;
#ifdef SECOND_HANDS
    static uint16_t sweep_theta = theta;
    int32_t diff = theta - sweep_theta;
    if (diff < 0)
        diff += 65536;
    sweep_theta += (diff + 8) / 16;
    wuVectorAA(centrex, centrey, a, b, base_theta + sweep_theta, &color);
#endif

    // minute hand
    a = a * 7 / 8;
    b = b * 7 / 8;
    theta = (theta + minutes * 65536) / 60;
    wuVectorAA(centrex, centrey, a, b, base_theta + theta, &color);

    // hour hand
    a = a * 1 / 2;
    b = b * 1 / 2;
    theta = (theta + (hours % 12) * 65536) / 12;
    wuVectorAA(centrex, centrey, a, b, base_theta + theta, &color);
}

void drawAnalogClock()
{
    struct tm timeinfo;
    static int hours = -1, minutes = -1, seconds = -1;

// turn off hash marks as they confuse the wife :)        
#ifdef NEVER
    if (leds_dirty)
    {
        // draw marks at every 5 seconds
#ifdef COMPUTEDHASHMARKS
        for (int x = 0; x < 60; x += 5)
        {
            uint16_t centrex = WIDTH * 128 - 128;
            uint16_t centrey = HEIGHT * 128 - 128;
            uint16_t a = WIDTH * 128;
            uint16_t b = HEIGHT * 128;
            uint16_t base_theta = 65536 * 3 / 4;
            uint16_t theta = x * 65536 / 60;
            int16_t x1, y1, x2, y2;

            x1 = (a * 3 / 4 * (int32_t)cos16(base_theta + theta)) / 32768;
            y1 = (b * 3 / 4 * (int32_t)sin16(base_theta + theta)) / 32768;
            x2 = (a * (int32_t)cos16(base_theta + theta)) / 32768;
            y2 = (b * (int32_t)sin16(base_theta + theta)) / 32768;
            wuLineAA(centrex + x1, centrey + y1, centrex + x2, centrey + y2, &settings.clockColor);
        }
#else
        uint16_t index;

        // the even numbers are marked by the points of the hexagon so no need to draw them
#ifdef false
        // 0/12
        index = XY(WIDTH / 2, 0);
        leds[index] = BlendColors(leds[index]);
        // 2
        index = XY(WIDTH - 1, HEIGHT / 2 - 10);
        leds[index] = BlendColors(leds[index]);
        // 4
        index = XY(WIDTH - 1, HEIGHT / 2 + 10);
        leds[index] = BlendColors(leds[index]);
        // 6
        index = XY(WIDTH / 2, HEIGHT - 1);
        leds[index] = BlendColors(leds[index]);
        // 8
        index = XY(0, HEIGHT / 2 + 10);
        leds[index] = BlendColors(leds[index]);
        // 10
        index = XY(0, HEIGHT / 2 - 10);
        leds[index] = BlendColors(leds[index]);
#endif
        // 1
        index = XY(WIDTH / 2 + 4, 5);
        leds[index] = BlendColors(leds[index]);
        // 3
        index = XY(WIDTH - 1, HEIGHT / 2);
        leds[index] = BlendColors(leds[index]);
        // 5
        index = XY(WIDTH - 6, HEIGHT / 2 + 14);
        leds[index] = BlendColors(leds[index]);
        // 7
        index = XY(WIDTH / 2 - 5, HEIGHT - 6);
        leds[index] = BlendColors(leds[index]);
        // 9
        index = XY(0, HEIGHT / 2);
        leds[index] = BlendColors(leds[index]);
        // 11
        index = XY(WIDTH / 2 - 5, 5);
        leds[index] = BlendColors(leds[index]);
#endif
    }
#endif // NEVER    

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
            displayHands(hours, minutes, seconds, settings.clockColor);
    }
}

// This look up table lists each of the clock drawing functions and their names
ClockFace clockFaceLUT[]{
    {drawNullClock, "Off"},
    {drawDigitalClock, "Digital"},
#ifdef WEATHER
    {drawTimeTempClock, "Time and Temp"}, 
#endif
    {drawAnalogClock, "Analog"}
};
uint8_t clockFaces = (sizeof(clockFaceLUT) / sizeof(clockFaceLUT[0])); // total number of valid face names in table

void drawClock()
{
    clockFaceLUT[settings.clockFace].renderFunc();
}

#ifndef WIFI // if we have WIFI, we don't need the manual settings modes
void mode_select_clock_face()
{
#ifdef ENCODER
    // use the left knob to select the clock face to draw
    static int lastLeftKnob = 0;
    int newFace = clockFace;
    int knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        if (knob < lastLeftKnob)
        {
            newFace++;
            newFace = newFace % clockFaces;
        }
        else
        {
            // offset is an unsigned 8 bits so can't go negative
            if (newFace == 0)
                newFace += clockFaces;
            --newFace;
        }
        lastLeftKnob = knob;
    }
#endif

    // save the new clock face
    setClockFace(newFace);

    // draw this on a fixed schedule so that when you enter the mode, you aren't
    // faced with a black screen until you rotate the left knob
    EVERY_N_MILLISECONDS(100)
    {
        fill_kaleidoscope_solid(leds, CRGB::CornflowerBlue);
    }

    adjustBrightness();
}
#endif

int setClockFace(int newFace)
{
    if (settings.clockFace != newFace)
    {
        settings.clockFace = newFace;
        DB_PRINTF("setClockFace: %s\r\n", clockFaceLUT[settings.clockFace].faceName);
        leds_dirty = true;
#ifdef WEATHER
        // update whether we need to fetch the weather
        need_weather = (drawClockFunc[clockFace] == drawTimeTempClock) ? true : false;
#endif
    }

    return settings.clockFace;
}
