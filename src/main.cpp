#include "main.h"
#include "Kaleidoscope.h"
#include "XY.h"
#ifdef DEMO
#include "plasma.h"
#include "ripples.h"
#include "TwinkleFox.h"
#include "XYDistortionWaves.h"
#include "XYAALines.h"
#include "XYmatrix.h"
#include "XYpacifica.h"
#include "XYsnake.h"
#include "XYfire.h"
#endif

#ifdef BOUNCE
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>
#endif

#ifdef WIFI

// https://github.com/khoih-prog/ESPAsync_WiFiManager
#ifdef DRD
//#define USE_SPIFFS true
#define ESP_DRD_USE_EEPROM true
#endif // DRD
#define USE_ESP_WIFIMANAGER_NTP true
#include <ESPAsync_WiFiManager.h>
#include <ESPAsync_WiFiManager-Impl.h>
#ifdef REST
#include <AsyncJson.h>
#include <ArduinoJson.h>
#endif // REST

#ifdef DRD
// https://github.com/khoih-prog/ESP_DoubleResetDetector
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
#include <ESP_DoubleResetDetector.h>
#endif

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
#endif // ALEXA

#ifdef TIME
#include "RealTimeClock.h"
#include <Preferences.h>
#endif // TIME

#ifdef WEATHER
#include "weather.h"
#endif // WEATHER

#endif // WIFI

//
// GLOBAL PIN DECLARATIONS -------------------------------------------------
//
#ifdef PHOTOCELL
#define PHOTOCELL_PIN 33
#endif

#ifdef ENCODER
// Change these pin numbers to the pins connected to your encoder.
#define ENCODER_CLK_PIN_LEFT 4
#define ENCODER_DT_PIN_LEFT 16
#define ENCODER_CLK_PIN_RIGHT 5
#define ENCODER_DT_PIN_RIGHT 18
#endif

// setup our LED strips for parallel output using FastLED
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

// this will compile for Arduino Mega

#ifdef BOUNCE
// Change these pin numbers to the button pins on your encoder.
#define ENCODER_SW_PIN_LEFT 2
#define ENCODER_SW_PIN_RIGHT 3
#endif

#define LED_STRIP_PIN_1 11
#define LED_STRIP_PIN_2 10
#define LED_STRIP_PIN_3 12
#define LED_STRIP_PIN_4 13
#else

// this will compile for ESP32

#ifdef BOUNCE
// Change these pin numbers to the button pins on your encoder.
#define ENCODER_SW_PIN_LEFT 17
#define ENCODER_SW_PIN_RIGHT 19
#endif

#define LED_STRIP_PIN_1 14
#define LED_STRIP_PIN_2 27
#define LED_STRIP_PIN_3 26
#define LED_STRIP_PIN_4 25
#endif
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
ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Kaleidoscope");
bool initialConfig = false;

#ifdef DRD
DoubleResetDetector *drd;
#endif

#ifdef ALEXA
Espalexa espalexa;
#endif
#endif // WIFI

// All Pixels off
void mode_off()
{
  // nothing to see here... (the pixels got cleared by the button press)
}

#ifdef DEBUG
// test the wiring and ensure all pixels light up correctly
// Q: Why does led[300] ==> led[312] not light up?
// A: Our strips aren't the same length (156 vs 144) so the shorter strips (1 and 2)
// have extra leds[x] positions that don't have physical LEDs.
void mode_test()
{
  static int index = 0;

  EVERY_N_MILLISECONDS(50)
  {
    // erase the last pixel
    leds[index] = CRGB::Black; // off

    // move to the next pixel
    if (++index >= NUM_STRIPS * NUM_LEDS_PER_STRIP)
      index = 0;
    DB_PRINTLN(index);

    // light up the next pixel
    leds[index] = CRGB::Red;

    leds_dirty = true;
  }

  adjustBrightness();
}
#endif

//
// BRIGHTNESS helpers -------------------------------------------------
//

#define MIN_BRIGHTNESS 32                    // the minimum brightness we want (above zero so it doesn't go completely dark)
#define MAX_BRIGHTNESS 255                   // the max possible brightness
#define MAX_BRIGHTNESS_READING 1024          // set this to the higest reading you get from the photocell
#define KNOB_INCREMENT (MAX_BRIGHTNESS / 20) // brightness range / number of pulses in one rotation of rotary encoder
#define DEBOUNCE_PHOTOCELL 64                // how much change we need to see in the average photocell reading before we change the brightness

