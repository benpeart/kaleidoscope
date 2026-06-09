#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
// flags to enable turning of various parts of the app for debugging purposes
#define BOUNCE
#define DEMO
#define ENCODER
// #define STATEMACHINEENCODER
// #define PHOTOCELL

// don't include components that require WiFi unless it is included
#define WIFI
#ifdef WIFI
#define REST
#define TIME
// #define WEATHER
#endif // WIFI

#ifdef ENCODER
#ifdef STATEMACHINEENCODER
#include <ESP32StateMachineEncoder.h>
#else
#include <ESP32Encoder.h>
#endif
extern ESP32Encoder knobRight;
extern ESP32Encoder knobLeft;
#endif

// update the FastLED brightness based on our ambient and manual settings only if requested (using the right knob)
#define MIN_BRIGHTNESS 32  // the minimum brightness we want (above zero so it doesn't go completely dark)
#define MAX_BRIGHTNESS 255 // the max possible brightness
void adjustBrightness(bool useKnob = true);

// 'speed' ranges from 0-255 with the default speed being 128 and two rotations of the knob being required to move
// from min to max. Each mode can map() that to the correct min/max speed range and value required to make smaller
// numbers (rotate left) being slower and larger numbers (rotate right) being faster
#define MIN_SPEED 0
#define MAX_SPEED 255
int adjustSpeed();

#endif // MAIN_H
