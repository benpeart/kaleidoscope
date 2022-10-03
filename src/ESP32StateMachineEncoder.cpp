/*
 * ESP32StateMachineEncoder.cpp
 *
 *  Based on Oleg Mazurov's code for rotary encoder interrupt service routines for AVR micros
 *  here https://chome.nerpa.tech/mcu/reading-rotary-encoder-on-arduino/ and using interrupts 
 *  https://chome.nerpa.tech/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros/
 */
#ifdef STATEMACHINEENCODER
#include <ESP32StateMachineEncoder.h>

enum puType ESP32Encoder::useInternalWeakPullResistors = DOWN;

ESP32Encoder::ESP32Encoder()
{
    attached = false;
    aPinNumber = 0;
    bPinNumber = 0;
    count = 0;
    old_AB = 3; // Lookup table index
    encval = 0; // Encoder value
}

ESP32Encoder::~ESP32Encoder()
{
    detachInterrupt(aPinNumber);
    detachInterrupt(bPinNumber);
};

// Encoder interrupt routine for both pins. Updates counter
// if they are valid and have rotated a full indent
void IRAM_ATTR ESP32Encoder::read_encoder(void *arg)
{
    ESP32Encoder *ptr = (ESP32Encoder *)arg;

    static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; // Lookup table

    ptr->old_AB <<= 2; // Remember previous state

    if (digitalRead(ptr->aPinNumber))
        ptr->old_AB |= 0x02; // Add current state of pin A
    if (digitalRead(ptr->bPinNumber))
        ptr->old_AB |= 0x01; // Add current state of pin B

    ptr->encval += enc_states[(ptr->old_AB & 0x0f)];

    // Update counter if encoder has rotated a full indent, that is at least 4 steps
    if (ptr->encval > 3)
    {                 // Four steps forward
        ptr->count--; // Decrease counter to match behavior of ESP32Encoder
        ptr->encval = 0;
    }
    else if (ptr->encval < -3)
    {                 // Four steps backwards
        ptr->count++; // Increase counter to match behavior of ESP32Encoder
        ptr->encval = 0;
    }
}

void ESP32Encoder::attachSingleEdge(uint8_t aPin, uint8_t bPin)
{
    if (attached)
        return;

    aPinNumber = aPin;
    bPinNumber = bPin;

    // Set encoder pins and attach interrupts
    switch (useInternalWeakPullResistors)
    {
    case UP:
        pinMode(aPinNumber, INPUT_PULLUP);
        pinMode(bPinNumber, INPUT_PULLUP);
        break;

    case DOWN:
        pinMode(aPinNumber, INPUT_PULLDOWN);
        pinMode(bPinNumber, INPUT_PULLDOWN);
        break;

    default:
        pinMode(aPinNumber, INPUT);
        pinMode(bPinNumber, INPUT);
        break;
    }
    attachInterruptArg(aPinNumber, read_encoder, this, CHANGE);
    attachInterruptArg(bPinNumber, read_encoder, this, CHANGE);
    attached = true;
}

int64_t ESP32Encoder::getCount()
{
    noInterrupts();
    int64_t ret = count;
    interrupts();
    return ret;
}

int64_t ESP32Encoder::clearCount()
{
    noInterrupts();
    count = 0;
    interrupts();
    return 0;
}

void ESP32Encoder::setCount(int64_t value)
{
    noInterrupts();
    count = value;
    interrupts();
}
#endif // STATEMACHINEENCODER
