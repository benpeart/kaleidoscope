#ifndef WIFIHELPERS_H
#define WIFIHELPERS_H

class AsyncWebServer;
extern AsyncWebServer webServer;

extern float currentLatitude;  // Positive = North, Negative = South
extern float currentLongitude; // Positive = East, Negative = West
extern bool locationAcquired;  // true if we have successfully acquired a location from IP geolocation

void wifi_setup(void);
void wifi_loop(void);

#endif // WIFIHELPERS_H