#ifdef PHOTOCELL
#define FILTER_LEN 50
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

// compute the brightness of the LED strips to match the ambient lighting
int ambientBrightness()
{
  // check the photocell and debounce it a bit as it moves around a lot
  static int lastPhotocell = 0;

#ifdef PHOTOCELL
  // The ADC input channels have a 12 bit resolution. This means that you can get analog readings
  // ranging from 0 to 4095, in which 0 corresponds to 0V and 4095 to 3.3V. https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
  // However, I've only measured it as high as 1903 with a very bright light directly on the photocell.
  int photocellReading = readADC_Avg(analogRead(PHOTOCELL_PIN));
  if ((photocellReading > lastPhotocell + DEBOUNCE_PHOTOCELL) || (photocellReading < lastPhotocell - DEBOUNCE_PHOTOCELL))
  {
    lastPhotocell = photocellReading;
    DB_PRINTF("photocell reading = %d\r\n", photocellReading);
  }
#endif

  // ensure we stay between our min and max valid values
  return constrain(map(lastPhotocell, 0, MAX_BRIGHTNESS_READING, 0, MAX_BRIGHTNESS), 0, MAX_BRIGHTNESS);
}

// manually adjust the brightness of the LED strips up or down from the ambientBrightness
static int LEDBrightnessManualOffset = 0;
int manualBrightness(bool useKnob)
{
  if (!useKnob)
    return LEDBrightnessManualOffset;

#ifdef ENCODER
  // use the right knob as a brightness increment/decrement
  static int lastRightKnob = 0;
  int knob = knobRight.getCount();
  if (knob != lastRightKnob)
  {
    if (knob > lastRightKnob)
      LEDBrightnessManualOffset -= KNOB_INCREMENT;
    else
      LEDBrightnessManualOffset += KNOB_INCREMENT;

    LEDBrightnessManualOffset = constrain(LEDBrightnessManualOffset, -MAX_BRIGHTNESS, MAX_BRIGHTNESS);
    lastRightKnob = knob;

    DB_PRINTF("LEDBrightnessManualOffset = %d\r\n", LEDBrightnessManualOffset);
  }
#ifdef DEBUG
  // test code for the left knob
  static int lastLeftKnob = 0;
  knob = knobLeft.getCount();
  if (knob != lastLeftKnob)
  {
    lastLeftKnob = knob;
    DB_PRINTF("Left knob count = %d\r\n", lastLeftKnob);
  }
#endif
#endif

  return LEDBrightnessManualOffset;
}

// update the FastLED brightness based on our ambient and manual settings
void adjustBrightness(bool useKnob)
{
  // store the current LED brightness so we can minimize minor differences
  static int LEDbrightness = 0;

  // constrain our total brightness from MIN_BRIGHTNESS to MAX_BRIGHTNESS so it doesn't get too dark
  int newBrightness = constrain(ambientBrightness() + manualBrightness(useKnob), MIN_BRIGHTNESS, MAX_BRIGHTNESS);

  // adjust our brightness if it has changed
  if (newBrightness != LEDbrightness)
  {
    LEDbrightness = newBrightness;
    DB_PRINTF("new brightness = %d\r\n", newBrightness);

    FastLED.setBrightness(dim8_raw(LEDbrightness));
    leds_dirty = true;
  }
}

//
// use the left rotary knob to adjust the speed of the kaleidoscope
//

// The increment is defined to require two complete knob rotations to go from min to max
// Each knob roration is 20 clicks.
#define INCREMENT_SPEED (KALEIDOSCOPE_MAX_SPEED / 40)
uint8_t kaleidoscope_speed = KALEIDOSCOPE_DEFAULT_SPEED;
int adjustSpeed()
{
#ifdef ENCODER
  static int lastLeftKnob = 0;
  int new_speed = kaleidoscope_speed;

  int knob = knobLeft.getCount();
  if (knob != lastLeftKnob)
  {
    if (knob > lastLeftKnob)
      new_speed -= INCREMENT_SPEED;
    else
      new_speed += INCREMENT_SPEED;

    kaleidoscope_speed = constrain(new_speed, 0, KALEIDOSCOPE_MAX_SPEED);
    lastLeftKnob = knob;

    DB_PRINTF("ms between frames = %d\r\n", kaleidoscope_speed);
  }
#endif
  return kaleidoscope_speed;
}

