#include "Arduino.h"

#ifdef ENCODER
// https://github.com/PaulStoffregen/Encoder
#include <Encoder.h>
#endif

#ifdef BOUNCE
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>
#endif

#define WIFI
#ifdef WIFI
// https://github.com/khoih-prog/ESPAsync_WiFiManager
#include <ESPAsync_WiFiManager.h>
#endif

// https://github.com/FastLED/FastLED
#define FASTLED_RMT_MAX_CHANNELS 2
#include <FastLED.h>

#define DEBUG
#define DEMO

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//
#ifdef PHOTOCELL
#define PHOTOCELL_PIN 32
#endif

#ifdef ENCODER
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
#endif

// setup our LED strips for parallel output using FastLED
#define LED_STRIPS_PIN_BASE 16
#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 156
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// With parallel updates for the LEDs so fast, we get flickering if we call
// FastLED.Show every loop. Maintain a 'dirty' bit so we know when to call Show.
boolean leds_dirty = true;

#include "debug.h"

#include "Kaleidoscope.h"
#include "ripples.h"
Kaleidoscope kaleidoscope;

#ifdef WIFI
AsyncWebServer webServer(80);
DNSServer dnsServer;
#endif

#define TIME
#ifdef TIME
#include "RealTimeClock.h"
RealTimeClock myclock;
#endif

#ifdef BOUNCE
// Instantiate Button objects from the Bounce2 namespace
Bounce2::Button leftButton = Bounce2::Button();
Bounce2::Button rightButton = Bounce2::Button();
#define DEBOUNCE_MS 5 // Button debounce time, in milliseconds
#endif

#ifdef ENCODER
// Instantiate rotary encoder knob objects
Encoder knobRight(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
Encoder knobLeft(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);
#endif

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

#define KNOB_INCREMENT 10

// automatically adjust the brightness of the LED strips to match the ambient lighting
void adjustBrightness()
{
  // store the current LED brightness so we can minimize minor differences
  static int LEDbrightness = 0;
  static int LEDBrightnessManualOffset = 0;

  // The ADC input channels have a 12 bit resolution. This means that you can get analog readings
  // ranging from 0 to 4095, in which 0 corresponds to 0V and 4095 to 3.3V
  // https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

  // check the photocell and debounce it a bit as it moves around a lot
  // it is currently setup to use the internal pullup resistor so we need to invert it's value
  static int lastPhotocell = 0;
#ifdef PHOTOCELL
  int photocellReading = 4095 - analogRead(PHOTOCELL_PIN);
  if ((photocellReading > lastPhotocell + 75) || (photocellReading < lastPhotocell - 75))
  {
    lastPhotocell = photocellReading;
    DB_PRINTF("photocell reading = %d\r\n", photocellReading);
  }
#endif

#ifdef ENCODER
  // use the right knob as a brightness increment/decrement
  static int lastKnob = 0;
  int knob = knobRight.read();
  if (knob != lastKnob)
  {
    if (knob > lastKnob)
      LEDBrightnessManualOffset -= KNOB_INCREMENT;
    else
      LEDBrightnessManualOffset += KNOB_INCREMENT;

    LEDBrightnessManualOffset = constrain(LEDBrightnessManualOffset, -4095, 4095);
    lastKnob = knob;

    DB_PRINT(F("LEDBrightnessManualOffset = "));
    DB_PRINTLN(LEDBrightnessManualOffset);
  }
#endif

  // map our total brightness from 0-4095 to 0-255 since that is the range for setBrightness
  int newBrightness = map(lastPhotocell + LEDBrightnessManualOffset, 0, 4095, 0, 255);
  newBrightness = constrain(newBrightness, 0, 255);

  // adjust our brightness if it has changed
  if (newBrightness != LEDbrightness)
  {
    LEDbrightness = newBrightness;
    DB_PRINTF("new brightness = %d\r\n", newBrightness);

    FastLED.setBrightness(dim8_raw(LEDbrightness));
    leds_dirty = true;
  }
}

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
#ifdef TIME
    mode_select_clock_face,
    mode_set_clock,
#endif
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_rainbowMarch,
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_sawTooth,
    mode_kaleidoscope_ripples,
    mode_kaleidoscope_blendWave,
#endif
#ifdef DEBUG
    mode_kaleidoscope_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 0; // Index of current mode in table

const PROGMEM char modeNames[N_MODES][64] =
    {
        "mode_kaleidoscope_screensaver",
        "mode_kaleidoscope_interactive",
        "mode_color_wash",
        "mode_snowflake",
        "mode_off",
        "mode_kaleidoscope_select_disks",
#ifdef TIME
        "mode_select_clock_face",
        "mode_set_clock",
#endif
#ifdef DEMO
        "mode_off",
        "mode_kaleidoscope_rainbowMarch",
        "mode_kaleidoscope_plasma",
        "mode_kaleidoscope_sawTooth",
        "mode_kaleidoscope_ripples",
        "mode_kaleidoscope_blendWave",
#endif
#ifdef DEBUG
        "mode_kaleidoscope_test",
#endif
        "mode_off"};

//
// SETUP FUNCTION -- RUNS ONCE AT PROGRAM START ----------------------------
//
void setup()
{
#ifdef DEBUG
  // 3 second delay for recovery
  delay(3000);

  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
#endif

#ifdef WIFI
  // connect to wifi or enter AP mode so it can be configured
  DB_PRINT("\nStarting Async_AutoConnect_ESP32_minimal on " + String(ARDUINO_BOARD));
  DB_PRINTLN(ESP_ASYNC_WIFIMANAGER_VERSION);
  ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Kaleidoscope");
  //ESPAsync_wifiManager.resetSettings();   //reset saved settings
  ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 132, 1), IPAddress(192, 168, 132, 1), IPAddress(255, 255, 255, 0));
  ESPAsync_wifiManager.autoConnect("KaleidoscopeAP");
  if (WiFi.status() == WL_CONNECTED)
  {
    DB_PRINT(F("Connected. Local IP: "));
    DB_PRINTLN(WiFi.localIP());
  }
  else
  {
    DB_PRINTLN(ESPAsync_wifiManager.getStatus(WiFi.status()));
  }
#endif

#ifdef TIME
  // intialize the real time clock
  myclock.setup();
#endif

#ifdef PHOTOCELL
  // initialize the photo resister using the pullup resistor
  pinMode(PHOTOCELL_PIN, INPUT_PULLUP);
#endif

  // initialize the random number generator using noise from an analog pin
  randomSeed(analogRead(33));

#ifdef BOUNCE
  // initialize the rotary encoder buttons using the pullup resistor
  leftButton.attach(ENCODER_BUTTON_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_BUTTON_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
#endif
  DB_PRINTLN(modeNames[mode]);

  // intialize the LED strips for parallel output
  FastLED.addLeds<LED_TYPE, LED_STRIPS_PIN_BASE + 0, COLOR_ORDER>(leds + 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIPS_PIN_BASE + 1, COLOR_ORDER>(leds + 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIPS_PIN_BASE + 2, COLOR_ORDER>(leds + 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIPS_PIN_BASE + 3, COLOR_ORDER>(leds + 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  leds_dirty = true;

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

#ifdef ENCODER
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
#endif

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  (*renderFunc[mode])();

  // update the clock
  myclock.loop();

  // if we have changes in the LEDs, show the updated frame
  if (leds_dirty)
  {
    FastLED.show();
    leds_dirty = false;
  }
}
