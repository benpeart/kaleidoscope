#include "main.h"
#include "render.h"
#include "pulse.h"

uint8_t bloodHue = 96;       // Blood color [hue from 0-255]
uint8_t bloodSat = 255;      // Blood staturation [0-255]
int flowDirection = -1;      // Use either 1 or -1 to set flow direction
uint16_t cycleLength = 1500; // Lover values = continuous flow, higher values = distinct pulses.
uint16_t pulseLength = 150;  // How long the pulse takes to fade out.  Higher value is longer.
uint16_t pulseOffset = 200;  // Delay before second pulse.  Higher value is more delay.
uint8_t baseBrightness = 10; // Brightness of LEDs when not pulsing. Set to 0 for off.

//===============================================================
// Heart Beat Functions
//   The base for all this goodness came from Mark Kriegsman and
//   was initially coded up by Chris Thodey.  I updated it to use
//   HSV and provided all the variables to play with up above.
//   -Marc

uint8_t pulseWave8(uint32_t ms, uint16_t cycleLength, uint16_t pulseLength)
{
    uint16_t T = ms % cycleLength;
    if (T > pulseLength)
        return baseBrightness;
    uint16_t halfPulse = pulseLength / 2;
    if (T <= halfPulse)
    {
        return (T * 255) / halfPulse; //first half = going up
    }
    else
    {
        return ((pulseLength - T) * 255) / halfPulse; //second half = going down
    }
}

int sumPulse(int time_shift)
{
    //time_shift = 0;  //Uncomment to heart beat/pulse all LEDs together
    int pulse1 = pulseWave8(millis() + time_shift, cycleLength, pulseLength);
    int pulse2 = pulseWave8(millis() + time_shift + pulseOffset, cycleLength, pulseLength);
    return qadd8(pulse1, pulse2); // Add pulses together without overflow
}

void mode_heartBeat()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        uint8_t bloodVal = sumPulse((5 / NUM_LEDS / 2) + (NUM_LEDS / 2) * i * flowDirection);
        leds[i] = CHSV(bloodHue, bloodSat, bloodVal);
    }
    leds_dirty = true;
}