// TODO: refactor the kaleidoscope mode logic into kaleidoscope.h/.cpp
// This array lists each of the display/animation drawing functions
// (which appear later in this code) in the order they're selected with
// the right button.  Some functions appear repeatedly...for example,
// we return to "mode_off" at several points in the sequence.
void (*renderFunc[])(void){
    mode_kaleidoscope,
#ifdef ENCODER
    mode_off, // make it obvious we're entering 'setup' modes
    mode_kaleidoscope_select_speed_brightness,
    //    mode_kaleidoscope_select_disks,
    mode_kaleidoscope_select_reflection_style,
#ifdef TIME
    mode_select_clock_face,
#endif
#endif
#ifdef DEMO
    mode_off, // make it obvious we're entering 'demo' modes
    mode_kaleidoscope_plasma,
    mode_kaleidoscope_ripples,
    mode_kaleidoscope_twinkle_fox,
    mode_xy_aalines,
    mode_xy_distortion_waves,
    mode_xy_matrix,
    mode_xy_pacifica,
    mode_xy_snake,
    mode_xy_fire,
#endif
#ifdef DEBUG
    mode_kaleidoscope_test,
    mode_xy_test,
    mode_test,
#endif
    mode_off // make it obvious we're entering 'regular' modes
};
#define N_MODES (sizeof(renderFunc) / sizeof(renderFunc[0]))
uint8_t kaleidoscope_mode = 0; // Index of current mode in table

const char modeNames[N_MODES][64] =
    {
        "Kaleidoscope",
#ifdef ENCODER
        "off",
        "kaleidoscope_select_speed_brightness",
        //        "kaleidoscope_select_disks",
        "kaleidoscope_select_reflection_style",
#ifdef TIME
        "select_clock_face",
#endif
#endif
#ifdef DEMO
        "off",
        "Plasma",
        "Ripples",
        "Twinkle Fox",
        "AA Lines",
        "Distortion Waves",
        "Matrix",
        "Pacifica",
        "Snake",
        "Fire",
#endif
#ifdef DEBUG
        "kaleidoscope_test",
        "xy_test",
        "test",
#endif
        "off"};

const PROGMEM char showInRESTAPI[N_MODES]{
    1,
#ifdef ENCODER
    0,
    0,
    //        "kaleidoscope_select_disks",
    0,
#ifdef TIME
    0,
#endif
#endif
#ifdef DEMO
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
#endif
#ifdef DEBUG
    1,
    1,
    1,
#endif
    0};

#ifdef ENCODER
// We need to save/restore the count for the rotary encoders
// when we switch between modes so that each mode doesn't impact
// the other modes.
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1
int modeEncoderCounts[N_MODES][2] =
    {
        {0, 0},
        {0, 0},
        {0, 0},
        //        {0, 0},
        {0, 0},
#ifdef TIME
        {0, 0},
#endif
#ifdef DEMO
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
#endif
#ifdef DEBUG
        {0, 0},
        {0, 0},
        {0, 0},
#endif
        {0, 0}};
#endif

void setKaleidoscopeMode(int new_mode)
{
  // if the mode changed
  if (kaleidoscope_mode != new_mode)
  {
#ifdef ENCODER
    int old_mode = kaleidoscope_mode;

    // save the encoder count for the old mode and restore the new mode count
    modeEncoderCounts[old_mode][LEFT_ENCODER] = knobLeft.getCount();
    knobLeft.setCount(modeEncoderCounts[new_mode][LEFT_ENCODER]);
    modeEncoderCounts[old_mode][RIGHT_ENCODER] = knobRight.getCount();
    knobRight.setCount(modeEncoderCounts[new_mode][RIGHT_ENCODER]);
#endif

    // output the new mode name and clear the led strips for the new mode
    kaleidoscope_mode = new_mode;
    DB_PRINTLN(modeNames[kaleidoscope_mode]);
    FastLED.clear();
    leds_dirty = true;
  }
}

