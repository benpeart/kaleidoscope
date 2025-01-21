#include "main.h"
#include "debug.h"
#ifdef WIFI
#include <WiFi.h>
#include "WiFiHelpers.h"
#include "WebUI.h"
#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncWebServer.h>
#ifdef OTA
// https://github.com/ayushsharma82/AsyncElegantOTA
#include <ElegantOTA.h>
#endif // OTA

#ifdef TIME
#include "RealTimeClock.h"
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

#define HTTP_PORT 80
AsyncWebServer webServer(HTTP_PORT);
DNSServer dnsServer;

void wifi_setup(const char *iHostname)
{
    WiFi.setHostname(iHostname);

    // connect to wifi or enter AP mode so it can be configured
#ifdef DRD
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    if (drd->detectDoubleReset())
    {
        DB_PRINTLN("Double reset detected");
        initialConfig = true;
    }
#endif

    // Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wifiManager(&webServer, &dnsServer);

    if (initialConfig)
    {
        DB_PRINTLN(F("Starting Config Portal"));

        // initial config, disable timeout.
        wifiManager.setConfigPortalTimeout(0);

        wifiManager.startConfigPortal(iHostname);
    }
    else
    {
        // Give 2 minutes to configure WiFi, otherwise, just go into kaleidoscope mode without it
        wifiManager.setConfigPortalTimeout(120);

        wifiManager.autoConnect(iHostname);
    }

    // report on our WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        DB_PRINT(F("Connected. Local IP: "));
        DB_PRINTLN(WiFi.localIP());
    }

    // setup the home page and other web UI (WiFi settings, upgrade, etc)
    WebUI_setup(&webServer);

#ifdef OTA
    // Add the ElegantOTA UI and require a username/password to update the firmware
    ElegantOTA.begin(&webServer, "admin", "admin");
    DB_PRINTLN(F("OTA web server started."));
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

#ifdef TIME
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

#ifdef OTA
    // will reboot the system 2 seconds after an upgrade
    ElegantOTA.loop();
#endif

    if ((WiFi.status() != WL_CONNECTED))
    {
        DB_PRINTLN(F("\nWiFi lost. Attempting to reconnect"));

        // Local intialization. Once its business is done, there is no need to keep it around
        AsyncWiFiManager wifiManager(&webServer, &dnsServer);

        // attempt to reconnect
        wifiManager.autoConnect("KaleidoscopeAP");

        // report on our WiFi connection status
        if (WiFi.status() == WL_CONNECTED)
        {
            DB_PRINT(F("Connected. Local IP: "));
            DB_PRINTLN(WiFi.localIP());
        }
    }
}
#endif // WIFI
