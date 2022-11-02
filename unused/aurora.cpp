#include <Arduino.h>
#include <FastLED.h>
#include "render.h"
#include "aurora.h"

/*
  Aurora effect
*/

//CONFIG
#define BACKLIGHT 5
#define W_MAX_COUNT 20   //Number of simultaneous waves
#define W_MAX_SPEED 6    //Higher number, higher speed
#define W_WIDTH_FACTOR 6 //Higher number, smaller waves

class AuroraWave
{
private:
    uint16_t ttl;
    CRGB basecolor;
    float basealpha;
    uint16_t age;
    uint16_t width;
    float center;
    bool goingleft;
    float speed_factor;
    bool alive = true;

public:
    void init(uint32_t segment_length, CRGB color)
    {
        ttl = random(500, 1501);
        basecolor = color;
        basealpha = random(60, 101) / (float)100;
        age = 0;
        width = random(segment_length / 20, segment_length / W_WIDTH_FACTOR); //half of width to make math easier
        if (!width)
            width = 1;
        center = random(101) / (float)100 * segment_length;
        goingleft = random(0, 2) == 0;
        speed_factor = (random(10, 31) / (float)100 * W_MAX_SPEED / 255);
        alive = true;
    }

    CRGB getColorForLED(int ledIndex)
    {
        if (ledIndex < center - width || ledIndex > center + width)
            return 0; //Position out of range of this wave

        CRGB rgb;

        //Offset of this led from center of wave
        //The further away from the center, the dimmer the LED
        float offset = ledIndex - center;
        if (offset < 0)
            offset = -offset;
        float offsetFactor = offset / width;

        //The age of the wave determines it brightness.
        //At half its maximum age it will be the brightest.
        float ageFactor = 0.1;
        if ((float)age / ttl < 0.5)
        {
            ageFactor = (float)age / (ttl / 2);
        }
        else
        {
            ageFactor = (float)(ttl - age) / ((float)ttl * 0.5);
        }

        //Calculate color based on above factors and basealpha value
        float factor = (1 - offsetFactor) * ageFactor * basealpha;
        rgb.r = basecolor.r * factor;
        rgb.g = basecolor.g * factor;
        rgb.b = basecolor.b * factor;

        return rgb;
    };

    //Change position and age of wave
    //Determine if its sill "alive"
    void update(uint32_t segment_length, uint32_t speed)
    {
        if (goingleft)
        {
            center -= speed_factor * speed;
        }
        else
        {
            center += speed_factor * speed;
        }

        age++;

        if (age > ttl)
        {
            alive = false;
        }
        else
        {
            if (goingleft)
            {
                if (center + width < 0)
                {
                    alive = false;
                }
            }
            else
            {
                if (center - width > segment_length)
                {
                    alive = false;
                }
            }
        }
    };

    bool stillAlive()
    {
        return alive;
    };
};

void mode_kaleidoscope_aurora()
{
    //aux1 = Wavecount
    //aux2 = Intensity in last loop

    AuroraWave *waves;

    if (SEGENV.aux0 != SEGMENT.intensity || SEGENV.call == 0)
    {
        //Intensity slider changed or first call
        SEGENV.aux1 = ((float)SEGMENT.intensity / 255) * W_MAX_COUNT;
        SEGENV.aux0 = SEGMENT.intensity;

        if (!SEGENV.allocateData(sizeof(AuroraWave) * SEGENV.aux1))
        {
            return mode_static(); //allocation failed
        }

        waves = reinterpret_cast<AuroraWave *>(SEGENV.data);

        for (int i = 0; i < SEGENV.aux1; i++)
        {
            waves[i].init(SEGLEN, col_to_crgb(color_from_palette(random8(), false, false, random(0, 3))));
        }
    }
    else
    {
        waves = reinterpret_cast<AuroraWave *>(SEGENV.data);
    }

    for (int i = 0; i < SEGENV.aux1; i++)
    {
        //Update values of wave
        waves[i].update(SEGLEN, SEGMENT.speed);

        if (!(waves[i].stillAlive()))
        {
            //If a wave dies, reinitialize it starts over.
            waves[i].init(SEGLEN, col_to_crgb(color_from_palette(random8(), false, false, random(0, 3))));
        }
    }

    //Loop through LEDs to determine color
    for (int i = 0; i < SEGLEN; i++)
    {
        CRGB mixedRgb = CRGB(BACKLIGHT, BACKLIGHT, BACKLIGHT);

        //For each LED we must check each wave if it is "active" at this position.
        //If there are multiple waves active on a LED we multiply their values.
        for (int j = 0; j < SEGENV.aux1; j++)
        {
            CRGB rgb = waves[j].getColorForLED(i);

            if (rgb != CRGB(0))
            {
                mixedRgb += rgb;
            }
        }

        setPixelColor(i, mixedRgb[0], mixedRgb[1], mixedRgb[2], BACKLIGHT);
    }
}