#ifdef WIFI
#ifdef REST
// This is needed to solve some threading issues with the REST APIs coming in via
// asyncronous messages. When we get a change, just store them in the temporary
// settings object then apply them on the primary thread so we aren't racing
// with the current display loop.
typedef struct
{
  int LEDBrightnessManualOffset;
  int kaleidoscope_speed;
  int kaleidoscope_mode;
  int clock_face;
  int draw_style;
  CRGB clockColor;
} kaleidoscope_settings;
kaleidoscope_settings settings = {
    LEDBrightnessManualOffset,
    kaleidoscope_speed,
    kaleidoscope_mode,
    clock_face,
    draw_style,
    clockColor};
volatile bool newSettings = false;

void applySettings(kaleidoscope_settings &settings)
{
  if (!newSettings)
    return;

  // all validation of values happens in saveSettings()
  if (LEDBrightnessManualOffset != settings.LEDBrightnessManualOffset)
  {
    LEDBrightnessManualOffset = settings.LEDBrightnessManualOffset;
  }

  if (kaleidoscope_speed != settings.kaleidoscope_speed)
  {
    kaleidoscope_speed = settings.kaleidoscope_speed;
  }

  if (kaleidoscope_mode != settings.kaleidoscope_mode)
  {
    setKaleidoscopeMode(settings.kaleidoscope_mode);
  }

  if (clock_face != settings.clock_face)
  {
    set_clock_face(settings.clock_face);
  }

  if (draw_style != settings.draw_style)
  {
    set_draw_style(settings.draw_style);
  }

  if (clockColor != settings.clockColor)
  {
    clockColor = settings.clockColor;
  }

  newSettings = false;
}

void getSettings(AsyncWebServerRequest *request)
{
  StaticJsonDocument<512> doc;
  String response;

  doc["mode"] = modeNames[kaleidoscope_mode];
  doc["drawStyle"] = drawStyles[draw_style];
  doc["brightness"] = LEDBrightnessManualOffset;
  doc["speed"] = kaleidoscope_speed;
  doc["clockFace"] = clockFaces[clock_face];
  doc["clockColor"] = clockColor.r << 16 | clockColor.g << 8 | clockColor.b;

  serializeJson(doc, response);
  request->send(200, "text/json", response);
}

