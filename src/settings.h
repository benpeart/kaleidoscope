#ifndef SETTINGS_H
#define SETTINGS_H

#include "main.h"
#include "Kaleidoscope.h"
#ifdef DEMO
#include "plasma.h"
#include "ripples.h"
#include "TwinkleFox.h"
#include "XYDistortionWaves.h"
#include "XYAALines.h"
#include "XYmatrix.h"
#include "XYpacifica.h"
#include "XYsnake.h"
#include "XYfire.h"
#endif

#ifdef TIME
#include "RealTimeClock.h"
#endif // TIME

#ifndef DEBUG
#ifndef WIFI
#define N_MODES 15
#else
#define N_MODES 16
#endif
#else
#define N_MODES 19
#endif
extern uint8_t kaleidoscope_mode; // Index of current mode in table
extern const char modeNames[N_MODES][64];
extern const PROGMEM char showInRESTAPI[N_MODES];

#endif // SETTINGS_H
