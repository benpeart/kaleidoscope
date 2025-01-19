#include "settings.h"
#include "debug.h"
#include "modes.h"
#include "render.h"
#include "RealTimeClock.h"

// -- EEPROM
Preferences preferences;
#define PREF_VERSION 1 // if setting structure has been changed, increment this number to reset all settings
#define PREF_NAMESPACE "pref"
#define PREF_KEY_VERSION "ver"

// A copy of what is stored in EEPROM so we can compare and only write out changes
kaleidoscope_settings EEPROMSettings;

// global settings anyone can read/write
kaleidoscope_settings settings;

// load persisted settings
void settingsSetup()
{
    // Init EEPROM, if not done before
    preferences.begin(PREF_NAMESPACE, false); // false = RW-mode
    if (preferences.getUInt(PREF_KEY_VERSION, 0) != PREF_VERSION)
    {
        preferences.clear(); // Remove all preferences under the opened namespace
        preferences.putUInt(PREF_KEY_VERSION, PREF_VERSION);
        DB_PRINTF("EEPROM init complete, all preferences deleted, new pref_version: %d\n", PREF_VERSION);
    }

    // load our settings from persistent storage
    EEPROMSettings.brightness = preferences.getInt("brightness", MAX_BRIGHTNESS);
    EEPROMSettings.speed = preferences.getInt("speed", MIN_SPEED);
    EEPROMSettings.mode = preferences.getInt("mode", 0);
    EEPROMSettings.drawStyle = preferences.getInt("drawStyle", 0);
#ifdef TIME
    EEPROMSettings.clockFace = preferences.getInt("clockFace", 0);
    EEPROMSettings.clockColor = CRGB::White;
    preferences.getBytes("clockColor", &settings.clockColor, sizeof(settings.clockColor));
#endif // TIME

    // copy the persisted settings to the current settings
    settings = EEPROMSettings;
};

// persist any changes to the settings
void settingsPersist()
{
    // if any changes have been made and not persisted, write them out now
    if (EEPROMSettings.brightness != settings.brightness)
    {
        DB_PRINTF("settingsPersist: brightness = %d\r\n", settings.brightness);
        preferences.putInt("brightness", settings.brightness);
    }
    if (EEPROMSettings.speed != settings.speed)
    {
        DB_PRINTF("settingsPersist: speed = %d\r\n", settings.speed);
        preferences.putInt("speed", settings.speed);
    }
    if (EEPROMSettings.mode != settings.mode)
    {
        DB_PRINTF("settingsPersist: mode = %s\r\n", KaleidoscopeModeLUT[settings.mode].modeName);
        preferences.putInt("mode", settings.mode);
    }
    if (EEPROMSettings.drawStyle != settings.drawStyle)
    {
        DB_PRINTF("settingsPersist: drawStyle = %s\r\n", drawStylesLUT[settings.drawStyle]);
        preferences.putInt("drawStyle", settings.drawStyle);
    }
#ifdef TIME
    if (EEPROMSettings.clockFace != settings.clockFace)
    {
        DB_PRINTF("settingsPersist: clockFace = %s\r\n", clockFaceLUT[settings.clockFace].faceName);
        preferences.putInt("clockFace", settings.clockFace);
    }
    if (EEPROMSettings.clockColor != settings.clockColor)
    {
        DB_PRINTF("settingsPersist: clockColor = #%06X\r\n", settings.clockColor.r << 16 | settings.clockColor.g << 8 | settings.clockColor.b);
        preferences.putBytes("clockColor", &settings.clockColor, sizeof(settings.clockColor));
    }
#endif // TIME

    // update our cached settings values
    EEPROMSettings = settings;
};
