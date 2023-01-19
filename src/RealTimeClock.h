#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include "render.h"

// maximum lenth of a valid mode name
#define MAX_FACE_NAME 16

/// Data structure to represent each possible clock face.
struct ClockFace
{
    void (*renderFunc)(void);           // pointer to the function that will render the clock face
    const char faceName[MAX_FACE_NAME]; // name of the clock face to use in the UI and REST APIs
};

// This look up table lists each of the available modes.
extern ClockFace clockFaceLUT[];
extern uint8_t clockFace; // Index of current clock face in table
extern uint8_t clockFaces; // Index of current clock face in table

int setClockFace(int newFace);
extern CRGB clockColor;

void drawClock();
void mode_select_clock_face();

void rtc_setup();

#endif // REALTIMECLOCK_H
