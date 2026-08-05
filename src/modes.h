#ifndef MODES_H
#define MODES_H

// maximum lenth of a valid mode name
#define MAX_MODE_NAME 64

// Data structure to represent each mode the Kaleidoscope can be in.
struct KaleidoscopeMode
{
    void (*renderFunc)(void);           // pointer to the function that will render the mode
    const char modeName[MAX_MODE_NAME]; // name of the mode to use in the UI and REST APIs
    bool showInRESTAPI;                 // flag if the mode should be shown in the REST APIs
    int modeEncoderCounts[2];           // the rotary encoder counts for this mode
};

// This look up table lists each of the available modes.
extern KaleidoscopeMode KaleidoscopeModeLUT[];
extern uint8_t kaleidoscopeModes; // total number of valid modes in the LUT

void setKaleidoscopeMode(int newMode);
void nextKaleidoscopeMode();
void previousKaleidoscopeMode();

// Calls to setKaleidoscopeMode() can happen asynchronously from the main loop, so we need to
// make sure that we don't try to clear the pixels until the previous mode has finished drawing. 
// This flag is set to true when we enter a new mode in setKaleidoscopeMode and cleared in the main loop.
extern bool eraseLEDs;

extern void mode_off();

#endif // MODES_H
