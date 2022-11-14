#include "main.h"
#ifdef WIFI
#include <WiFi.h>
#include "WiFiHelpers.h"

#ifdef TIME
#include "RealTimeClock.h"
#include <Preferences.h>
#endif // TIME

#ifdef DRD
// https://github.com/khoih-prog/ESP_DoubleResetDetector
#define ESP_DRD_USE_EEPROM true

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

#include <ESP_DoubleResetDetector.h>

DoubleResetDetector *drd;
#endif // DRD

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESPAsync_WiFiManager.h>
#define USE_AVAILABLE_PAGES true

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
#define USE_ESP_WIFIMANAGER_NTP true

// Just use enough to save memory. On ESP8266, can cause blank ConfigPortal screen
// if using too much memory
#define USING_AMERICA true

// https://github.com/khoih-prog/ESPAsync_WiFiManager
#include <ESPAsync_WiFiManager.h>
#include <ESPAsyncWebServer.h>

#define HTTP_PORT 80
AsyncWebServer webServer(HTTP_PORT);

void wifi_setup(const char *iHostname)
{
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

    // Local intialization. Once its business is done, there is no need to keep it around
    AsyncDNSServer dnsServer;
    ESPAsync_WiFiManager wifiManager(&webServer, &dnsServer, "Kaleidoscope");

    // Set config portal channel, default = 1. Use 0 => random channel from 1-13
    wifiManager.setConfigPortalChannel(0);

    if (wifiManager.WiFi_SSID() == "")
    {
        DB_PRINTLN(F("No AP credentials"));
        initialConfig = true;
    }

    if (initialConfig)
    {
        DB_PRINTLN(F("Starting Config Portal"));

        // initial config, disable timeout.
        wifiManager.setConfigPortalTimeout(0);

        wifiManager.startConfigPortal("KaleidoscopeAP");
    }
    else
    {
        // Give 2 minutes to configure WiFi, otherwise, just go into kaleidoscope mode without it
        wifiManager.setConfigPortalTimeout(120);

        wifiManager.autoConnect("KaleidoscopeAP");
    }

    // report on our WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        DB_PRINT(F("Connected. Local IP: "));
        DB_PRINTLN(WiFi.localIP());
    }
    else
    {
        DB_PRINTLN(wifiManager.getStatus(WiFi.status()));
    }

#ifdef TIME
    // this only returns a value during the initial config step (call wifiManager.resetSettings() to test)
    // store the string in persistant storage for later use
    String timezoneName = wifiManager.getTimezoneName();
    if (timezoneName.length())
    {
        // write the timezone string into persistant memory
        DB_PRINTF("Saving timezone '%s'\r\n", timezoneName.c_str());
        const char *tz = wifiManager.getTZ(timezoneName);
        Preferences preferences;
        preferences.begin("kaleidoscope", false);
        preferences.putString("tz", tz);
        preferences.end();
    }

    // intialize the real time clock
    rtc_setup();
#endif
}

void wifi_loop(void)
{
#ifdef DRD
    // Call the double reset detector loop method every so often so that it can recognise when the timeout expires.
    // You can also call drd.stop() when you wish to no longer consider the next reset as a double reset.
    drd->loop();
#endif

    if ((WiFi.status() != WL_CONNECTED))
    {
        DB_PRINTLN(F("\nWiFi lost. Attempting to reconnect"));

        // Local intialization. Once its business is done, there is no need to keep it around
        AsyncDNSServer dnsServer;
        ESPAsync_WiFiManager wifiManager(&webServer, &dnsServer, "Kaleidoscope");

        // Set config portal channel, default = 1. Use 0 => random channel from 1-13
        wifiManager.setConfigPortalChannel(0);

        // attempt to reconnect
        wifiManager.autoConnect("KaleidoscopeAP");

        // report on our WiFi connection status
        if (WiFi.status() == WL_CONNECTED)
        {
            DB_PRINT(F("Connected. Local IP: "));
            DB_PRINTLN(WiFi.localIP());
        }
        else
        {
            DB_PRINTLN(wifiManager.getStatus(WiFi.status()));
        }
    }
}
#endif // WIFI
