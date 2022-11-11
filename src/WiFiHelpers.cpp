#include "main.h"
#ifdef WIFI
#include <WiFi.h>
#include "WiFiHelpers.h"

#ifdef TIME
#include "RealTimeClock.h"
#include <Preferences.h>
#endif // TIME

// https://github.com/khoih-prog/ESPAsync_WiFiManager
#define USE_ESP_WIFIMANAGER_NTP true
#include <ESPAsync_WiFiManager.h>
#include <ESPAsyncWebServer.h>

#ifdef DRD
// https://github.com/khoih-prog/ESP_DoubleResetDetector
#define ESP_DRD_USE_EEPROM true
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
#include <ESP_DoubleResetDetector.h>
#endif // DRD

bool initialConfig = false;
ESPAsync_WiFiManager *wifiManager;
AsyncWebServer *webServer;
#ifdef DRD
DoubleResetDetector *drd;
#endif // DRD

#if 1
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

    webServer = new AsyncWebServer(80);
    wifiManager = new ESPAsync_WiFiManager(webServer, NULL, "Kaleidoscope");
    if (wifiManager->WiFi_SSID() == "")
    {
        DB_PRINTLN(F("No AP credentials"));
        initialConfig = true;
    }
    if (initialConfig)
    {
        DB_PRINTLN(F("Starting Config Portal"));
        wifiManager->startConfigPortal("KaleidoscopeAP");
    }
    else
    {
        wifiManager->autoConnect("KaleidoscopeAP");
    }

    // report on our WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        DB_PRINT(F("Connected. Local IP: "));
        DB_PRINTLN(WiFi.localIP());
    }
    else
    {
        DB_PRINTLN(wifiManager->getStatus(WiFi.status()));
    }

#ifdef TIME
    // this only returns a value during the initial config step (call wifiManager->resetSettings() to test)
    // store the string in persistant storage for later use
    String timezoneName = wifiManager->getTimezoneName();
    if (timezoneName.length())
    {
        // write the timezone string into persistant memory
        DB_PRINTF("Saving timezone '%s'\r\n", timezoneName.c_str());
        const char *tz = wifiManager->getTZ(timezoneName);
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

        // attempt to reconnect
        wifiManager->autoConnect("KaleidoscopeAP");

        // report on our WiFi connection status
        if (WiFi.status() == WL_CONNECTED)
        {
            DB_PRINT(F("Connected. Local IP: "));
            DB_PRINTLN(WiFi.localIP());
        }
        else
        {
            DB_PRINTLN(wifiManager->getStatus(WiFi.status()));
        }
    }
}

#else
//////////////////////////////////////////////

// New connectMultiWiFi() logic from v1.7.0
// Max times to try WiFi per loop() iteration. To avoid blocking issue in loop()
// Default 1 and minimum 1.
#if !defined(MAX_NUM_WIFI_RECON_TRIES_PER_LOOP)
#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP 1
#else
#if (MAX_NUM_WIFI_RECON_TRIES_PER_LOOP < 1)
#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP 1
#endif
#endif

