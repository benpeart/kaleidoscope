#pragma once
#include <Arduino.h>

enum puType
{
    UP,
    DOWN,
    NONE
};

class ESP32Encoder
{
private:
    boolean attached;
    uint8_t aPinNumber;
    uint8_t bPinNumber;
    int64_t count;
    static void IRAM_ATTR read_encoder(void *arg);
    uint8_t old_AB; // Lookup table index
    int8_t encval;  // Encoder value

public:
    ESP32Encoder();
    ~ESP32Encoder();
    void attachSingleEdge(uint8_t aPintNumber, uint8_t bPinNumber);
    int64_t getCount();
    int64_t clearCount();
    void setCount(int64_t value);
    void setFilter(uint16_t value){};
    static enum puType useInternalWeakPullResistors;
};
