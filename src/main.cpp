#include "Arduino.h"

// https://github.com/PaulStoffregen/Encoder
#include <Encoder.h>

// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// https://github.com/FastLED/FastLED
#include <FastLED.h>

// https://github.com/PaulStoffregen/Time
#include <TimeLib.h>

#define DEBUG
#define DEMO

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//
#define PHOTOCELL_PIN A9

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: CLK and DT pins have interrupt capability
//   Good Performance: only CLK pins have interrupt capability
//   Low Performance:  neither pin has interrupt capability
#define ENCODER_CLK_PIN_LEFT 0
#define ENCODER_DIRECTION_PIN_LEFT 1
#define ENCODER_BUTTON_PIN_LEFT 2
#define ENCODER_CLK_PIN_RIGHT 10
#define ENCODER_DIRECTION_PIN_RIGHT 11
#define ENCODER_BUTTON_PIN_RIGHT 12

// setup our LED strips for parallel output using FastLED
#define LED_STRIPS_PIN_BASE 19
#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156

// The Teensy with parallel updates for the LEDs is so fast, we get flickering
// if we call FastLED.Show every loop. Maintain a 'dirty' bit so we know when
// to call Show.
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
boolean leds_dirty = false;

#include "debug.h"
#include "Kaleidoscope.h"
#include "RealTimeClock.h"

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
}

void mode_color_wash()
{
}

void mode_snowflake()
{
}

void mode_set_brightness()
{
}

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/blendwave/blendwave.ino
void mode_blendWave()
{
  static CRGB clr1;
  static CRGB clr2;
  static uint8_t speed;
  static uint8_t loc1;

  EVERY_N_MILLISECONDS(50)
  {
    speed = beatsin8(6, 0, 255);

    clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
    clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

    loc1 = beatsin8(10, 0);

    fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
    fill_gradient_RGB(leds, loc1, clr2, NUM_STRIPS * NUM_LEDS_PER_STRIP - 1, clr1);
    leds_dirty = true;
  }
}

#endif // DEMO

//
// GLOBAL VARIABLES --------------------------------------------------------
//

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
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_rainbowMarch,
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_sawTooth,
    mode_blendWave,
#endif
#ifdef DEBUG
    mode_kaleidoscope_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 0; // Index of current mode in table

#ifdef DEBUG
const char modeNames[N_MODES][64] =
    {
        "mode_kaleidoscope_screensaver",
        "mode_kaleidoscope_interactive",
        "mode_color_wash",
        "mode_snowflake",
        "mode_off",
        "mode_kaleidoscope_select_disks",
        "mode_select_clock_face",
        "mode_set_clock",
        "mode_set_brightness",
#ifdef DEMO
        "mode_off",
        "mode_kaleidoscope_rainbowMarch",
        "mode_kaleidoscope_plasma",
        "mode_kaleidoscope_sawTooth",
        "mode_blendWave",
#endif
#ifdef DEBUG
        "mode_kaleidoscope_test",
#endif
        "mode_off"
#endif
};

// global instances of objects
RealTimeClock clock;
Kaleidoscope kaleidoscope;

// Instantiate Button objects from the Bounce2 namespace
Bounce2::Button leftButton = Bounce2::Button();
Bounce2::Button rightButton = Bounce2::Button();
#define DEBOUNCE_MS 5 // Button debounce time, in milliseconds

// Instantiate rotary encoder knob objects
Encoder knobRight(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
Encoder knobLeft(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);

// automatically adjust the brightness of the LED strips to match the ambient lighting
void adjustBrightness()
{
  // store the current LED brightness so we can minimize minor differences
  static int LEDbrightness = 0;

  // check the photocell and map 0-1023 to 0-255 since that is the range for setBrightness
  // it is currently setup to use the internal pullup resistor so we need to invert and reading
  int photocellReading = 1023 - analogRead(PHOTOCELL_PIN);
  int newBrightness = map(photocellReading, 0, 1023, 0, 255);

  // adjust our brightness if it has changed significantly
  if ((newBrightness > LEDbrightness + 5) || (newBrightness < LEDbrightness - 5))
  {
    DB_PRINT(F("Analog photocell reading = "));
    DB_PRINTLN(photocellReading); // the raw analog reading
    DB_PRINT(F("new brightness = "));
    DB_PRINTLN(newBrightness);

    LEDbrightness = newBrightness;
    FastLED.setBrightness(LEDbrightness);
    leds_dirty = true;
  }
}

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

  // initialize the photo resister using the pullup resistor
  pinMode(PHOTOCELL_PIN, INPUT_PULLUP);

  // initialize the rotary encoder buttons using the pullup resistor
  leftButton.attach(ENCODER_BUTTON_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_BUTTON_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
  DB_PRINTLN(modeNames[mode]);

  // intialize the LED strips for parallel output on the Teensy 4
  // https://github.com/FastLED/FastLED/wiki/Parallel-Output#parallel-output-on-the-teensy-4
  FastLED.addLeds<NUM_STRIPS, WS2812B, LED_STRIPS_PIN_BASE, GRB>(leds, NUM_LEDS_PER_STRIP);
  FastLED.clear();

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
  // automatically adjust the brightness of the LED strips to match the ambient lighting
  adjustBrightness();

  // Left button pressed?
  leftButton.update();
  if (leftButton.pressed())
  {
    if (mode)
      mode--; // Go to prior mode
    else
      mode = N_MODES - 1; // or "wrap around" to last mode
    DB_PRINTLN(modeNames[mode]);

    // clear the led strips for the new mode
    FastLED.clear();
  }

  // Right button pressed?
  rightButton.update();
  if (rightButton.pressed())
  {
    if (mode < (N_MODES - 1))
      mode++; // Advance to next mode
    else
      mode = 0; // or "wrap around" to start
    DB_PRINTLN(modeNames[mode]);

    // clear the led strips for the new mode
    FastLED.clear();
  }

  // read and report on the knobs
  static int32_t positionLeft = -9999;
  static int32_t positionRight = -9999;
  int32_t newLeft = knobLeft.read();
  int32_t newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight)
  {
    DB_PRINT(F("Left = "));
    DB_PRINT(newLeft);
    DB_PRINT(F(", Right = "));
    DB_PRINT(newRight);
    DB_PRINTLN();
    positionLeft = newLeft;
    positionRight = newRight;
  }

  // Render one frame in current mode. To control the speed of updates, save the time the frame
  // was last displayed and only display the next frame when enough time has elapsed. See
  // mode_kaleidoscope_screensaver for an example.
  (*renderFunc[mode])();

  // update the clock overlay
  clock.loop();

  // if we have changes in the LEDs, show the current frame
  if (leds_dirty)
  {
    FastLED.show();
    leds_dirty = false;
  }
}
