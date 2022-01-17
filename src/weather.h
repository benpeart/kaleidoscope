#ifndef WEATHER_H
#define WEATHER_H

#include "Kaleidoscope.h"

struct weather {
    uint8_t iconCode;
    uint8_t temp;
};
extern weather current_weather;

void weather_setup(weather &w);
void weather_loop(weather &w);

void weather_draw_icon(weather &w);

#endif // WEATHER_H
