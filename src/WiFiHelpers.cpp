#include "main.h"
#include "debug.h"
#ifdef WIFI
#include <AsyncTCP.h>
#include <MycilaESPConnect.h>
#include "settings.h"
#ifdef TIME
#include "RealTimeClock.h"
#endif // TIME
#include "WebUI.h"

AsyncWebServer webServer(80);
static Mycila::ESPConnect espConnect(webServer);
static Mycila::ESPConnect::Config espConnectConfig;

// Solar calculation constants (Set to your local coordinates)
float currentLatitude = 38.048136f;   // Positive = North, Negative = South
float currentLongitude = -79.481413f; // Positive = East, Negative = West
bool locationAcquired = false;

void fetchLocationFromIP_Async()
{
    // we only need to do this once
    if (locationAcquired || !espConnect.isConnected())
        return;

    static bool fetchInProgress = false;
    if (fetchInProgress)
        return;

    fetchInProgress = true;

    // Allocate client and buffer on heap so callbacks can access them
    AsyncClient *client = new AsyncClient();
    String *responseBuffer = new String();

    client->onConnect([client](void *arg, AsyncClient *c)
                      {
    DB_PRINTLN("AsyncTCP connected to ip-api.com. Sending HTTP request...");

    // Send standard HTTP/1.1 GET request
    // 'Connection: close' ensures the server cleanly closes after sending the body
    const char *httpRequest =
        "GET /json/?fields=status,message,lat,lon,timezone HTTP/1.1\r\n"
        "Host: ip-api.com\r\n"
        "User-Agent: ESP32-Kaleidoscope/1.0\r\n"
        "Connection: close\r\n\r\n";

    c->write(httpRequest); }, NULL);

    client->onData([responseBuffer](void *arg, AsyncClient *c, void *data, size_t len)
                   {
    // Append incoming TCP chunk
    responseBuffer->concat((const char *)data, len); }, NULL);

    client->onDisconnect([client, responseBuffer](void *arg, AsyncClient *c)
                         {
    DB_PRINTLN("AsyncTCP disconnected. Parsing GeoIP response...");

    // Find the end of HTTP headers (\r\n\r\n)
    int bodyIndex = responseBuffer->indexOf("\r\n\r\n");
    if (bodyIndex != -1)
    {
      String jsonPayload = responseBuffer->substring(bodyIndex + 4);

      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, jsonPayload);

      if (!err)
      {
        if (doc["status"] == "success")
        {
          currentLatitude = doc["lat"];
          currentLongitude = doc["lon"];
          locationAcquired = true;

          DB_PRINTF("GeoIP acquired: %.4f, %.4f (%s)\r\n",
                    currentLatitude,
                    currentLongitude,
                    doc["timezone"].as<const char *>());
        }
        else
        {
          const char *msg = doc["message"] | "unknown error";
          DB_PRINTF("GeoIP failed with status 'fail': %s\r\n", msg);
        }
      }
      else
      {
        DB_PRINTF("GeoIP JSON parse error: %s\r\n", err.c_str());
      }
    }
    else
    {
      DB_PRINTLN("GeoIP failed: HTTP headers malformed or missing.");
    }

    // Free heap memory and reset guard flag
    delete responseBuffer;
    delete client;
    fetchInProgress = false; }, NULL);

    client->onError([client, responseBuffer](void *arg, AsyncClient *c, int8_t error)
                    {
    DB_PRINTF("AsyncTCP error: %d\r\n", error);

    delete responseBuffer;
    delete client;
    fetchInProgress = false; }, NULL);

    // Connect to ip-api.com on port 80 (non-blocking)
    if (!client->connect("ip-api.com", 80))
    {
        DB_PRINTLN("AsyncTCP connect call failed immediately.");
        delete responseBuffer;
        delete client;
        fetchInProgress = false;
    }
}

void wifi_setup(void)
{
    // reuse a potentially set hostname, or set a default one
    espConnect.loadConfiguration(espConnectConfig);
    if (!espConnectConfig.hostname.length())
    {
        espConnectConfig.hostname = "kaleidoscope";
    }

    // It is possible to serve a logo. Has to be called before calling "begin()"
#if 0    
    webServer.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "image/png", favicon_png, sizeof(favicon_png)); });

    webServer.on("/logo", HTTP_GET, [&](AsyncWebServerRequest *request)
                 {
        AsyncWebServerResponse* response = request->beginResponse(200, "image/png");
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "public, max-age=900");
        request->send(response); });
#endif

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
        switch (state) 
        {
            case Mycila::ESPConnect::State::NETWORK_CONNECTED:
                fetchLocationFromIP_Async();
                // fall through to AP_STARTED to ensure web server is running
            case Mycila::ESPConnect::State::AP_STARTED:
                WebUI_setup(&webServer, &espConnect);
                webServer.begin();
                break;
            case Mycila::ESPConnect::State::NETWORK_DISCONNECTED:
                webServer.end();
                break;

            default:
                break;
        } });

    espConnect.setConnectTimeout(20);
    espConnect.setCaptivePortalTimeout(40);
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
}
#endif // WIFI