void getModes(AsyncWebServerRequest *request)
{
  // compute the required size
  const size_t CAPACITY = JSON_ARRAY_SIZE(N_MODES);

  // allocate the memory for the document
  StaticJsonDocument<CAPACITY> doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the names
  for (int x = 0; x < N_MODES; x++)
  {
    if (pgm_read_byte_near(&showInRESTAPI[x]))
      array.add(modeNames[x]);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  request->send(200, "text/json", response);
}

void getFaces(AsyncWebServerRequest *request)
{
  // compute the required size
  const size_t CAPACITY = JSON_ARRAY_SIZE(N_CLOCK_FACES);

  // allocate the memory for the document
  StaticJsonDocument<CAPACITY> doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the names
  for (int x = 0; x < N_CLOCK_FACES; x++)
  {
    array.add(clockFaces[x]);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  request->send(200, "text/json", response);
}

void getDrawStyles(AsyncWebServerRequest *request)
{
  // compute the required size
  const size_t CAPACITY = JSON_ARRAY_SIZE(N_DRAW_STYLES);

  // allocate the memory for the document
  StaticJsonDocument<CAPACITY> doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the draw style names
  for (int x = 0; x < N_DRAW_STYLES; x++)
  {
    array.add(drawStyles[x]);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  request->send(200, "text/json", response);
}

void saveSettings(AsyncWebServerRequest *request, JsonVariant &json)
{
  const JsonObject &jsonObj = json.as<JsonObject>();

  // update the brightness (if it was passed)
  JsonVariant brightness = jsonObj["brightness"];
  if (!brightness.isNull())
  {
    settings.LEDBrightnessManualOffset = constrain((int)brightness, -MAX_BRIGHTNESS, MAX_BRIGHTNESS);
    newSettings = true;
    DB_PRINTF("brightness = %d\r\n", settings.LEDBrightnessManualOffset);
  }

  JsonVariant speed = jsonObj["speed"];
  if (!speed.isNull())
  {
    settings.kaleidoscope_speed = constrain((int)speed, 0, KALEIDOSCOPE_MAX_SPEED);
    newSettings = true;
    DB_PRINTF("speed = %d\r\n", settings.kaleidoscope_speed);
  }

  const char *modeName = jsonObj["mode"];
  if (modeName)
  {
    for (int x = 0; x < N_MODES; x++)
    {
      if (String(modeNames[x]).equalsIgnoreCase(String(modeName)))
      {
        settings.kaleidoscope_mode = x;
        newSettings = true;
        DB_PRINTF("mode = %s\r\n", modeNames[x]);
        break;
      }
    }
  }

  const char *clockFace = jsonObj["clockFace"];
  if (clockFace)
  {
    for (int x = 0; x < N_CLOCK_FACES; x++)
    {
      if (String(clockFaces[x]).equalsIgnoreCase(String(clockFace)))
      {
        settings.clock_face = x;
        newSettings = true;
        DB_PRINTF("clockFace = %s\r\n", clockFaces[x]);
        break;
      }
    }
  }

  const char *drawStyle = jsonObj["drawStyle"];
  if (drawStyle)
  {
    for (int x = 0; x < N_DRAW_STYLES; x++)
    {
      if (String(drawStyles[x]).equalsIgnoreCase(String(drawStyle)))
      {
        settings.draw_style = x;
        newSettings = true;
        DB_PRINTF("drawStyle = %s\r\n", drawStyles[x]);
        break;
      }
    }
  }

  JsonVariant clockColor = jsonObj["clockColor"];
  if (!clockColor.isNull())
  {
    settings.clockColor = CRGB((uint32_t)clockColor);
    newSettings = true;
    DB_PRINTF("clockColor = %d\r\n", (uint32_t)settings.clockColor);
  }

  request->send(200, "text/plain", "OK");
}
#endif

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

void check_WiFi(void)
{
  if ((WiFi.status() != WL_CONNECTED))
  {
    DB_PRINTLN(F("\nWiFi lost. Attempting to reconnect"));

    // attempt to reconnect
    ESPAsync_wifiManager.autoConnect("KaleidoscopeAP");

    // report on our WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      DB_PRINT(F("Connected. Local IP: "));
      DB_PRINTLN(WiFi.localIP());
    }
    else
    {
      DB_PRINTLN(ESPAsync_wifiManager.getStatus(WiFi.status()));
    }
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
  DB_PRINT("\nStarting Kaleidoscope on " + String(ARDUINO_BOARD));
#endif

#ifdef WIFI
  // connect to wifi or enter AP mode so it can be configured
  DB_PRINTLN(ESP_ASYNC_WIFIMANAGER_VERSION);

#ifdef DRD
  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  if (drd->detectDoubleReset())
  {
    DB_PRINTLN("Double reset detected");
    initialConfig = true;
  }
#endif

  ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Kaleidoscope");
  //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 132, 1), IPAddress(192, 168, 132, 1), IPAddress(255, 255, 255, 0));
  //ESPAsync_wifiManager.resetSettings();   //reset saved settings
  if (ESPAsync_wifiManager.WiFi_SSID() == "")
  {
    DB_PRINTLN(F("No AP credentials"));
    initialConfig = true;
  }
  if (initialConfig)
  {
    DB_PRINTLN(F("Starting Config Portal"));
    ESPAsync_wifiManager.startConfigPortal("KaleidoscopeAP");
  }
  else
  {
    ESPAsync_wifiManager.autoConnect("KaleidoscopeAP");
  }

  // report on our WiFi connection status
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
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", "Hi! I an a digital Kaleidoscope. You can do an over the air update by visiting http://kaleidoscope/update"); });

  // Start ElegantOTA and require a username/password
  AsyncElegantOTA.begin(&webServer, "admin", "admin");
  DB_PRINTLN(F("OTA web server started."));
#endif

#ifdef REST
  webServer.on("/api/settings", HTTP_GET, getSettings);
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/settings", saveSettings);
  webServer.addHandler(handler);
  webServer.on("/api/modes", HTTP_GET, getModes);
  webServer.on("/api/faces", HTTP_GET, getFaces);
  webServer.on("/api/drawstyles", HTTP_GET, getDrawStyles);
#endif

#ifdef ALEXA
  webServer.onNotFound([](AsyncWebServerRequest *request)
                       {
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
  pinMode(PHOTOCELL_PIN, INPUT /*_PULLUP*/);

// D:\src\kaleidoscope\.pio\libdeps\node32s\ESP32Encoder\src\ESP32Encoder.cpp
//gpio_pullup_en((gpio_num_t)PHOTOCELL_PIN);
#endif

  // initialize the random number generator using noise from an analog pin
  randomSeed(analogRead(33));

#ifdef BOUNCE
  // initialize the rotary encoder switches. The KY-040 rotary encoders already contain
  // 10k-Ohm pull up resisters so we don't need to turn on the internal ones
  leftButton.attach(ENCODER_SW_PIN_LEFT, INPUT);
  leftButton.interval(DEBOUNCE_MS);
  leftButton.setPressedState(LOW);
  rightButton.attach(ENCODER_SW_PIN_RIGHT, INPUT);
  rightButton.interval(DEBOUNCE_MS);
  rightButton.setPressedState(LOW);
#endif

#ifdef ENCODER
  // Initialize the rotary encoders. The KY-040 rotary encoders already contain
  // 10k-Ohm pull up resisters so we don't need to turn on the internal ones
  ESP32Encoder::useInternalWeakPullResistors = NONE;
  knobRight.attachSingleEdge(ENCODER_CLK_PIN_RIGHT, ENCODER_DT_PIN_RIGHT);
  knobRight.setFilter(1023);
  knobLeft.attachSingleEdge(ENCODER_CLK_PIN_LEFT, ENCODER_DT_PIN_LEFT);
  knobLeft.setFilter(1023);
#endif

  // intialize the LED strips for parallel output
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_1, COLOR_ORDER>(leds + 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_2, COLOR_ORDER>(leds + 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_3, COLOR_ORDER>(leds + 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN_4, COLOR_ORDER>(leds + 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  leds_dirty = true;
  DB_PRINTLN(modeNames[kaleidoscope_mode]);
}

//
// LOOP FUNCTION -- RUNS OVER AND OVER FOREVER -----------------------------
//
void loop()
{
#ifdef REST
  // apply any settings changes received via the REST APIs
  applySettings(settings);
#endif

#ifdef BOUNCE
  // check for a mode change
  uint8_t new_mode = kaleidoscope_mode;

  // Left button pressed?
  leftButton.update();
  if (leftButton.pressed())
  {
    if (new_mode)
      new_mode--; // Go to prior mode
    else
      new_mode = N_MODES - 1; // or "wrap around" to last mode
  }

  // Right button pressed?
  rightButton.update();
  if (rightButton.pressed())
  {
    if (new_mode < (N_MODES - 1))
      new_mode++; // Advance to next mode
    else
      new_mode = 0; // or "wrap around" to start
  }

  setKaleidoscopeMode(new_mode);
#endif

#ifdef WIFI
  // check and reconnect to WiFi if needed
  check_WiFi();

#ifdef DRD
  drd->loop();
#endif

#ifdef ALEXA
  espalexa.loop();
#endif // ALEXA
#endif // WIFI

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  (*renderFunc[kaleidoscope_mode])();

  // draw the clock face (can be a null clock face - see mode_select_clock_face())
#ifdef WIFI
#ifdef TIME
  // don't draw the clock if we're in 'off' mode
  if (renderFunc[kaleidoscope_mode] != mode_off)
    draw_clock();
#endif // TIME

#ifdef WEATHER
  if (WiFi.status() == WL_CONNECTED)
    weather_loop(current_weather);
#endif // WEATHER
#endif // WIFI

// Show an activity spinner and the current fps. After 500 ms of no LED updates show 0 fps.
// This is to prevent the fps flickering between 0 fps and x fps when there are no updates
// to display (i.e. not calling FastLED.Show every loop).
#ifdef DEBUG
  static CEveryNMilliseconds triggerTimer(500);
  static const char *spinner = "|/-\\";
  static int spinner_index = 0;

  if (leds_dirty)
  {
    DB_PRINTF("\r%c %d fps\r", spinner[spinner_index], FastLED.getFPS());
    spinner_index = (spinner_index + 1) % sizeof(spinner);
    triggerTimer.reset();
  }
  else
  {
    if (triggerTimer)
    {
      DB_PRINT("\r0 fps   \r");
    }
  }
#endif

  // if we have changes in the LEDs, show the updated frame
  if (leds_dirty)
  {
    FastLED.show();
    leds_dirty = false;
  }
}