uint8_t connectMultiWiFi()
{
#if ESP32
// For ESP32, this better be 0 to shorten the connect time.
// For ESP32-S2/C3, must be > 500
#if (USING_ESP32_S2 || USING_ESP32_C3)
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS 500L
#else
// For ESP32 core v1.0.6, must be >= 500
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS 800L
#endif
#else
// For ESP8266, this better be 2200 to enable connect the 1st time
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS 2200L
#endif

#define WIFI_MULTI_CONNECT_WAITING_MS 500L

    uint8_t status;

    // ESP_WML_LOGINFO(F("Connecting MultiWifi..."));

    WiFi.mode(WIFI_STA);

    setHostname();

    int i = 0;
    status = wifiMulti.run();
    delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

    uint8_t numWiFiReconTries = 0;

    while ((status != WL_CONNECTED) && (numWiFiReconTries++ < MAX_NUM_WIFI_RECON_TRIES_PER_LOOP))
    {
        status = WiFi.status();

        if (status == WL_CONNECTED)
            break;
        else
            delay(WIFI_MULTI_CONNECT_WAITING_MS);
    }

    if (status == WL_CONNECTED)
    {
        // ESP_WML_LOGWARN1(F("WiFi connected after time: "), i);
        // ESP_WML_LOGWARN3(F("SSID="), WiFi.SSID(), F(",RSSI="), WiFi.RSSI());
        // ESP_WML_LOGWARN3(F("Channel="), WiFi.channel(), F(",IP="), WiFi.localIP());
    }
    else
    {
        // ESP_WML_LOGERROR(F("WiFi not connected"));

#if RESET_IF_NO_WIFI

#if USING_MRD
        // To avoid unnecessary MRD
        mrd->loop();
#else
        // To avoid unnecessary DRD
        drd->loop();
#endif

#if ESP8266
        ESP.reset();
#else
        ESP.restart();
#endif

#endif
    }

    return status;
}

//////////////////////////////////////////////

#ifndef CONFIG_TIMEOUT
#warning Default CONFIG_TIMEOUT = 60s
#define CONFIG_TIMEOUT 60000L
#endif

void startConfigurationMode()
{
#if SCAN_WIFI_NETWORKS
    configTimeout = 0; // To allow user input in CP

    WiFiNetworksFound = scanWifiNetworks(&indices);
#endif

    // turn the LED_BUILTIN ON to tell us we are in configuration mode.
    // digitalWrite(LED_BUILTIN, LED_ON);

    if ((portal_ssid == "") || portal_pass == "")
    {
        String chipID = String(ESP_getChipId(), HEX);
        chipID.toUpperCase();

        portal_ssid = "ESP_" + chipID;

        portal_pass = "MyESP_" + chipID;
    }

    WiFi.mode(WIFI_AP);

    // New
    delay(100);

    static int channel;
    // Use random channel if WiFiAPChannel == 0
    if (WiFiAPChannel == 0)
    {
        // channel = random(MAX_WIFI_CHANNEL) + 1;
        channel = (millis() % MAX_WIFI_CHANNEL) + 1;
    }
    else
        channel = WiFiAPChannel;

    // softAPConfig() must be put before softAP() for ESP8266 core v3.0.0+ to work.
    // ESP32 or ESP8266is core v3.0.0- is OK either way
    WiFi.softAPConfig(portal_apIP, portal_apIP, IPAddress(255, 255, 255, 0));

    WiFi.softAP(portal_ssid.c_str(), portal_pass.c_str(), channel);

    ESP_WML_LOGERROR3(F("\nstConf:SSID="), portal_ssid, F(",PW="), portal_pass);
    ESP_WML_LOGERROR3(F("IP="), portal_apIP.toString(), ",ch=", channel);

    delay(100); // ref: https://github.com/espressif/arduino-esp32/issues/985#issuecomment-359157428

    // Move up for ESP8266
    // WiFi.softAPConfig(portal_apIP, portal_apIP, IPAddress(255, 255, 255, 0));

    if (!server)
    {
        server = new AsyncWebServer(HTTP_PORT);
    }

    // See https://stackoverflow.com/questions/39803135/c-unresolved-overloaded-function-type?rq=1
    if (server)
    {
        server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                   { handleRequest(request); });
        server->begin();
    }

    // If there is no saved config Data, stay in config mode forever until having config Data.
    // or SSID, PW, Server,Token ="nothing"
    if (hadConfigData)
    {
        configTimeout = millis() + CONFIG_TIMEOUT;

        ESP_WML_LOGDEBUG3(F("s:millis() = "), millis(), F(", configTimeout = "), configTimeout);
    }
    else
    {
        configTimeout = 0;
        ESP_WML_LOGDEBUG(F("s:configTimeout = 0"));
    }

    configuration_mode = true;
}

