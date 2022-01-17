#include "main.h"
#include "weather.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

weather current_weather;

#define WEATHER_GET_FREQUENCY 10

#define stationID "KVAGOSHE2"
#define units "e"
#define apiKey "60abba20f79348ddabba20f79378dd94"
#define currentConditions "https://api.weather.com/v2/pws/observations/current?stationId=" stationID "&format=json&units=" units "&apiKey=" apiKey

void get_current_conditions(weather &w)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(currentConditions);

        // Send HTTP GET request
        int httpResponseCode = http.GET();

        // if status == 200 OK
        if (200 == httpResponseCode)
        {
            String payload = http.getString();
            DB_PRINTF("HTTP Response code: %d\r\n", httpResponseCode);
            DB_PRINTLN(payload);

            // Parse the JSON input
            DynamicJsonDocument doc(1536);
            DeserializationError err = deserializeJson(doc, payload);

            // Parse succeeded?
            if (!err)
            {
                w.temp = doc["observations"][0]["imperial"]["temp"];
            }
            else
            {
                Serial.print(F("deserializeJson() returned "));
                Serial.println(err.f_str());
            }
        }
        else
        {
            DB_PRINTF("Error code: %d\r\n", httpResponseCode);
        }

        // Free resources
        http.end();
    }
    else
    {
        DB_PRINTLN("WiFi Disconnected");
    }
}

void weather_setup(weather &w)
{
    get_current_conditions(w);
}

void weather_loop(weather &w)
{
    EVERY_N_MINUTES(WEATHER_GET_FREQUENCY)
    {
        get_current_conditions(w);
    }
}

void weather_draw_icon(weather *w)
{
}
