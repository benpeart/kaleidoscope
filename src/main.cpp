#include "main.h"
#include "Kaleidoscope.h"
#ifdef DEMO
#include "ripples.h"
#include "beatwave.h"
#include "rainbowmarch.h"
#include "plasma.h"
#include "blendwave.h"
#include "sawtooth.h"
#include "XYIndex.h"
#include "XYmatrix.h"
#include "XYrainbow.h"
#endif

#ifdef BOUNCE
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>
#endif

#ifdef WIFI
// https://github.com/khoih-prog/ESPAsync_WiFiManager
#define USE_ESP_WIFIMANAGER_NTP true
#include <ESPAsync_WiFiManager.h>

#ifdef OTA
// https://github.com/ayushsharma82/AsyncElegantOTA
#include <AsyncElegantOTA.h>
#endif

#ifdef ALEXA
// https://github.com/Aircoookie/Espalexa
#define ESPALEXA_ASYNC
#define ESPALEXA_MAXDEVICES 1
#define ESPALEXA_DEBUG
#include <Espalexa.h>
#endif

#ifdef TIME
#include "RealTimeClock.h"
#include <Preferences.h>
#endif
#endif // WIFI

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//
#ifdef PHOTOCELL
#define PHOTOCELL_PIN 33
#endif

#ifdef BOUNCE
// Change these pin numbers to the button pins on your encoder.
#define ENCODER_SWITCH_PIN_LEFT 5
#define ENCODER_SWITCH_PIN_RIGHT 23
#endif

#ifdef ENCODER
// Change these pin numbers to the pins connected to your encoder.
#define ENCODER_CLK_PIN_LEFT 16
#define ENCODER_DIRECTION_PIN_LEFT 17
#define ENCODER_CLK_PIN_RIGHT 21
#define ENCODER_DIRECTION_PIN_RIGHT 22
#endif

// setup our LED strips for parallel output using FastLED
#define LED_STRIP_PIN_1 14
#define LED_STRIP_PIN_2 27
#define LED_STRIP_PIN_3 26
#define LED_STRIP_PIN_4 25
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

//
// Global variables  -------------------------------------------------
//

#ifdef BOUNCE
// Instantiate Button objects from the Bounce2 namespace
Bounce2::Button leftButton = Bounce2::Button();
Bounce2::Button rightButton = Bounce2::Button();
#define DEBOUNCE_MS 15 // Button debounce time, in milliseconds
#endif

#ifdef ENCODER
// Instantiate rotary encoder knob objects
ESP32Encoder knobRight;
ESP32Encoder knobLeft;
#endif

#ifdef WIFI
AsyncWebServer webServer(80);
DNSServer dnsServer;

#ifdef ALEXA
Espalexa espalexa;
#endif
#endif // WIFI

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
}

//
// BRIGHTNESS helpers -------------------------------------------------
//

#define KNOB_INCREMENT 100

#ifdef PHOTOCELL
#define FILTER_LEN 15
uint32_t readADC_Avg(int ADC_Raw)
{
  static uint32_t ADCBuffer[FILTER_LEN];
  static int index = 0;
  uint32_t Sum = 0;

  ADCBuffer[index++] = ADC_Raw;
  if (index == FILTER_LEN)
  {
    index = 0;
  }
  for (int i = 0; i < FILTER_LEN; i++)
  {
    Sum += ADCBuffer[i];
  }
  return (Sum / FILTER_LEN);
}
#endif

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
  static int lastPhotocell = 0;
#ifdef PHOTOCELL
  int photocellReading = readADC_Avg(analogRead(PHOTOCELL_PIN));
  if ((photocellReading > lastPhotocell + 256) || (photocellReading < lastPhotocell - 256))
  {
    lastPhotocell = photocellReading;
    DB_PRINTF("photocell reading = %d\r\n", photocellReading);
  }
#endif

  // use the right knob as a brightness increment/decrement
  static int lastRightKnob = 0;
  int knob = knobRight.getCount();
  if (knob != lastRightKnob)
  {
    if (knob > lastRightKnob)
      LEDBrightnessManualOffset -= KNOB_INCREMENT;
    else
      LEDBrightnessManualOffset += KNOB_INCREMENT;

    LEDBrightnessManualOffset = constrain(LEDBrightnessManualOffset, -4095, 4095);
    lastRightKnob = knob;

    DB_PRINTF("LEDBrightnessManualOffset = %d\r\n", LEDBrightnessManualOffset);
  }

  // test code for the left knob
  static int lastLeftKnob = 0;
  knob = knobLeft.getCount();
  if (knob != lastLeftKnob)
  {
    lastLeftKnob = knob;
    DB_PRINTF("Left knob count = %d\r\n", lastLeftKnob);
  }

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
    mode_off, // make it obvious we're entering 'setup' modes
    mode_kaleidoscope_select_disks,
#ifdef TIME
    mode_select_clock_face,
#endif
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_rainbowMarch,
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_sawTooth,
    mode_kaleidoscope_ripples,
    mode_kaleidoscope_blendWave,
    mode_kaleidoscope_beatWave,
    mode_xy_matrix,
    mode_xy_rainbow,
#endif
#ifdef DEBUG
    mode_kaleidoscope_test,
    mode_xy_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t mode = 0; // Index of current mode in table