void wifi_setup(const char *iHostname = "")
{
#define TIMEOUT_CONNECT_WIFI 30000

    // Turn OFF
    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LED_OFF);

#if USING_MRD
    //// New MRD ////
    mrd = new MultiResetDetector(MRD_TIMEOUT, MRD_ADDRESS);
    bool noConfigPortal = true;

    if (mrd->detectMultiReset())
#else
    //// New DRD ////
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    bool noConfigPortal = true;

    if (drd->detectDoubleReset())
#endif
    {
        // //ESP_WML_LOGINFO(F("Multi or Double Reset Detected"));
        noConfigPortal = false;
    }
    //// New DRD/MRD ////

    if (LOAD_DEFAULT_CONFIG_DATA)
    {
        DB_PRINTLN(F("======= Start Default Config Data ======="));
        displayConfigData(defaultConfig);
    }

    WiFi.mode(WIFI_STA);

    if (iHostname[0] == 0)
    {
        String _hostname = "ESP_" + String(ESP_getChipId(), HEX);
        _hostname.toUpperCase();

        getRFC952_hostname(_hostname.c_str());
    }
    else
    {
        // Prepare and store the hostname only not NULL
        getRFC952_hostname(iHostname);
    }

    // //ESP_WML_LOGINFO1(F("Hostname="), RFC952_hostname);

    hadConfigData = getConfigData();

    isForcedConfigPortal = isForcedCP();

    //// New DRD/MRD ////
    //  noConfigPortal when getConfigData() OK and no MRD/DRD'ed
    // if (getConfigData() && noConfigPortal)
    if (hadConfigData && noConfigPortal && (!isForcedConfigPortal))
    {
        hadConfigData = true;

        DB_PRINTLN(noConfigPortal ? F("bg: noConfigPortal = true") : F("bg: noConfigPortal = false"));

        for (uint16_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
        {
            if (strlen(ESP_WM_LITE_config.WiFi_Creds[i].wifi_pw) >= PASSWORD_MIN_LEN)
            {
                wifiMulti.addAP(ESP_WM_LITE_config.WiFi_Creds[i].wifi_ssid, ESP_WM_LITE_config.WiFi_Creds[i].wifi_pw);
            }
        }

        if (connectMultiWiFi() == WL_CONNECTED)
        {
            // //ESP_WML_LOGINFO(F("bg: WiFi OK."));
        }
        else
        {
            // //ESP_WML_LOGINFO(F("bg: Fail2connect WiFi"));
            //  failed to connect to WiFi, will start configuration mode
            startConfigurationMode();
        }
    }
    else
    {
        DB_PRINTLN(isForcedConfigPortal ? F("bg: isForcedConfigPortal = true") : F("bg: isForcedConfigPortal = false"));

        // If not persistent => clear the flag so that after reset. no more CP, even CP not entered and saved
        if (persForcedConfigPortal)
        {
            // //ESP_WML_LOGINFO1(F("bg:Stay forever in CP:"), isForcedConfigPortal ? F("Forced-Persistent") : (noConfigPortal ? F("No ConfigDat") : F("DRD/MRD")));
        }
        else
        {
            // //ESP_WML_LOGINFO1(F("bg:Stay forever in CP:"), isForcedConfigPortal ? F("Forced-non-Persistent") : (noConfigPortal ? F("No ConfigDat") : F("DRD/MRD")));
            clearForcedCP();
        }

        hadConfigData = isForcedConfigPortal ? true : (noConfigPortal ? false : true);

        // failed to connect to WiFi, will start configuration mode
        startConfigurationMode();
    }
}

