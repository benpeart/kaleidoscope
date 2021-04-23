#include "Arduino.h"

// https://github.com/PaulStoffregen/Encoder
#include <Encoder.h>

// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// https://github.com/FastLED/FastLED
#include <FastLED.h>

// https://github.com/PaulStoffregen/Time
#include <TimeLib.h>

// https://github.com/PaulStoffregen/OctoWS2811
#include <OctoWS2811.h>

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

template <EOrder RGB_ORDER = GRB, uint8_t CHIP = WS2811_800kHz>
class CTeensy4Controller : public CPixelLEDController<RGB_ORDER, 8, 0xFF>
{
  OctoWS2811 *pocto;

public:
  CTeensy4Controller(OctoWS2811 *_pocto)
      : pocto(_pocto){};

  virtual void init() {}
  virtual void showPixels(PixelController<RGB_ORDER, 8, 0xFF> &pixels)
  {

    uint32_t i = 0;
    while (pixels.has(1))
    {
      uint8_t r = pixels.loadAndScale0();
      uint8_t g = pixels.loadAndScale1();
      uint8_t b = pixels.loadAndScale2();
      pocto->setPixel(i++, r, g, b);
      pixels.stepDithering();
      pixels.advanceData();
    }

    pocto->show();
  }
};

// These buffers need to be large enough for all the pixels.
// The total number of pixels is "ledsPerStrip * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is
// 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
DMAMEM int displayMemory[NUM_LEDS_PER_STRIP * NUM_STRIPS * 3 / 4];
int drawingMemory[NUM_LEDS_PER_STRIP * NUM_STRIPS * 3 / 4];
byte pinList[NUM_STRIPS] = {19, 18, 14, 15};
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
OctoWS2811 octo(NUM_LEDS_PER_STRIP, displayMemory, drawingMemory, WS2811_RGB | WS2811_800kHz, NUM_STRIPS, pinList);
CTeensy4Controller<GRB, WS2811_800kHz> *pcontroller;

// The Teensy with parallel updates for the LEDs is so fast, we get flickering
// if we call FastLED.Show every loop. Maintain a 'dirty' bit so we know when
// to call Show.
boolean leds_dirty = true;

#include "debug.h"
#include "Kaleidoscope.h"
#include "ripples.h"
#include "RealTimeClock.h"

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

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
}

void mode_color_wash()
{
  fill_rainbow(leds, NUM_STRIPS * NUM_LEDS_PER_STRIP, 0, 20);
}

void mode_snowflake()
{
}

//
// BRIGHTNESS helpers -------------------------------------------------
//

#define KNOB_MULTIPLIER 10

// automatically adjust the brightness of the LED strips to match the ambient lighting
void adjustBrightness()
{
  // store the current LED brightness so we can minimize minor differences
  static int LEDbrightness = 0;
  static int lastKnob = 0;
  static int LEDBrightnessManualOffset = 0;

  // check the photocell and debounce it a bit as it moves around a lot
  // it is currently setup to use the internal pullup resistor so we need to invert it's value
  static int lastPhotocell = 0;
  int photocellReading = 1023 - analogRead(PHOTOCELL_PIN);
  if ((photocellReading > lastPhotocell + 75) || (photocellReading < lastPhotocell - 75))
  {
    lastPhotocell = photocellReading;

    DB_PRINT(F("photocell reading = "));
    DB_PRINTLN(photocellReading);
  }

  // use the right knob as a brightness increment/decrement
  int knob = knobRight.read();
  if (knob != lastKnob)
  {
    if (knob > lastKnob)
      LEDBrightnessManualOffset -= KNOB_MULTIPLIER;
    else
      LEDBrightnessManualOffset += KNOB_MULTIPLIER;

    LEDBrightnessManualOffset = constrain(LEDBrightnessManualOffset, -1023, 1023);
    lastKnob = knob;

    DB_PRINT(F("LEDBrightnessManualOffset = "));
    DB_PRINTLN(LEDBrightnessManualOffset);
  }

  // map our total brightness from 0-1023 to 0-255 since that is the range for setBrightness
  int newBrightness = map(lastPhotocell + LEDBrightnessManualOffset, 0, 1023, 0, 255);
  newBrightness = constrain(newBrightness, 0, 255);

  // adjust our brightness if it has changed
  if (newBrightness != LEDbrightness)
  {
#ifdef DEBUG
    /*    
    DB_PRINTLN(LEDBrightnessManualOffset);*/
    DB_PRINT(F("new brightness = "));
    DB_PRINTLN(newBrightness);
#endif

    LEDbrightness = newBrightness;
    FastLED.setBrightness(dim8_raw(LEDbrightness));
    leds_dirty = true;
  }
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
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_rainbowMarch,
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_sawTooth,
    mode_kaleidoscope_ripples,
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
#ifdef DEMO
        "mode_off",
        "mode_kaleidoscope_rainbowMarch",
        "mode_kaleidoscope_plasma",
        "mode_kaleidoscope_sawTooth",
        "mode_kaleidoscope_ripples",
        "mode_blendWave",
#endif
#ifdef DEBUG
        "mode_kaleidoscope_test",
#endif
        "mode_off"
#endif
};

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

  // initialize the random number generator using noise from an analog pin
  randomSeed(analogRead(PHOTOCELL_PIN));

  // initialize the photo resister using the pullup resistor
  pinMode(PHOTOCELL_PIN, INPUT_PULLUP);

  // intialize the real time clock
  clock.setup();

  // initialize the kaleidoscope
  kaleidoscope.setup();

  // initialize the rotary encoder buttons using the pullup resistor
  leftButton.attach(ENCODER_BUTTON_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_BUTTON_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
  DB_PRINTLN(modeNames[mode]);

  // intialize the LED strips for parallel output on the Teensy 4
  octo.begin();
  pcontroller = new CTeensy4Controller<GRB, WS2811_800kHz>(&octo);
  FastLED.addLeds(pcontroller, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP);
  FastLED.clear();
  leds_dirty = true;
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
    leds_dirty = true;
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
    leds_dirty = true;
  }

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  (*renderFunc[mode])();

  // update the clock overlay
  //  clock.loop();

  // if we have changes in the LEDs, show the updated frame
  if (leds_dirty)
  {
    FastLED.show();
    leds_dirty = false;
  }
}
