#ifndef LED_STRIPS_H
#define LED_STRIPS_H

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// define a  class for the LED strips
// I tried to make this a proper singleton class but apparently the Arduino tools don't support that
//  LED_strips.h:46: undefined reference to `LEDStrips::instance'
class LEDStrips
{
public:
    CRGB strip[LED_STRIPS][LEDS_PER_STRIP];

    // initialize all LED strips
    void setup()
    {
        DB_PRINTLN(F("LEDStrips.setup"));

        FastLED.addLeds<WS2812B, LED_STRIPS_PIN_BASE + 0, GRB>(strip[0], LEDS_PER_STRIP);
        // FIX THIS (when we have more space)!
        /*FastLED.addLeds<WS2812B, LED_STRIPS_PIN_BASE + 1, GRB>(strip[1], LEDS_PER_STRIP);
        FastLED.addLeds<WS2812B, LED_STRIPS_PIN_BASE + 2, GRB>(strip[2], LEDS_PER_STRIP);
        FastLED.addLeds<WS2812B, LED_STRIPS_PIN_BASE + 3, GRB>(strip[3], LEDS_PER_STRIP);*/
    };

    // automatically adjust the brightness of the LED strips to match the ambient lighting
    void adjustBrightness()
    {
        // store the current LED brightness so we can minimize minor differences
        static int LEDbrightness = 0;

        // check the photocell and map 0-1023 to 0-255 since that is the range for setBrightness
        int photocellReading = analogRead(PHOTOCELL_PIN);
        int newBrightness = map(photocellReading, 0, 1023, 0, 255);

        // adjust our brightness if it has changed significantly
        if ((newBrightness > LEDbrightness + 5) || (newBrightness < LEDbrightness - 5))
        {
            DB_PRINT(F("Analog photocell reading = "));
            DB_PRINTLN(photocellReading); // the raw analog reading
            DB_PRINT(F("new brightness = "));
            DB_PRINTLN(newBrightness);

            LEDbrightness = newBrightness;
            FastLED.setBrightness(LEDbrightness);
        }
    }
};

extern LEDStrips leds;

#endif // LED_STRIPS_H
