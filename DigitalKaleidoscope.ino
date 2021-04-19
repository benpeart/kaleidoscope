
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

#define DEBUG
#define DEMO

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

// Change this to be a pin that has hardware SPI support
// On the Arduino UNO this is pin 11
// On the Teensey these are pins 11-13 with the Chip Select signal on Pin 10
#define LED_STRIPS_PIN_BASE 11
#define NUM_STRIPS 1 // FIX THIS (when we have more space)!
#define NUM_LEDS_PER_STRIP 156

#include "debug.h"
#include "LEDStrips.h"
#include "Kaleidoscope.h"
#ifdef CLOCK
#include "RealTimeClock.h"
#endif
//#include "seven-segment.h"

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
    mode_blendWave,
    mode_rainbowMarch,
    mode_sawTooth,
    mode_plasma,
#endif
#ifdef DEBUG
    mode_kaleidoscope_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 0; // Index of current mode in table

//
// GLOBAL VARIABLES --------------------------------------------------------
//

// global instances of objects
LEDStrips leds;
#ifdef CLOCK
RealTimeClock clock;
#endif
Kaleidoscope kaleidoscope;

// Instantiate Button objects from the Bounce2 namespace
Bounce2::Button leftButton = Bounce2::Button();
Bounce2::Button rightButton = Bounce2::Button();
#define DEBOUNCE_MS 5 // Button debounce time, in milliseconds

// Instantiate rotary encoder knob objects
Encoder knobRight(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
Encoder knobLeft(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);

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

  // initialize the rotary encoder buttons using the pullup resistor
  leftButton.attach(ENCODER_BUTTON_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_BUTTON_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);

  // intialize the LED strips
  leds.setup();

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
  // automatically adjust the brightness of the LED strips to match the ambient lighting
  leds.adjustBrightness();

  // Update the buttons
  leftButton.update();
  rightButton.update();

  // Handle button pressed?
  if (leftButton.pressed())
  {
    DB_PRINTLN(F("leftButton.pressed"));

    if (mode)
      mode--; // Go to prior mode
    else
      mode = N_MODES - 1; // or "wrap around" to last mode

    // clear the led strips for the new mode
    FastLED.clear();
  }

  // Right button pressed?
  if (rightButton.pressed())
  {
    DB_PRINTLN(F("rightButton.pressed"));

    if (mode < (N_MODES - 1))
      mode++; // Advance to next mode
    else
      mode = 0; // or "wrap around" to start

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
  FastLED.show();
}

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
}

void mode_color_wash()
{
  DB_PRINTLN(F("mode_color_wash"));
}

void mode_snowflake()
{
  DB_PRINTLN(F("mode_snowflake"));
}

void mode_set_brightness()
{
  DB_PRINTLN(F("mode_set_brightness"));
}

#ifdef DEMO

// https://github.com/atuline/FastLED-Demos/blob/master/blendwave/blendwave.ino
void mode_blendWave()
{
  //DB_PRINTLN(F("mode_blendWave"));

  static CRGB clr1;
  static CRGB clr2;
  static uint8_t speed;
  static uint8_t loc1;

  speed = beatsin8(6, 0, 255);

  clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
  clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

  loc1 = beatsin8(10, 0, NUM_LEDS_PER_STRIP - 1);

  fill_gradient_RGB(leds.strip[0], 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds.strip[0], loc1, clr2, NUM_LEDS_PER_STRIP - 1, clr1);
}

// https://github.com/atuline/FastLED-Demos/blob/master/rainbow_march/rainbow_march.ino
void mode_rainbowMarch()
{
  //DB_PRINTLN(F("mode_rainbowMarch"));

  uint8_t thisdelay = 200, deltahue = 10;
  uint8_t thishue = millis() * (255 - thisdelay) / 255; // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.

  // thishue = beat8(50);           // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
  // thishue = beatsin8(50,0,255);  // This can change speeds on the fly. You can also add these to each other.

  fill_rainbow(leds.strip[0], NUM_LEDS_PER_STRIP, thishue, deltahue);
}

// https://github.com/atuline/FastLED-Demos/blob/master/sawtooth/sawtooth.ino
void mode_sawTooth()
{
  //DB_PRINTLN(F("mode_sawTooth"));

  // Palette definitions
  static CRGBPalette16 currentPalette = PartyColors_p;
  static CRGBPalette16 targetPalette = PartyColors_p;
  static TBlendType currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND

  int bpm = 60;
  int ms_per_beat = 60000 / bpm; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
  int ms_per_led = 60000 / bpm / NUM_LEDS_PER_STRIP;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (NUM_LEDS_PER_STRIP); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
    fill_solid(leds.strip[0], NUM_LEDS_PER_STRIP, CRGB::Black);
  else
    leds.strip[0][cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending); // I prefer to use palettes instead of CHSV or CRGB assignments.
}

// https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

void mode_plasma()
{
  //DB_PRINTLN(F("mode_plasma"));

  static CRGBPalette16 currentPalette = ForestColors_p; // Palette definitions
  static CRGBPalette16 targetPalette;
  static TBlendType currentBlending = LINEARBLEND;

  EVERY_N_MILLISECONDS(50)
  {                                       // FastLED based non-blocking delay to update/display the sequence.
    int thisPhase = beatsin8(6, -64, 64); // Setting phase change for a couple of waves.
    int thatPhase = beatsin8(7, -64, 64);

    for (int k = 0; k < NUM_LEDS_PER_STRIP; k++)
    {
      // For each of the LED's in the strand, set a brightness based on a wave as follows:
      int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2; // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
      int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

      leds.strip[0][k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending); // Let's now add the foreground colour.
    }
  }

  EVERY_N_MILLISECONDS(100)
  {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5)
  {                            // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8(); // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)), CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)));
  }
}

#endif // DEMO
