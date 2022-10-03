#include "main.h"
#ifdef WEATHER
#include "weather.h"
#include <AsyncHTTPRequest_Generic.h> // https://github.com/khoih-prog/AsyncHTTPRequest_Generic
#include <ArduinoJson.h>               // https://github.com/bblanchon/ArduinoJson

bool need_weather = false;
weather current_weather;
AsyncHTTPRequest request;

#define WEATHER_GET_FREQUENCY 10 // fetch the updated weather conditions every 10 minutes

// compose the REST query to retrieve the current conditions from my weather station
// https://docs.google.com/document/d/1KGb8bTVYRsNgljnNH67AMhckY8AQT2FVwZ9urj8SWBs/edit#heading=h.cmnb42w2owqe
#define stationID "KVAGOSHE2"
#define units "e"
#define apiKey "60abba20f79348ddabba20f79378dd94"
#define currentConditions "http://api.weather.com/v2/pws/observations/current?stationId=" stationID "&format=json&units=" units "&apiKey=" apiKey

void requestCurrentConditions()
{
    DB_PRINTF("requestCurrentConditions request.readyState = %d\r\n", request.readyState());
    if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone)
    {
        // Only send() if open() returns success
        bool requestOpenResult = request.open("GET", currentConditions);
        if (requestOpenResult)
        {
            //request.setReqHeader("Accept", "*/*");
            //request.setReqHeader("Accept-Encoding", "gzip, deflate, br");
            request.send();
        }
        else
        {
            DB_PRINTF("requestCurrentConditions can't send bad request '%s'\r\n", currentConditions);
        }
    }
    else
    {
        DB_PRINTLN("requestCurrentConditions can't send request");
    }
}

void requestCallBack(void *optParam, AsyncHTTPRequest *request, int readyState)
{
    weather *w = (weather *)optParam;

    DB_PRINTF("requestCallBack readyState = %d\r\n", readyState);
    if (readyState == readyStateDone)
    {
        int httpResponseCode = request->responseHTTPcode();
        DB_PRINTF("HTTP Response code: %d\r\n", httpResponseCode);

        String payload = request->responseText();
        DB_PRINTLN("\n**************************************");
        DB_PRINT(payload);
        DB_PRINTLN("**************************************");
#ifdef DEBUG
        request->setDebug(false);
#endif

        if (200 == httpResponseCode)
        {
            // Parse the JSON input
            DynamicJsonDocument doc(1536);
            DeserializationError err = deserializeJson(doc, payload);
            if (!err)
            {
                w->temp = doc["observations"][0]["imperial"]["temp"];
                DB_PRINTF("Updated temperature is %d\r\n", w->temp);
            }
            else
            {
                DB_PRINTF("deserializeJson() returned \"%s\"", err.c_str());
            }
        }
    }
}

void weather_loop(weather &w)
{
    static bool init = false;

    if (!need_weather)
        return;

    // do a one time init
    if (!init)
    {
        init = true;
#ifdef DEBUG
        request.setDebug(true);
#endif
        request.onReadyStateChange(requestCallBack, &current_weather);
        requestCurrentConditions();
    }

    EVERY_N_MINUTES(WEATHER_GET_FREQUENCY)
    {
        requestCurrentConditions();
    }
}

void weather_draw_icon(weather *w)
{
}

#endif // WEATHER
