#include <Arduino.h>

// flags to enable turning of various parts of the app for debugging purposes
//#define DEBUG
#define BOUNCE
#define DEMO
#define ENCODER
#define PHOTOCELL

// don't include components that require WiFi unless it is included
#define WIFI
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

// update the FastLED brightness based on our ambient and manual settings only if requested (using the right knob)
void adjustBrightness(bool useKnob = true);

// update the speed of the animations using the left knob
#define DEFAULT_SPEED_DELAY 500
#define MAX_SPEED_DELAY 1000
extern int ms_between_frames;
int adjustSpeed();
