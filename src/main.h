#include <Arduino.h>

// flags to enable turning of various parts of the app for debugging purposes
#define DEBUG
#define BOUNCE
#define DEMO
#define ENCODER
#define PHOTOCELL

// don't include components that require WiFi unless it is included
//#define WIFI
#ifdef WIFI
#define DRD
#define OTA
//#define ALEXA // can't get Alexa to discover my devices, from the issues, seems like this is a common problem
#define TIME
#endif

// include debugging macros
#include "debug.h"

#ifdef ENCODER
#ifndef STATEMACHINEENCODER
#include <ESP32StateMachineEncoder.h>
#else
#include <ESP32Encoder.h>
#endif
extern ESP32Encoder knobRight;
extern ESP32Encoder knobLeft;
#endif

// adjust the brightness of the LED strips to match the ambient lighting using a photo cell
// and rotary encoders for manual adjustment.
void adjustBrightness();