const PROGMEM char modeNames[N_MODES][64] =
    {
        "mode_kaleidoscope_screensaver",
        "mode_kaleidoscope_interactive",
        "mode_off",
        "mode_kaleidoscope_select_disks",
#ifdef TIME
        "mode_select_clock_face",
#endif
#ifdef DEMO
        "mode_off",
        "mode_kaleidoscope_rainbowMarch",
        "mode_kaleidoscope_plasma",
        "mode_kaleidoscope_sawTooth",
        "mode_kaleidoscope_ripples",
        "mode_kaleidoscope_blendWave",
        "mode_kaleidoscope_beatWave",
        "mode_xy_matrix",
        "mode_xy_rainbow",
#endif
#ifdef DEBUG
        "mode_kaleidoscope_test",
        "mode_xy_test",
#endif
        "mode_off"};

#ifdef ALEXA
//our Alexa callback function
void hueChanged(EspalexaDevice *d)
{
  Serial.print("E changed to ");
  Serial.print(d->getValue());
  Serial.print(", colormode ");
  switch (d->getColorMode())
  {
  case EspalexaColorMode::hs:
    Serial.print("hs, ");
    Serial.print("hue ");
    Serial.print(d->getHue());
    Serial.print(", sat ");
    Serial.println(d->getSat());
    break;
  case EspalexaColorMode::xy:
    Serial.print("xy, ");
    Serial.print("x ");
    Serial.print(d->getX());
    Serial.print(", y ");
    Serial.println(d->getY());
    break;
  case EspalexaColorMode::ct:
    Serial.print("ct, ");
    Serial.print("ct ");
    Serial.println(d->getCt());
    break;
  case EspalexaColorMode::none:
    Serial.println("none");
    break;
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
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
#endif

#ifdef WIFI
  // connect to wifi or enter AP mode so it can be configured
  DB_PRINT("\nStarting Kaleidoscope on " + String(ARDUINO_BOARD));
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

#ifdef OTA
  // add a simple home page (OTA update UI is on /update)
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I an a digital Kaleidoscope.");
  });

  // Start ElegantOTA and require a username/password
  AsyncElegantOTA.begin(&webServer, "admin", "admin");
  DB_PRINTLN(F("OTA web server started."));
#endif

#ifdef ALEXA
  webServer.onNotFound([](AsyncWebServerRequest *request) {
    // if you don't know the URI, ask espalexa whether it is an Alexa control request
    if (!espalexa.handleAlexaApiCall(request))
    {
      // handle the 404 error
      request->send(404, "text/plain", "Not found");
    }
  });

  // Define your devices here.
  espalexa.addDevice("Hue", hueChanged, EspalexaDeviceType::extendedcolor); //color + color temperature

  // give espalexa a pointer to your server object so it can use your server instead of creating its own
  espalexa.begin(&webServer);
#endif

#ifdef TIME
  // this only returns a value during the initial config step (call resetSettings() to test)
  // store the string in persistant storage for later use
  String timezoneName = ESPAsync_wifiManager.getTimezoneName();
  if (timezoneName.length())
  {
    // write the timezone string into persistant memory
    DB_PRINTF("Saving timezone '%s'\r\n", timezoneName.c_str());
    const char *tz = ESPAsync_wifiManager.getTZ(timezoneName);
    Preferences preferences;
    preferences.begin("kaleidoscope", false);
    preferences.putString("tz", tz);
    preferences.end();
  }

  // intialize the real time clock
  rtc_setup();
#endif

#ifndef ALEXA
  webServer.begin(); //omit this since it will be done by espalexa.begin(&webServer)
#endif
#endif // WIFI

#ifdef PHOTOCELL
  // Testing shows that the internal pullup resistors on the ESP32 are complete crap and
  // unusable. Probably why every example does their own external pullup/down resistors.
  pinMode(PHOTOCELL_PIN, INPUT/*_PULLUP*/);

  // D:\src\kaleidoscope\.pio\libdeps\node32s\ESP32Encoder\src\ESP32Encoder.cpp
  //gpio_pullup_en((gpio_num_t)PHOTOCELL_PIN);
#endif

  // initialize the random number generator using noise from an analog pin
  randomSeed(analogRead(33));

#ifdef BOUNCE
  // initialize the rotary encoder switches using the pullup resistor
  leftButton.attach(ENCODER_SWITCH_PIN_LEFT, INPUT_PULLUP);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_SWITCH_PIN_RIGHT, INPUT_PULLUP);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
#endif

#ifdef ENCODER
  // initialize the rotary encoders with filters re https://github.com/madhephaestus/ESP32Encoder#a-note-on-ky-040-and-similar
  ESP32Encoder::useInternalWeakPullResistors = UP;
  knobRight.attachSingleEdge(ENCODER_CLK_PIN_RIGHT, ENCODER_DIRECTION_PIN_RIGHT);
  knobRight.setFilter(1024);
  knobLeft.attachSingleEdge(ENCODER_CLK_PIN_LEFT, ENCODER_DIRECTION_PIN_LEFT);
  knobLeft.setFilter(1024);
#endif

  // intialize the LED strips for parallel output
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_1, COLOR_ORDER>(leds + 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_2, COLOR_ORDER>(leds + 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_3, COLOR_ORDER>(leds + 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_4, COLOR_ORDER>(leds + 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  leds_dirty = true;
  DB_PRINTLN(modeNames[mode]);
}

//
// LOOP FUNCTION -- RUNS OVER AND OVER FOREVER -----------------------------
//
void loop()
{

#ifdef BOUNCE
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

#ifdef WIFI
#ifdef OTA
  AsyncElegantOTA.loop();
#endif

#ifdef ALEXA
  espalexa.loop();
#endif

#ifdef TIME
  // update the clock
  rtc_loop();
#endif
#endif // WIFI

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  (*renderFunc[mode])();

  // if we have changes in the LEDs, show the updated frame
  if (leds_dirty)
  {
    FastLED.show();
    leds_dirty = false;
  }
}
