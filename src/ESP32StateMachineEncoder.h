#pragma once
#include <Arduino.h>

class ESP32StateMachineEncoder
{
private:
    boolean attached;
    uint8_t aPinNumber;
    uint8_t bPinNumber;
    volatile int64_t count;
    static void IRAM_ATTR read_encoder(void *arg);
    uint8_t old_AB; // Lookup table index
    int8_t encval;  // Encoder value

public:
    ESP32StateMachineEncoder();
    ~ESP32StateMachineEncoder();
    void attachSingleEdge(uint8_t aPintNumber, uint8_t bPinNumber, uint8_t mode);
    int64_t getCount() { return count; }
    int64_t clearCount() { return count = 0; }
    void setCount(int64_t value) { count = value; }
};
