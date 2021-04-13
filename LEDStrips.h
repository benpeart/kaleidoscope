#ifndef LED_STRIPS_H
#define LED_STRIPS_H
#include <Adafruit_NeoPixel.h>

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
    Adafruit_NeoPixel strip[LED_STRIPS];

    // initialize all LED strips
    void setup()
    {
        DB_PRINTLN("LEDStrips.setup");

        for (int x = 0; x < LED_STRIPS; x++)
        {
            // Parameter 1 = number of pixels in strip
            // Parameter 2 = Arduino pin number (most are valid)
            // Parameter 3 = pixel type flags, add together as needed:
            //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
            //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
            //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
            //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
            //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
            strip[x] = Adafruit_NeoPixel(PIXELS_PER_STRIP, LED_STRIPS_PIN_BASE + x, NEO_GRB + NEO_KHZ800);
            strip[x].begin();
            strip[x].clear();
        }
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
            DB_PRINT("Analog photocell reading = ");
            DB_PRINTLN(photocellReading); // the raw analog reading
            DB_PRINT("new brightness = ");
            DB_PRINTLN(newBrightness);

            LEDbrightness = newBrightness;
            for (int x = 0; x < LED_STRIPS; x++)
            {
                strip[x].setBrightness(LEDbrightness);
            }
        }
    }
};

extern LEDStrips leds;

#endif // LED_STRIPS_H