static bool configuration_mode = false;
static bool wifi_connected = false;
static unsigned long configTimeout;
static int retryTimes = 0;
void wifi_loop(void)
{

    static bool wifiDisconnectedOnce = false;

    // Lost connection while running. Give chance to reconfig.
    // Check WiFi status every 5s and update status
    // Check twice to be sure wifi disconnected is real
    static unsigned long checkstatus_timeout = 0;
#define WIFI_STATUS_CHECK_INTERVAL 5000L

    static uint32_t curMillis;

    curMillis = millis();

#if USING_MRD
    //// New MRD ////
    // Call the mulyi reset detector loop method every so often,
    // so that it can recognise when the timeout expires.
    // You can also call mrd.stop() when you wish to no longer
    // consider the next reset as a multi reset.
    mrd->loop();
    //// New MRD ////
#else
    //// New DRD ////
    // Call the double reset detector loop method every so often,
    // so that it can recognise when the timeout expires.
    // You can also call drd.stop() when you wish to no longer
    // consider the next reset as a double reset.
    drd->loop();
    //// New DRD ////
#endif

    if (!configuration_mode && (curMillis > checkstatus_timeout))
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            wifi_connected = true;
        }
        else
        {
            if (wifiDisconnectedOnce)
            {
                wifiDisconnectedOnce = false;
                wifi_connected = false;
                DB_PRINTLN(F("r:Check&WLost"));
            }
            else
            {
                wifiDisconnectedOnce = true;
            }
        }

        checkstatus_timeout = curMillis + WIFI_STATUS_CHECK_INTERVAL;
    }

    // Lost connection in running. Give chance to reconfig.
    if (WiFi.status() != WL_CONNECTED)
    {
        // If configTimeout but user hasn't connected to configWeb => try to reconnect WiFi
        // But if user has connected to configWeb, stay there until done, then reset hardware
        if (configuration_mode && (configTimeout == 0 || millis() < configTimeout))
        {
            retryTimes = 0;

            // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
            if (String(ARDUINO_BOARD) == "ESP32S2_DEV")
            {
                delay(1);
            }

            return;
        }
        else
        {
#if RESET_IF_CONFIG_TIMEOUT
            // If we're here but still in configuration_mode, permit running TIMES_BEFORE_RESET times before reset hardware
            // to permit user another chance to config.
            if (configuration_mode && (configTimeout != 0))
            {
                DB_PRINTLN(F("r:Check RESET_IF_CONFIG_TIMEOUT"));

                if (++retryTimes <= CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET)
                {
                    DB_PRINTLN1(F("run: WiFi lost, configTimeout. Connect WiFi. Retry#:"), retryTimes);
                }
                else
                {
                    resetFunc();
                }
            }
#endif

            // Not in config mode, try reconnecting before forcing to config mode
            if (WiFi.status() != WL_CONNECTED)
            {
#if (WIFI_RECON_INTERVAL > 0)

                static uint32_t lastMillis = 0;

                if ((lastMillis == 0) || (curMillis - lastMillis) > WIFI_RECON_INTERVAL)
                {
                    lastMillis = curMillis;

                    DB_PRINTLN(F("r:WLost.ReconW"));

                    if (connectMultiWiFi() == WL_CONNECTED)
                    {
                        // turn the LED_BUILTIN OFF to tell us we exit configuration mode.
                        // //digitalWrite(LED_BUILTIN, LED_OFF);

                        DB_PRINTLN(F("run: WiFi reconnected"));
                    }
                }
#else
                DB_PRINTLN(F("run: WiFi lost. Reconnect WiFi"));

                wifiManager->autoConnect("Kaleidoscope");
                if (WiFi.status() == WL_CONNECTED)
                {
                    // turn the LED_BUILTIN OFF to tell us we exit configuration mode.
                    // //digitalWrite(LED_BUILTIN, LED_OFF);

                    DB_PRINTLN(F("run: WiFi reconnected"));
                }
#endif
            }
        }
    }
    else if (configuration_mode)
    {
        configuration_mode = false;
        DB_PRINTLN(F("run: got WiFi back"));
        // turn the LED_BUILTIN OFF to tell us we exit configuration mode.
        // //digitalWrite(LED_BUILTIN, LED_OFF);
    }
}
#endif
#endif // WIFI
