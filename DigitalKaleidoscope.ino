#include <Encoder.h>

#define DEBUG

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//

#define PHOTOCELL_PIN 1 // the cell and 10K pulldown are connected to a0

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: CLK and DT pins have interrupt capability
//   Good Performance: only CLK pins have interrupt capability
//   Low Performance:  neither pin has interrupt capability
#define ENCODER_CLK_PIN_RIGHT 2 // the CLK pins neeed to be on iterrupts (pins 2 and 3 for Arduino UNO)
#define ENCODER_CLK_PIN_LEFT 3
#define ENCODER_DIRECTION_PIN_RIGHT 4
#define ENCODER_DIRECTION_PIN_LEFT 5

#define ENCODER_BUTTON_PIN_RIGHT 6
#define ENCODER_BUTTON_PIN_LEFT 7
#define LED_STRIPS_PIN_BASE 8
#define LED_STRIPS 4
#define PIXELS_PER_STRIP 150

#include "debug.h"
#include "LEDStrips.h"
#include "Kaleidoscope.h"
#include "RealTimeClock.h"
#include "seven-segment.h"

// This array lists each of the display/animation drawing functions
// (which appear later in this code) in the order they're selected with
// the right button.  Some functions appear repeatedly...for example,
// we return to "mode_off" at several points in the sequence.
void (*renderFunc[])(void){
    mode_kaleidoscope_screensaver,
    mode_kaleidoscope_interactive,
    mode_color_wash,
    mode_snowflake,
    mode_off, // make it obvious we're entering 'setup' modes
    mode_kaleidoscope_select_disks,
    mode_select_clock_face,
    mode_set_clock,
    mode_set_brightness,
#ifdef DEBUG
    mode_off, // make it obvious we're entering 'demo' modes
    mode_HSV_wash,
    mode_rainbow,
    mode_rainbowCycle,
    //    mode_theaterChase,  // comment out these two as they are not interactive
    //    mode_theaterChaseRainbow,
    mode_kaleidoscope_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 0; // Index of current mode in table

//
// GLOBAL VARIABLES --------------------------------------------------------
//

Encoder knobRight(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
Encoder knobLeft(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);

// These are used for 'debouncing' the left & right button inputs,
// for switching between modes.
boolean prevStateLeft, prevStateRight;
uint32_t lastChangeTimeLeft = 0, lastChangeTimeRight = 0;
#define DEBOUNCE_MS 15 // Button debounce time, in milliseconds

// global instances of objects
LEDStrips leds;
RealTimeClock clock;
Kaleidoscope kaleidoscope;

//
// SETUP FUNCTION -- RUNS ONCE AT PROGRAM START ----------------------------
//
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

  // initialize the rotary encoder buttons
  pinMode(ENCODER_BUTTON_PIN_RIGHT, INPUT);
  digitalWrite(ENCODER_BUTTON_PIN_RIGHT, HIGH); //turn pullup resistor on
  pinMode(ENCODER_BUTTON_PIN_LEFT, INPUT);
  digitalWrite(ENCODER_BUTTON_PIN_LEFT, HIGH); //turn pullup resistor on

  // intialize the LED strips
  leds.setup();

  // intialize the real time clock
  clock.setup();

  // initialize the kaleidoscope
  kaleidoscope.setup();
}

//
// LOOP FUNCTION -- RUNS OVER AND OVER FOREVER -----------------------------
//
void loop()
{
  static long positionLeft = -9999;
  static long positionRight = -9999;
  long newLeft, newRight;

  // automatically adjust the brightness of the LED strips to match the ambient lighting
  leds.adjustBrightness();

  // Read and debounce left button
  uint32_t t = millis();
  if ((t - lastChangeTimeLeft) >= DEBOUNCE_MS)
  {
    int temp = digitalRead(ENCODER_BUTTON_PIN_LEFT);
    boolean b = temp ? false : true;

    // Left button state changed?
    if (b != prevStateLeft)
    {
      DB_PRINTLN("b != prevStateLeft");
      prevStateLeft = b;
      lastChangeTimeLeft = t;

      // Left button pressed?
      if (b)
      {
        if (mode)
          mode--; // Go to prior mode
        else
          mode = N_MODES - 1; // or "wrap around" to last mode

        // clear the led strips for the new mode
        for (int x = 0; x < LED_STRIPS; x++)
          leds.strip[x].clear();
      }
    }
  }

  // Read and debounce right button
  if ((t - lastChangeTimeRight) >= DEBOUNCE_MS)
  {
    boolean b = !digitalRead(ENCODER_BUTTON_PIN_RIGHT);

    // Right button state changed?
    if (b != prevStateRight)
    {
      DB_PRINTLN("b != prevStateRight");
      prevStateRight = b;
      lastChangeTimeRight = t;

      // Right button pressed?
      if (b)
      {
        if (mode < (N_MODES - 1))
          mode++; // Advance to next mode
        else
          mode = 0; // or "wrap around" to start

        // clear the led strips for the new mode
        for (int x = 0; x < LED_STRIPS; x++)
          leds.strip[x].clear();
      }
    }
  }

  // read and report on the knobs
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight)
  {
    DB_PRINT("Left = ");
    DB_PRINT(newLeft);
    DB_PRINT(", Right = ");
    DB_PRINT(newRight);
    DB_PRINTLN();
    positionLeft = newLeft;
    positionRight = newRight;
  }

  // Render one frame in current mode. To control the speed of updates, save the time the frame
  // was last displayed and only display the next frame when enough time has elapsed. See
  // mode_kaleidoscope_screensaver for an example.
  (*renderFunc[mode])();

  // render any overlay
  // update the clock display
  clock.loop();

  // update the led strips to show the current frame
  for (int x = 0; x < LED_STRIPS; x++)
    leds.strip[x].show();
}

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
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

#ifdef DEBUG

void mode_HSV_wash()
{
  static long x = 0;

  leds.strip[0].fill(leds.strip[0].gamma32(leds.strip[0].ColorHSV(x)), 0, 150);
  x += 100;
  if (x >= 65535)
    x = 0;
}

void mode_rainbow()
{
  static uint16_t j = 0;

  for (uint16_t i = 0; i < leds.strip[0].numPixels(); i++)
  {
    leds.strip[0].setPixelColor(i, Wheel((i + j) & 255));
  }

  j++;
  if (j >= 256)
    j = 0;
}

// Slightly different, this makes the rainbow equally distributed throughout
void mode_rainbowCycle()
{
  static uint16_t j;

  for (uint16_t i = 0; i < leds.strip[0].numPixels(); i++)
  {
    leds.strip[0].setPixelColor(i, Wheel(((i * 256 / leds.strip[0].numPixels()) + j) & 255));
  }

  j++;
  if (j >= 256 * 5)
    j = 0;
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
