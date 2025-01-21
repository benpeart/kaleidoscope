#ifndef WIFIHELPERS_H
#define WIFIHELPERS_H

class AsyncWebServer;
extern AsyncWebServer webServer;

void wifi_setup(const char *iHostname);
void wifi_loop(void);

#endif // WIFIHELPERS_H
