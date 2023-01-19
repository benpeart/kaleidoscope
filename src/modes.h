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
extern uint8_t kaleidoscope_mode;  // Index of current mode in the LUT
extern uint8_t kaleidoscope_modes; // total number of valid modes in the LUT

void setKaleidoscopeMode(int new_mode);
void nextKaleidoscopeMode();
void previousKaleidoscopeMode();

extern void mode_off();

#endif // MODES_H
