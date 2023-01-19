#include "main.h"
#include "modes.h"

#ifdef BOUNCE
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>
#endif // BOUNCE

#ifdef WIFI
#include "WiFiHelpers.h"
#include "WebUI.h"

#ifdef REST
#include <AsyncJson.h>
#include <ArduinoJson.h>
#endif // REST

#ifdef ALEXA
// https://github.com/Aircoookie/Espalexa
#define ESPALEXA_ASYNC
#define ESPALEXA_MAXDEVICES 1
#define ESPALEXA_DEBUG
#include <Espalexa.h>
#endif // ALEXA

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

#ifdef ALEXA
Espalexa espalexa;
#endif

//
// BRIGHTNESS helpers -------------------------------------------------
//

#define MIN_BRIGHTNESS 32                    // the minimum brightness we want (above zero so it doesn't go completely dark)
#define MAX_BRIGHTNESS 255                   // the max possible brightness
#define MAX_BRIGHTNESS_READING 1024          // set this to the highest reading you get from the photocell
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
#endif // PHOTOCELL

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
#endif // PHOTOCELL

  // ensure we stay between our min and max valid values
  return constrain(map(lastPhotocell, 0, MAX_BRIGHTNESS_READING, 0, MAX_BRIGHTNESS), 0, MAX_BRIGHTNESS);
}

// manually adjust the brightness of the LED strips up or down from the ambientBrightness
static int LEDBrightnessManualOffset = MAX_BRIGHTNESS;
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
#endif // DEBUG
#endif // ENCODER

  return LEDBrightnessManualOffset;
}

//
// NOTE: adjustBrightness and adjustSpeed are currently called from the individual
// modes as each mode may want to do something different with the knobs (ie the 'snake' mode)
//

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
    // TODO: need to obtain the settings semaphore before changing the settings
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
uint8_t kaleidoscopeSpeed = KALEIDOSCOPE_DEFAULT_SPEED;
int adjustSpeed()
{
#ifdef ENCODER
  static int lastLeftKnob = 0;
  int new_speed = kaleidoscopeSpeed;

  int knob = knobLeft.getCount();
  if (knob != lastLeftKnob)
  {
    if (knob > lastLeftKnob)
      new_speed -= INCREMENT_SPEED;
    else
      new_speed += INCREMENT_SPEED;

    // TODO: need to obtain the settings semaphore before changing the settings
    kaleidoscopeSpeed = constrain(new_speed, 0, KALEIDOSCOPE_MAX_SPEED);
    lastLeftKnob = knob;

    DB_PRINTF("ms between frames = %d\r\n", kaleidoscopeSpeed);
  }
#endif
  return kaleidoscopeSpeed;
}

#ifdef REST
//
// When WiFi is actively transmitting/receiving data, the FastLED.show() command tends to crash or hang
// the main 'loop()' thread. This is because the FastLed library disables interrupts during calls to show()
// so that it can ensure the timing is correct while updating the LED strips. To fix this correctly
// requires us to switch to a different type of LED that doesn't require such tight timing loops to update
// or to switch to a different processor that has better hardware than the ESP32. Neither of these options
// are great so I'll try to minimze the conflict using a semaphore.
//
SemaphoreHandle_t WiFiSemaphore = NULL;

// This is needed to solve some threading issues with the REST APIs coming in via
// asyncronous messages. When we get a change, just store them in the temporary
// settings object then apply them on the primary thread so we aren't racing
// with the current display loop.
typedef struct
{
  int LEDBrightnessManualOffset;
  int kaleidoscopeSpeed;
  int kaleidoscopeMode;
  int drawStyle;
#ifdef TIME
  int clockFace;
  CRGB clockColor;
#endif // TIME
} kaleidoscope_settings;
kaleidoscope_settings settings = {
    LEDBrightnessManualOffset,
    kaleidoscopeSpeed,
    kaleidoscopeMode,
    drawStyle,
#ifdef TIME
    clockFace,
    CRGB::White       // BUGBUG: Ugly hack to work around improper sequence of global initialization by startup runtime. Should be 'clockColor'.
#endif // TIME
};
volatile bool newSettings = false;

