#include "Arduino.h"

// https://github.com/PaulStoffregen/Encoder
#include <Encoder.h>

// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// https://github.com/FastLED/FastLED
#include <FastLED.h>

#ifdef CLOCK
// https://github.com/adafruit/RTClib
#include <RTClib.h>
#endif

#define ENCODER
#define DEBUG
#define DEMO

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//
#ifdef PHOTOCELL
#define PHOTOCELL_PIN 1 // the cell and 10K pulldown are connected to a0
#endif

#ifdef ENCODER
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: CLK and DT pins have interrupt capability
//   Good Performance: only CLK pins have interrupt capability
//   Low Performance:  neither pin has interrupt capability
#define ENCODER_CLK_PIN_RIGHT 10
#define ENCODER_CLK_PIN_LEFT 0
#define ENCODER_DIRECTION_PIN_RIGHT 11
#define ENCODER_DIRECTION_PIN_LEFT 1

#define ENCODER_BUTTON_PIN_RIGHT 12
#define ENCODER_BUTTON_PIN_LEFT 2
#endif

#define LED_STRIPS_PIN_BASE 19
#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

#include "debug.h"
#include "Kaleidoscope.h"
#ifdef CLOCK
#include "RealTimeClock.h"
#endif

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

  speed = beatsin8(6, 0, 255);

  clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
  clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

  loc1 = beatsin8(10, 0, NUM_STRIPS * NUM_LEDS_PER_STRIP - 1);

  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_STRIPS * NUM_LEDS_PER_STRIP - 1, clr1);
}

// https://github.com/atuline/FastLED-Demos/blob/master/rainbow_march/rainbow_march.ino
void mode_rainbowMarch()
{
  uint8_t thisdelay = 200, deltahue = 10;
  uint8_t thishue = millis() * (255 - thisdelay) / 255; // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.

  // thishue = beat8(50);           // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
  // thishue = beatsin8(50,0,255);  // This can change speeds on the fly. You can also add these to each other.

  fill_rainbow(leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, thishue, deltahue);
}

// https://github.com/atuline/FastLED-Demos/blob/master/sawtooth/sawtooth.ino
void mode_sawTooth()
{
  // Palette definitions
  static CRGBPalette16 currentPalette = PartyColors_p;
  static TBlendType currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND

  int bpm = 60;
  int ms_per_beat = 60000 / bpm; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
  int ms_per_led = 60000 / bpm / NUM_STRIPS * NUM_LEDS_PER_STRIP;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (NUM_STRIPS * NUM_LEDS_PER_STRIP); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
    fill_solid(leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending); // I prefer to use palettes instead of CHSV or CRGB assignments.
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
#ifdef CLOCK
    mode_select_clock_face,
    mode_set_clock,
#endif
    mode_set_brightness,
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_rainbowMarch,
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_sawTooth,
    mode_blendWave,
    mode_rainbowMarch,
    mode_sawTooth,
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
#ifdef CLOCK
        "mode_select_clock_face",
        "mode_set_clock",
#endif
        "mode_set_brightness",
#ifdef DEMO
        "mode_off",
        "mode_kaleidoscope_rainbowMarch",
        "mode_kaleidoscope_plasma",
        "mode_kaleidoscope_sawTooth",
        "mode_blendWave",
        "mode_rainbowMarch",
        "mode_sawTooth",
#endif
#ifdef DEBUG
        "mode_kaleidoscope_test",
#endif
        "mode_off"
#endif
};

// global instances of objects
#ifdef CLOCK
RealTimeClock clock;
#endif
Kaleidoscope kaleidoscope;

#ifdef ENCODER
// Instantiate Button objects from the Bounce2 namespace
Bounce2::Button leftButton = Bounce2::Button();
Bounce2::Button rightButton = Bounce2::Button();
#define DEBOUNCE_MS 5 // Button debounce time, in milliseconds

// Instantiate rotary encoder knob objects
Encoder knobRight(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
Encoder knobLeft(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);
#endif

#ifdef PHOTOCELL
// automatically adjust the brightness of the LED strips to match the ambient lighting
void adjustBrightness()
{
  // store the current LED brightness so we can minimize minor differences
  static int LEDbrightness = 0;

  // check the photocell and map 0-1023 to 0-255 since that is the range for setBrightness
  int photocellReading = analogRead(PHOTOCELL_PIN);
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
  }
}
#endif

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

#ifdef ENCODER
  // initialize the rotary encoder buttons using the pullup resistor
  leftButton.attach(ENCODER_BUTTON_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_BUTTON_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
#endif
  DB_PRINTLN(modeNames[mode]);

  // intialize the LED strips for parallel output on the Teensy 4
  // https://github.com/FastLED/FastLED/wiki/Parallel-Output#parallel-output-on-the-teensy-4
  FastLED.addLeds<NUM_STRIPS, WS2812B, LED_STRIPS_PIN_BASE, GRB>(leds, NUM_LEDS_PER_STRIP);
  FastLED.clear();
  FastLED.setBrightness(32);
#ifdef CLOCK
  // intialize the real time clock
  clock.setup();
#endif

  // initialize the kaleidoscope
  kaleidoscope.setup();
}

//
// LOOP FUNCTION -- RUNS OVER AND OVER FOREVER -----------------------------
//
void loop()
{
#ifdef PHOTOCELL
  // automatically adjust the brightness of the LED strips to match the ambient lighting
  adjustBrightness();
#endif

#ifdef ENCODER
  // Update the buttons
  leftButton.update();
  rightButton.update();

  // Handle button pressed?
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
  static long positionLeft = -9999;
  static long positionRight = -9999;
  long newLeft = knobLeft.read();
  long newRight = knobRight.read();
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
#endif

  // Render one frame in current mode. To control the speed of updates, save the time the frame
  // was last displayed and only display the next frame when enough time has elapsed. See
  // mode_kaleidoscope_screensaver for an example.
  (*renderFunc[mode])();

  // render any overlay
#ifdef CLOCK
  // update the clock display
  clock.loop();
#endif

  // update the led strips to show the current frame
  EVERY_N_MILLISECONDS(250)
  {
    FastLED.show();
  }
}
