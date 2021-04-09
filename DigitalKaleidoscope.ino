#define __DEBUG__

#include "LEDStrips.h"
#include "Kaleidoscope.h"
#include "RealTimeClock.h"
#include "seven-segment.h"

Kaleidoscope kaleidoscope;
RealTimeClock clock;

void setup()
{
#ifdef __DEBUG__
  // 3 second delay for recovery
  delay(3000);

  Serial.begin(115200);
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
#endif
#endif

  // initialize the random number generator using noise from analog pin 5
  randomSeed(analogRead(5));

  // intialize the LED strips
  LEDs.setup();

  // intialize the real time clock
  clock.setup();

  // initialize the kaleidoscope
  kaleidoscope.setup();
}

void loop()
{
  // automatically adjust the brightness of the LED strips to match the ambient lighting
  LEDs.adjustBrightness();

  // animate and draw the kaleidoscope 
  kaleidoscope.loop();

  // update the clock display
  clock.loop();

#ifdef __NDEBUG__
  // Some example procedures showing how to display to the pixels:
  for (long x = 0; x < 65535; x += 100)
  {
    LEDs.strip[0].fill(LEDs.strip[0].gamma32(LEDs.strip[0].ColorHSV(x)), 0, 16);
    LEDs.strip[0].show();
    delay(50);
  }

  // Send a theater pixel chase in...
  theaterChase(LEDs.strip[0].Color(127, 127, 127), 50); // White
  theaterChase(LEDs.strip[0].Color(127, 0, 0), 50);     // Red
  theaterChase(LEDs.strip[0].Color(0, 0, 127), 50);     // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
#endif
}

#ifdef __NDEBUG__

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < LEDs.strip[0].numPixels(); i++)
  {
    LEDs.strip[0].setPixelColor(i, c);
    LEDs.strip[0].show();
    delay(wait);
  }
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < LEDs.strip[0].numPixels(); i++)
    {
      LEDs.strip[0].setPixelColor(i, Wheel((i + j) & 255));
    }
    LEDs.strip[0].show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  // 5 cycles of all colors on wheel
  for (j = 0; j < 256 * 5; j++)
  {
    for (i = 0; i < LEDs.strip[0].numPixels(); i++)
    {
      LEDs.strip[0].setPixelColor(i, Wheel(((i * 256 / LEDs.strip[0].numPixels()) + j) & 255));
    }
    LEDs.strip[0].show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
  // do 10 cycles of chasing
  for (int j = 0; j < 10; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < LEDs.strip[0].numPixels(); i = i + 3)
      {
        LEDs.strip[0].setPixelColor(i + q, c);
      }
      LEDs.strip[0].show();

      delay(wait);

      // turn every third pixel off
      for (uint16_t i = 0; i < LEDs.strip[0].numPixels(); i = i + 3)
      {
        LEDs.strip[0].setPixelColor(i + q, 0);
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
  // cycle all 256 colors in the wheel
  for (int j = 0; j < 256; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < LEDs.strip[0].numPixels(); i = i + 3)
      {
        LEDs.strip[0].setPixelColor(i + q, Wheel((i + j) % 255));
      }
      LEDs.strip[0].show();

      delay(wait);

      // turn every third pixel off
      for (uint16_t i = 0; i < LEDs.strip[0].numPixels(); i = i + 3)
      {
        LEDs.strip[0].setPixelColor(i + q, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return LEDs.strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return LEDs.strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return LEDs.strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

#endif // __DEBUG__