void applySettings(kaleidoscope_settings &settings)
{
  if (!newSettings)
    return;

  // all validation of values happens in saveSettings()
  DB_PRINTF("REST applySettings: %s\r\n", newSettings ? "TRUE" : "FALSE");
  if (LEDBrightnessManualOffset != settings.LEDBrightnessManualOffset)
  {
    LEDBrightnessManualOffset = settings.LEDBrightnessManualOffset;
  }

  if (kaleidoscopeSpeed != settings.kaleidoscopeSpeed)
  {
    kaleidoscopeSpeed = settings.kaleidoscopeSpeed;
  }

  if (kaleidoscopeMode != settings.kaleidoscopeMode)
  {
    setKaleidoscopeMode(settings.kaleidoscopeMode);
  }

  if (drawStyle != settings.drawStyle)
  {
    setDrawStyle(settings.drawStyle);
  }

#ifdef TIME
  if (clockFace != settings.clockFace)
  {
    setClockFace(settings.clockFace);
  }

  if (clockColor != settings.clockColor)
  {
    clockColor = settings.clockColor;
  }
#endif // TIME

  newSettings = false;
}

void saveSettings(AsyncWebServerRequest *request, JsonVariant &json)
{
  const JsonObject &jsonObj = json.as<JsonObject>();

  // update the brightness (if it was passed)
  DB_PRINTLN("REST saveSettings:");
  JsonVariant brightness = jsonObj["brightness"];
  if (!brightness.isNull())
  {
    settings.LEDBrightnessManualOffset = constrain((int)brightness, -MAX_BRIGHTNESS, MAX_BRIGHTNESS);
    newSettings = true;
    DB_PRINTF("  brightness = %d\r\n", settings.LEDBrightnessManualOffset);
  }

  JsonVariant speed = jsonObj["speed"];
  if (!speed.isNull())
  {
    settings.kaleidoscopeSpeed = constrain((int)speed, 0, KALEIDOSCOPE_MAX_SPEED);
    newSettings = true;
    DB_PRINTF("  speed = %d\r\n", settings.kaleidoscopeSpeed);
  }

  const char *modeName = jsonObj["mode"];
  if (modeName)
  {
    for (int x = 0; x < kaleidoscopeModes; x++)
    {
      if (String(KaleidoscopeModeLUT[x].modeName).equalsIgnoreCase(String(modeName)))
      {
        settings.kaleidoscopeMode = x;
        newSettings = true;
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
      if (String(drawStyles[x]).equalsIgnoreCase(String(drawStyle)))
      {
        settings.drawStyle = x;
        newSettings = true;
        DB_PRINTF("  drawStyle = %s\r\n", drawStyles[x]);
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
        settings.clockFace = x;
        newSettings = true;
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
    newSettings = true;
    DB_PRINTF("  clockColor = #%06X\r\n", settings.clockColor.r << 16 | settings.clockColor.g << 8 | settings.clockColor.b);
  }
#endif // TIME

  request->send(200, "text/plain", "OK");
}

void getSettings(AsyncWebServerRequest *request)
{
  DynamicJsonDocument doc(512);
  String response;

  doc["mode"] = KaleidoscopeModeLUT[kaleidoscopeMode].modeName;
  doc["drawStyle"] = drawStyles[drawStyle];
  doc["brightness"] = LEDBrightnessManualOffset;
  doc["speed"] = kaleidoscopeSpeed;
#ifdef TIME
  doc["clockFace"] = clockFaceLUT[clockFace].faceName;
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
  DynamicJsonDocument doc(JSON_ARRAY_SIZE(kaleidoscopeModes));

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
  DynamicJsonDocument doc(JSON_ARRAY_SIZE(clockFaces));

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
  DynamicJsonDocument doc(JSON_ARRAY_SIZE(N_DRAW_STYLES));

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

#endif // REST

#ifdef ALEXA
// our Alexa callback function
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
#endif // ALEXA

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
  DB_PRINTLN("\nStarting Kaleidoscope on " + String(ARDUINO_BOARD));
#endif

#ifdef WIFI
  // Semaphore cannot be used before a call to vSemaphoreCreateBinary().
  WiFiSemaphore = xSemaphoreCreateBinary();
  if (WiFiSemaphore != NULL)
  {
    // The semaphore was created successfully and can now be used. Note: binary semaphores
    // created using xSemaphoreCreateBinary() are created in a state such that the
    // the semaphore must first be 'given' before it can be 'taken'.
    xSemaphoreGive(WiFiSemaphore);
  }

  // connect to wifi or enter AP mode so it can be configured
  wifi_setup();

  // setup the home page and other web UI (WiFi settings, upgrade, etc)
  WebUI_setup(&webServer);

  // setup the REST API endpoints and handlers
#ifdef REST
  webServer.on("/api/settings", HTTP_GET, getSettings);
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/settings", saveSettings);
  webServer.addHandler(handler);
  webServer.on("/api/modes", HTTP_GET, getModes);
#ifdef TIME
  webServer.on("/api/faces", HTTP_GET, getFaces);
#endif // TIME
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
                         } });

  // Define your devices here.
  espalexa.addDevice("Hue", hueChanged, EspalexaDeviceType::extendedcolor); // color + color temperature

  // give espalexa a pointer to your server object so it can use your server instead of creating its own
  espalexa.begin(&webServer);
#endif

#ifndef ALEXA
  webServer.begin(); // omit this since it will be done by espalexa.begin(&webServer)
#endif
#endif // WIFI

#ifdef PHOTOCELL
  // Testing shows that the internal pullup resistors on the ESP32 are complete crap and
  // unusable. Probably why every example does their own external pullup/down resistors.
  pinMode(PHOTOCELL_PIN, INPUT /*_PULLUP*/);

// D:\src\kaleidoscope\.pio\libdeps\node32s\ESP32Encoder\src\ESP32Encoder.cpp
// gpio_pullup_en((gpio_num_t)PHOTOCELL_PIN);
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
  DB_PRINTLN(KaleidoscopeModeLUT[kaleidoscopeMode].modeName);
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

#ifdef ALEXA
  espalexa.loop();
#endif // ALEXA
#endif // WIFI

  // Render one frame in current mode. To control the speed of updates, use the
  // EVERY_N_MILLISECONDS(N) macro to only update the frame when it is needed.
  // Also be sure to set leds_dirty = true so that the updated frame will be displayed.
  (*KaleidoscopeModeLUT[kaleidoscopeMode].renderFunc)();

  // draw the clock face (can be a null clock face - see mode_select_clock_face())
#ifdef TIME
  // don't draw the clock if we're in 'off' mode
  if (KaleidoscopeModeLUT[kaleidoscopeMode].renderFunc != mode_off)
    drawClock();
#endif // TIME

#ifdef WEATHER
  if (WiFi.status() == WL_CONNECTED)
    weather_loop(current_weather);
#endif // WEATHER

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
#ifdef WIFI
    // See if we can obtain the semaphore. If not, just skip this update.
    if (WiFiSemaphore && xSemaphoreTake(WiFiSemaphore, 1) == pdTRUE)
    {
#endif
      // We were able to obtain the semaphore and can now update the LEDs and disable interrupts.
      FastLED.show();
      leds_dirty = false;
#ifdef WIFI
      // We have finished updating the LEDs.  Release the semaphore.
      xSemaphoreGive(WiFiSemaphore);
    }
#endif
  }
}
