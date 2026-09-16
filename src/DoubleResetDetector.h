#pragma once

#include <Arduino.h>
#include <Preferences.h>

class DoubleResetDetector
{
public:
    DoubleResetDetector(Preferences *prefs, uint32_t timeoutSeconds = 10, const char *ns = "drd")
        : _prefs(prefs),
          _timeoutMs(timeoutSeconds * 1000UL),
          _namespace(ns),
          _doubleResetDetected(false),
          _waitingForTimeout(false),
          _startTime(0)
    {
    }

    // Call early in setup() before peripheral/network initialization
    void setup()
    {
        if (!_prefs)
        {
            DB_PRINTLN(F("[DRD] Error: Preferences pointer is NULL!"));
            return;
        }

        // Check if the flag was left set from an immediate prior reboot/power-cut
        bool flagSet = _prefs->getBool(FLAG_KEY, false);

        if (flagSet)
        {
            _doubleResetDetected = true;
            DB_PRINTLN(F("[DRD] Double reset / power cycle detected!"));

            // Clear flag immediately so third reboot won't trigger DRD
            if (_prefs->putBool(FLAG_KEY, false) == 0)
            {
                DB_PRINTLN(F("[DRD] Error: Failed to clear flag in NVS after detection!"));
            }
            _waitingForTimeout = false;
        }
        else
        {
            _doubleResetDetected = false;
            DB_PRINTF("[DRD] Normal boot. Arming flag for %u ms window...\r\n", _timeoutMs);
            // Arm flag in NVS: if power is cut or reset occurs before timeout, this remains true
            if (_prefs->putBool(FLAG_KEY, true) == 0)
            {
                DB_PRINTLN(F("[DRD] Error: Failed to write arming flag to NVS!"));
            }
            _waitingForTimeout = true;
            _startTime = millis();
        }
    }

    // Call continuously in loop() to automatically disarm the detector once timeout passes
    void loop()
    {
        if (!_waitingForTimeout || !_prefs)
            return;

        if (millis() - _startTime >= _timeoutMs)
        {
            if (_prefs->putBool(FLAG_KEY, false) == 0)
            {
                DB_PRINTLN(F("[DRD] Error: Failed to write disarmed state to NVS!"));
            }
            else
            {
                DB_PRINTLN(F("[DRD] Timeout expired. Flag successfully disarmed."));
            }
            _waitingForTimeout = false;
        }
    }

    // Returns true if a double reset/power-cut cycle was detected on this boot
    bool isDoubleReset() const
    {
        return _doubleResetDetected;
    }

    // Manually disarm the detector immediately (e.g. user aborts or enters safe mode early)
    void clear()
    {
        if (!_prefs)
        {
            DB_PRINTLN(F("[DRD] Error: Preferences pointer is NULL in clear()!"));
            return;
        }

        if (_prefs->putBool(FLAG_KEY, false) == 0)
        {
            DB_PRINTLN(F("[DRD] Error: Failed to clear flag in clear()!"));
        }
        else
        {
            DB_PRINTLN(F("[DRD] Flag manually cleared."));
        }

        _waitingForTimeout = false;
    }

private:
    static constexpr const char *FLAG_KEY = "drd_flag";

    Preferences *_prefs;
    const uint32_t _timeoutMs;
    const char *_namespace;
    bool _doubleResetDetected;
    bool _waitingForTimeout;
    unsigned long _startTime;
};