#define DEBUG

#include "debug.h"
#include "LEDStrips.h"
#include "Kaleidoscope.h"
#include "RealTimeClock.h"
#include "seven-segment.h"

// These are used for 'debouncing' the left & right button inputs,
// for switching between modes.
boolean prevStateLeft, prevStateRight;
uint32_t lastChangeTimeLeft = 0, lastChangeTimeRight = 0;
#define DEBOUNCE_MS 15 // Button debounce time, in milliseconds

// This array lists each of the display/animation drawing functions
// (which appear later in this code) in the order they're selected with
// the right button.  Some functions appear repeatedly...for example,
// we return to "mode_off" at several points in the sequence.
void (*renderFunc[])(void){
    mode_off, // Starts here, with LEDs off
    mode_kaleidoscope_screensaver,
    mode_kaleidoscope_interactive,
    mode_color_wash,
    mode_snowflake,
    mode_off, // make it obvious we're entering 'setup' modes
    mode_set_brightness,
    mode_set_clock
#ifdef DEBUG
    ,
    mode_off, // make it obvious we're entering 'demo' modes
    mode_HSV_wash,
    mode_rainbow,
    mode_rainbowCycle,
    mode_theaterChase,
    mode_theaterChaseRainbow
#endif    
    };
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 9; // FIX THIS Index of current mode in table

LEDStrips leds;
RealTimeClock clock;
Kaleidoscope kaleidoscope;

void setup()
{
#ifdef DEBUG
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
  leds.setup();

  // intialize the real time clock
  clock.setup();

  // initialize the kaleidoscope
  kaleidoscope.setup();
}

void loop()
{
  // automatically adjust the brightness of the LED strips to match the ambient lighting
  leds.adjustBrightness();

  // Read and debounce left button
  uint32_t t = millis();
  if ((t - lastChangeTimeLeft) >= DEBOUNCE_MS)
  {
    boolean b = 0; /*// FIX THIS CircuitPlayground.leftButton();*/

    // Left button state changed?
    if (b != prevStateLeft)
    {
      prevStateLeft = b;
      lastChangeTimeLeft = t;

      // Left button pressed?
      if (b)
      {
        if (mode)
          mode--; // Go to prior mode
        else
          mode = N_MODES - 1; // or "wrap around" to last mode
      }
    }
  }

  // Read and debounce right button
  if ((t - lastChangeTimeRight) >= DEBOUNCE_MS)
  {
    boolean b = 0; /*FIX THIS CircuitPlayground.rightButton();*/

    // Right button state changed?
    if (b != prevStateRight)
    {
      prevStateRight = b;
      lastChangeTimeRight = t;

      // Right button pressed?
      if (b)
      {
        if (mode < (N_MODES - 1))
          mode++; // Advance to next mode
        else
          mode = 0; // or "wrap around" to start
      }
    }
  }

  // Render one frame in current mode
  (*renderFunc[mode])();

  // render any overlay
  // update the clock display
  //  clock.loop();

  // update the led strips to show the current frame
  for (int x = 0; x < LED_STRIPS; x++)
    leds.strip[x].show();

  //  delay(wait);
}

// All Pixels off
void mode_off()
{
  DB_PRINTLN("mode_off");

  for (int x = 0; x < LED_STRIPS; x++)
    leds.strip[x].clear();
}

void mode_kaleidoscope_screensaver()
{
  DB_PRINTLN("mode_kaleidoscope_screensaver");

  // animate and draw the kaleidoscope
  kaleidoscope.loop();
}

void mode_kaleidoscope_interactive()
{
  DB_PRINTLN("mode_kaleidoscope_interactive");
}

void mode_color_wash()
{
  DB_PRINTLN("mode_color_wash");
}

void mode_snowflake()
{
  DB_PRINTLN("mode_snowflake");
}

void mode_set_brightness()
{
  DB_PRINTLN("mode_set_brightness");
}

void mode_set_clock()
{
  DB_PRINTLN("mode_set_clock");
}

#ifdef DEBUG

void mode_HSV_wash()
{
  DB_PRINTLN("mode_HSV_wash");

  // Some example procedures showing how to display to the pixels:
  for (long x = 0; x < 65535; x += 100)
  {
    leds.strip[0].fill(leds.strip[0].gamma32(leds.strip[0].ColorHSV(x)), 0, 150);
    leds.strip[0].show();
    delay(50);
  }
}

void mode_rainbow()
{
  DB_PRINTLN("mode_rainbow");

  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < leds.strip[0].numPixels(); i++)
    {
      leds.strip[0].setPixelColor(i, Wheel((i + j) & 255));
    }
    leds.strip[0].show();
    delay(20);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void mode_rainbowCycle()
{
  DB_PRINTLN("mode_rainbowCycle");

  uint16_t i, j;

  // 5 cycles of all colors on wheel
  for (j = 0; j < 256 * 5; j++)
  {
    for (i = 0; i < leds.strip[0].numPixels(); i++)
    {
      leds.strip[0].setPixelColor(i, Wheel(((i * 256 / leds.strip[0].numPixels()) + j) & 255));
    }
    leds.strip[0].show();
    delay(20);
  }
}

//Theatre-style crawling lights.
void mode_theaterChase()
{
  DB_PRINTLN("mode_theaterChase");

  // do 10 cycles of chasing
  for (int j = 0; j < 10; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < leds.strip[0].numPixels(); i = i + 3)
      {
        leds.strip[0].setPixelColor(i + q, 0x0f0f0f);
      }
      leds.strip[0].show();

      delay(20);

      // turn every third pixel off
      for (uint16_t i = 0; i < leds.strip[0].numPixels(); i = i + 3)
      {
        leds.strip[0].setPixelColor(i + q, 0);
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void mode_theaterChaseRainbow()
{
  DB_PRINTLN("mode_theaterChaseRainbow");

  // cycle all 256 colors in the wheel
  for (int j = 0; j < 256; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      // turn every third pixel on
      for (uint16_t i = 0; i < leds.strip[0].numPixels(); i = i + 3)
      {
        leds.strip[0].setPixelColor(i + q, Wheel((i + j) % 255));
      }
      leds.strip[0].show();

      delay(20);

      // turn every third pixel off
      for (uint16_t i = 0; i < leds.strip[0].numPixels(); i = i + 3)
      {
        leds.strip[0].setPixelColor(i + q, 0);
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
    return leds.strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return leds.strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

#endif // DEBUG
