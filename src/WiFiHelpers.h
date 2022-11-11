#ifndef WIFIHELPERS_H
#define WIFIHELPERS_H

class ESPAsync_WiFiManager;
extern ESPAsync_WiFiManager *wifiManager;

class AsyncWebServer;
extern AsyncWebServer *webServer;

extern void wifi_setup(const char *iHostname = "");
extern void wifi_loop(void);

#endif // WIFIHELPERS_H
