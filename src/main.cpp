#include "main.h"
#include "debug.h"
#include "settings.h"
#include "modes.h"
#include <math.h>

#ifdef BOUNCE
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>
#endif // BOUNCE

#ifdef WIFI
#include "WiFiHelpers.h"

#ifdef REST
#include <AsyncJson.h>
#endif // REST

#ifdef TIME
#include "RealTimeClock.h"
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

#ifdef BOUNCE
// Change these pin numbers to the button pins on your encoder.
#define ENCODER_SW_PIN_LEFT 17
#define ENCODER_SW_PIN_RIGHT 19
#endif // BOUNCE

// setup our LED strips for parallel output using FastLED
#ifdef JTAG
// JTAG uses GPIO12-GPIO15 so we can't use GPIO14 for an LED strip. Redefine it to use an unused GPIO
// so that we can debug even though we can't see the output of LED_STRIP_PIN_1 when mounted on the PCB.
// TODO: when we rev the PCB, use a different GPIO for LED_STRIP_PIN_1
#define LED_STRIP_PIN_1 32
#else
#define LED_STRIP_PIN_1 14
#endif
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

//
// BRIGHTNESS helpers -------------------------------------------------
//

// Solar calculation constants (Set to your local coordinates)
#define LATITUDE 38.048136f   // Positive = North, Negative = South
#define LONGITUDE -79.481413f // Positive = East, Negative = West

// true if the brightness has been changed via the REST API and needs to be reflected immediately
volatile bool brightness_dirty = false;

// Tracks the next Unix timestamp (epoch seconds) when brightness needs recalculation.
// Checking once every 15 to 30 seconds only while in twilight, and setting the next wake 
// time to the exact start of the next twilight when outside it, drops checks from ~28,000 down to
// roughly ~250 per day.
static time_t nextSolarCheckEpoch = 0;

// Transition window duration in minutes (e.g., 60 mins total: 30 mins before, 30 mins after dawn/dusk)
#define TWILIGHT_TRANSITION_MINUTES 60.0f

#define KNOB_INCREMENT (MAX_BRIGHTNESS / 20) // brightness range / number of pulses in one rotation of rotary encoder

// Returns a smooth multiplier from 0.5 (full night) to 1.0 (full day) and schedules next check
float getSolarBrightnessScale()
{
#if defined(TIME)
  time_t now = time(nullptr);
  if (now < 100000)
  {
    // Fallback if clock is not synchronized yet; recheck in 10s
    nextSolarCheckEpoch = now + 10;
    return 1.0f;
  }

  struct tm local_tm;
  struct tm utc_tm;
  localtime_r(&now, &local_tm);
  gmtime_r(&now, &utc_tm);

  int N = local_tm.tm_yday + 1;

  // Solar declination (radians)
  float declination = 0.4093f * sin(2.0f * M_PI * (284 + N) / 365.0f);
  float latRad = LATITUDE * (M_PI / 180.0f);

  // Hour angle for standard zenith
  float cosOmega = -tan(latRad) * tan(declination);

  if (cosOmega >= 1.0f)
  {
    // Polar night: check again in an hour
    nextSolarCheckEpoch = now + 3600;
    return 0.5f;
  }
  if (cosOmega <= -1.0f)
  {
    // Midnight sun: check again in an hour
    nextSolarCheckEpoch = now + 3600;
    return 1.0f;
  }

  float omega = acos(cosOmega) * (180.0f / M_PI);

  // Solar noon adjusted for UTC offset
  time_t local_sec = mktime(&local_tm);
  time_t utc_sec = mktime(&utc_tm);
  float gmtOffsetHours = (float)difftime(local_sec, utc_sec) / 3600.0f;
  float solarNoon = 12.0f - (LONGITUDE / 15.0f) + gmtOffsetHours;

  float dawnHour = solarNoon - (omega / 15.0f);
  float duskHour = solarNoon + (omega / 15.0f);

  // Local time in fractional hours
  float currentHour = local_tm.tm_hour + (local_tm.tm_min / 60.0f) + (local_tm.tm_sec / 3600.0f);

  float halfWindowHours = (TWILIGHT_TRANSITION_MINUTES / 60.0f) / 2.0f;
  float dawnStart = dawnHour - halfWindowHours;
  float dawnEnd = dawnHour + halfWindowHours;
  float duskStart = duskHour - halfWindowHours;
  float duskEnd = duskHour + halfWindowHours;

  float dayFactor = 0.0f;

  if (currentHour >= dawnEnd && currentHour < duskStart)
  {
    // Full daylight: schedule wake-up exactly when dusk transition begins
    dayFactor = 1.0f;
    float secondsUntilDusk = (duskStart - currentHour) * 3600.0f;
    nextSolarCheckEpoch = now + (time_t)max(1.0f, secondsUntilDusk);
  }
  else if (currentHour >= duskEnd || currentHour < dawnStart)
  {
    // Full night: schedule wake-up exactly when dawn transition begins
    dayFactor = 0.0f;
    float hoursUntilDawn = (currentHour < dawnStart) ? (dawnStart - currentHour)
                                                     : (24.0f - currentHour + dawnStart);
    nextSolarCheckEpoch = now + (time_t)max(1.0f, hoursUntilDawn * 3600.0f);
  }
  else
  {
    // In active twilight transition: step every 15s (captures single-unit changes at max slope)
    nextSolarCheckEpoch = now + 15;

    if (currentHour < dawnEnd)
    {
      // Dawn window: ramping UP (night -> day)
      float progress = (currentHour - dawnStart) / (2.0f * halfWindowHours);
      dayFactor = 0.5f * (1.0f - cos(constrain(progress, 0.0f, 1.0f) * M_PI));
    }
    else
    {
      // Dusk window: ramping DOWN (day -> night)
      float progress = (currentHour - duskStart) / (2.0f * halfWindowHours);
      dayFactor = 0.5f * (1.0f + cos(constrain(progress, 0.0f, 1.0f) * M_PI));
    }
  }

  // Scale: 0.5 (night) to 1.0 (day)
  return 0.5f + (0.5f * dayFactor);
#else
  return 1.0f;
#endif
}

