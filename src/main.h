#include <Arduino.h>

// flags to enable turning of various parts of the app for debugging purposes
#define BOUNCE
#define DEMO
#define ENCODER
#define PHOTOCELL
#define WIFI
#ifdef WIFI
#define DRD
#define OTA
//#define ALEXA // can't get Alexa to discover my devices, from the issues, seems like this is a common problem
#define TIME
#endif

// enable debugging macros
#define DEBUG
#include "debug.h"

#ifdef ENCODER
// https://github.com/madhephaestus/ESP32Encoder
#include <ESP32Encoder.h>
#endif

// adjust the brightness of the LED strips to match the ambient lighting using a photo cell
// and rotary encoders for manual adjustment.
extern ESP32Encoder knobRight;
extern ESP32Encoder knobLeft;
void adjustBrightness();
