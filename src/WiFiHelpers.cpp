#include "main.h"
#include "debug.h"
#ifdef WIFI
#include <MycilaESPConnect.h>
#include "settings.h"
#ifdef TIME
#include "RealTimeClock.h"
#endif // TIME
#include "WebUI.h"

AsyncWebServer webServer(80);
static Mycila::ESPConnect espConnect(webServer);
static Mycila::ESPConnect::Config espConnectConfig;

void wifi_setup(void)
{
    // reuse a potentially set hostname, or set a default one
    espConnect.loadConfiguration(espConnectConfig);
    if (!espConnectConfig.hostname.length())
    {
        espConnectConfig.hostname = "kaleidoscope";
    }

    // clear persisted config
    webServer.on("/clear", HTTP_GET, [&](AsyncWebServerRequest *request)
                 {
        DB_PRINTLN("Clearing configuration...");
        espConnect.clearConfiguration();
        request->send(200);
        ESP.restart(); });

    // restart the device
    webServer.on("/restart", HTTP_GET, [&](AsyncWebServerRequest *request)
                 {
        DB_PRINTLN("Restarting...");
        request->send(200);
        ESP.restart(); });

    // network state listener is required here in async mode
    espConnect.listen([](__unused Mycila::ESPConnect::State previous, Mycila::ESPConnect::State state)
                      {
        switch (state) {
        case Mycila::ESPConnect::State::NETWORK_CONNECTED:
        case Mycila::ESPConnect::State::AP_STARTED:
            // Setup the web UI handlers (kaleidoscope page and API endpoints)
            WebUI_setup(&webServer, &espConnect);
            webServer.begin();
            break;

        case Mycila::ESPConnect::State::NETWORK_DISCONNECTED:
            webServer.end();
            break;

        default:
            break;
        } });

    espConnect.setAutoRestart(true);
    espConnect.setBlocking(false);

    DB_PRINTLN("====> Trying to connect to saved WiFi or will start captive portal in the background...");

    // Use the automatic form of begin that handles NVS save/load
    espConnect.begin(espConnectConfig.hostname.c_str(), (espConnectConfig.hostname + "AP").c_str());

    DB_PRINTLN("====> WiFi setup() completed...");

#ifdef TIME
    // intialize the real time clock
    rtc_setup();
#endif
}

void wifi_loop(void)
{
    espConnect.loop();

#ifdef DEBUG
    EVERY_N_SECONDS(5)
    {
        JsonDocument doc;
        espConnect.toJson(doc.to<JsonObject>());
        serializeJsonPretty(doc, Serial);
        DB_PRINTLN();
    }
#endif // DEBUG
}
#endif // WIFI