// manually adjust the brightness offset via the rotary encoder
int manualBrightness(bool useKnob)
{
  if (!useKnob)
    return settings.brightness;

#ifdef ENCODER
  // use the right knob as a brightness increment/decrement
  static int lastRightKnob = 0;
  int knob = knobRight.getCount();
  if (knob != lastRightKnob)
  {
    if (knob > lastRightKnob)
      settings.brightness -= KNOB_INCREMENT;
    else
      settings.brightness += KNOB_INCREMENT;

    settings.brightness = constrain(settings.brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    lastRightKnob = knob;

    DB_PRINTF("brightness = %d\r\n", settings.brightness);
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
#endif // DEBUG
#endif // ENCODER

  return settings.brightness;
}

//
// NOTE: adjustBrightness and adjustSpeed are currently called from the individual
// modes as each mode may want to do something different with the knobs (ie the 'snake' mode)
//

// update the FastLED brightness based on getSolarBrightnessScale and settings
void adjustBrightness(bool useKnob)
{
  manualBrightness(useKnob);

  static int LEDbrightness = 0;
  time_t now = time(nullptr);

  // Recompute immediately if:
  // 1. Encoder knob was turned (useKnob)
  // 2. REST API modified settings (brightness_dirty)
  // 3. Next scheduled recalculation time has arrived
  if (useKnob || brightness_dirty || (now >= nextSolarCheckEpoch))
  {
    brightness_dirty = false;

    int newBrightness = constrain((int)round(settings.brightness * getSolarBrightnessScale()), MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    if (newBrightness != LEDbrightness)
    {
      LEDbrightness = newBrightness;
      DB_PRINTF("new brightness = %d\r\n", newBrightness);

      FastLED.setBrightness(dim8_raw(LEDbrightness));
      leds_dirty = true;
    }
  }
}

//
// use the left rotary knob to adjust the speed of the kaleidoscope
//

// The increment is defined to require two complete knob rotations to go from min to max
// Each knob roration is 20 clicks.
#define INCREMENT_SPEED (MAX_SPEED / 40)
int adjustSpeed()
{
#ifdef ENCODER
  static int lastLeftKnob = 0;
  int new_speed = settings.speed;

  int knob = knobLeft.getCount();
  if (knob != lastLeftKnob)
  {
    if (knob > lastLeftKnob)
      new_speed -= INCREMENT_SPEED;
    else
      new_speed += INCREMENT_SPEED;

    settings.speed = constrain(new_speed, 0, MAX_SPEED);
    lastLeftKnob = knob;

    DB_PRINTF("ms between frames = %d\r\n", settings.speed);
  }
#endif
  return settings.speed;
}

#ifdef REST
void saveSettings(AsyncWebServerRequest *request, JsonVariant &json)
{
  const JsonObject &jsonObj = json.as<JsonObject>();

  DB_PRINTLN("REST saveSettings:");

  // update the brightness (if it was passed)
  JsonVariant brightness = jsonObj["brightness"];
  if (!brightness.isNull())
  {
    settings.brightness = constrain((int)brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    brightness_dirty = true; // Triggers immediate adjustment in loop
    DB_PRINTF("  brightness = %d\r\n", settings.brightness);
  }

  JsonVariant speed = jsonObj["speed"];
  if (!speed.isNull())
  {
    settings.speed = constrain((int)speed, 0, MAX_SPEED);
    DB_PRINTF("  speed = %d\r\n", settings.speed);
  }

  const char *modeName = jsonObj["mode"];
  if (modeName)
  {
    for (int x = 0; x < kaleidoscopeModes; x++)
    {
      if (String(KaleidoscopeModeLUT[x].modeName).equalsIgnoreCase(String(modeName)))
      {
        setKaleidoscopeMode(x);
        DB_PRINTF("  mode = %s\r\n", KaleidoscopeModeLUT[x].modeName);
        break;
      }
    }
  }

  const char *drawStyle = jsonObj["drawStyle"];
  if (drawStyle)
  {
    for (int x = 0; x < N_DRAW_STYLES; x++)
    {
      if (String(drawStylesLUT[x]).equalsIgnoreCase(String(drawStyle)))
      {
        setDrawStyle(x);
        DB_PRINTF("  drawStyle = %s\r\n", drawStylesLUT[x]);
        break;
      }
    }
  }

#ifdef TIME
  const char *clockFace = jsonObj["clockFace"];
  if (clockFace)
  {
    for (int x = 0; x < clockFaces; x++)
    {
      if (String(clockFaceLUT[x].faceName).equalsIgnoreCase(String(clockFace)))
      {
        setClockFace(x);
        DB_PRINTF("  clockFace = %s\r\n", clockFaceLUT[x].faceName);
        break;
      }
    }
  }

  JsonVariant clockColor = jsonObj["clockColor"];
  if (!clockColor.isNull())
  {
    uint32_t color;

    sscanf(clockColor, "#%06X", &color);
    settings.clockColor = CRGB(color);
    DB_PRINTF("  clockColor = #%06X\r\n", settings.clockColor.r << 16 | settings.clockColor.g << 8 | settings.clockColor.b);
  }
#endif // TIME

  request->send(200, "text/plain", "OK");
}

void getSettings(AsyncWebServerRequest *request)
{
  JsonDocument doc;
  String response;

  doc["mode"] = KaleidoscopeModeLUT[settings.mode].modeName;
  doc["drawStyle"] = drawStylesLUT[settings.drawStyle];
  doc["brightness"] = settings.brightness;
  doc["speed"] = settings.speed;
#ifdef TIME
  doc["clockFace"] = clockFaceLUT[settings.clockFace].faceName;
  char color[8];
  sprintf(color, "#%06X", settings.clockColor.r << 16 | settings.clockColor.g << 8 | settings.clockColor.b);
  doc["clockColor"] = color;
#endif // TIME

  serializeJson(doc, response);
  DB_PRINTLN("REST getSettings: " + response);
  request->send(200, "text/json", response);
}

void getModes(AsyncWebServerRequest *request)
{
  // allocate the memory for the document
  JsonDocument doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the names
  for (int x = 0; x < kaleidoscopeModes; x++)
  {
    if (KaleidoscopeModeLUT[x].showInRESTAPI)
      array.add(KaleidoscopeModeLUT[x].modeName);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  DB_PRINTLN("REST getModes: " + response);
  request->send(200, "text/json", response);
}

#ifdef TIME
void getFaces(AsyncWebServerRequest *request)
{
  // allocate the memory for the document
  JsonDocument doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the names
  for (int x = 0; x < clockFaces; x++)
  {
    array.add(clockFaceLUT[x].faceName);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  DB_PRINTLN("REST getFaces: " + response);
  request->send(200, "text/json", response);
}
#endif // TIME

void getDrawStyles(AsyncWebServerRequest *request)
{
  // allocate the memory for the document
  JsonDocument doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add the draw style names
  for (int x = 0; x < N_DRAW_STYLES; x++)
  {
    array.add(drawStylesLUT[x]);
  }

  // serialize the array and send the result
  String response;
  serializeJson(doc, response);
  DB_PRINTLN("REST getDrawStyles: " + response);
  request->send(200, "text/json", response);
}

#endif // REST

//
// SETUP FUNCTION -- RUNS ONCE AT PROGRAM START ----------------------------
//

void setup()
{
#ifdef DEBUG
  // 3 second delay for recovery
  delay(3000);

  Serial.begin(921600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
  DB_PRINTLN("\nStarting Kaleidoscope on " + String(ARDUINO_BOARD));
#endif

  // initialize the settings from persistent storage
  settingsSetup();

#ifdef WIFI
  // connect to wifi or enter AP mode so it can be configured
  wifi_setup();

#ifdef REST
  // setup the REST API endpoints and handlers
  webServer.on("/api/settings", HTTP_GET, getSettings);
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/settings", saveSettings);
  webServer.addHandler(handler);
  webServer.on("/api/modes", HTTP_GET, getModes);
#ifdef TIME
  webServer.on("/api/faces", HTTP_GET, getFaces);
#endif // TIME
  webServer.on("/api/drawstyles", HTTP_GET, getDrawStyles);
#endif
#endif // WIFI

#ifdef PHOTOCELL
  // Testing shows that the internal pullup resistors on the ESP32 are complete crap and
  // unusable. Probably why every example does their own external pullup/down resistors.
  pinMode(PHOTOCELL_PIN, INPUT /*_PULLUP*/);
#endif

  randomSeed(esp_random()); // Get a random number from the hardware RNG

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
  ESP32Encoder::useInternalWeakPullResistors = puType::none;
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
  DB_PRINTLN(KaleidoscopeModeLUT[settings.mode].modeName);
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
    previousKaleidoscopeMode();

  // Right button pressed?
  rightButton.update();
  if (rightButton.pressed())
    nextKaleidoscopeMode();
#endif

#ifdef WIFI
  // check that WiFi is still connected and reconnect if necessary
  wifi_loop();
#endif // WIFI

  if (eraseLEDs)
  {
    // clear the LED strips before drawing the new mode
    FastLED.clear(true);
    leds_dirty = true;
    eraseLEDs = false;
  }

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  KaleidoscopeModeLUT[settings.mode].renderFunc();

#ifdef TIME
  // if the frame has been updated, overlay the clock face (can be a null clock face - see mode_select_clock_face())
  if ((KaleidoscopeModeLUT[settings.mode].renderFunc != mode_off) && leds_dirty)
    clockFaceLUT[settings.clockFace].renderFunc();
#endif // TIME

#ifdef WEATHER
  if (WiFi.status() == WL_CONNECTED)
    weather_loop(current_weather);
#endif // WEATHER

// Show an activity spinner and the current fps. After 500 ms of no LED updates show 0 fps.
// This is to prevent the fps flickering between 0 fps and x fps when there are no updates
// to display (i.e. not calling FastLED.Show every loop).
#ifdef DEBUG_FPS
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
// #define DEBUG_SPINNER
#ifdef DEBUG_SPINNER
    static const char *spinner = "|/-\\";
    static int spinner_index = 0;

    DB_PRINTF("\r%c", spinner[spinner_index]);
    spinner_index = (spinner_index + 1) % sizeof(spinner);
#endif                  // DEBUG_SPINNER
    leds_dirty = false; // clear the dirty flag before showing the frame or changes via asyncronous REST calls will fail to be drawn
    FastLED.show();
  }

  // persist any changes to the settings
  EVERY_N_SECONDS(5)
  {
    settingsPersist();
  }
}