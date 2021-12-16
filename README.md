# Kaleidoscope
A Digital Kaleidoscope maker project.

## Configure WiFi settings

When the Kaleidoscope first boots up, it will attempt to connect to WiFi using the current settings.
If that fails, it will will go into "Access Point mode" which will allow you to access it via WiFi using your laptop or phone.

Connect to the Kaleidoscope WiFi access point with your device and follow the onscreen instructions to configure the WiFi settings.
The Kaleidoscope will then reboot and connect using the updated settings.

Any time the Kaleidoscope cannot connect with the current WiFi settings, it will go into AP mode so that the settings can be updated.
If you are still having problems, reboot the Kaleidoscope (unplug/plug) twice a few seconds apart (less than 10) and the settings will be cleared and it will go into AP mode.

## Over-the-air updates

**Important note**: Over-the-air updates can only be done when the Kaleidoscope is powered on and connected to the internet but in 'off' mode.

1. Build your new image
2. On the Kaleidoscope (or via the app) select the mode 'off.'
3. In a web browser, go to the URL http://kaleidoscope/update
4. Login with the correct credentials (default is admin/admin)
5. Ensure the "Firmware" button is selected and click the "Choose File" button
6. Find the firmware you just built (the default filename is .pio/build/node32s/firmware.bin) and choose "Open"
7. The firmware will be uploaded to the Kaleidoscope and then it will reboot

## REST API documentation

The REST API is available at: http://kaleidoscope/api/settings. Alternately, check your router for the IP address.

GET will return a result that includes the following entries:

```
{
    "brightness": 4095,
    "speed": 25,
    "mode": "Twinkle Fox",
    "clockFace": "None",
    "drawStyle": "Six way",
    "handsColor": 16646134
}
```

PUT will allow you to set some or all of the same values.

### Brightness

TODO, switch this to be 0-255 which is the actual available range that it gets mapped to.

Control how brightly the LEDs are lit. The valid range is -305 (dim) to 4095 (bright).

### Speed

The Kaleidoscope speed is a value between 0 (slow) to 255 (fast).
Not all modes can honor all speeds; each one is responsible for making a 'best effort.'

### Mode

All the available modes for the Kaleidoscope. This can vary as more are added or removed but currently it includes:

```
    {
        "off",
        "Kaleidoscope",
        "Plasma",
        "Ripples",
        "Twinkle Fox",
        "AA Lines",
        "Distortion Waves",
        "Rainbow",
        "Matrix",
        "Pacifica",
        "Snake",
        "Fire",
```        

### clockFace

TODO: change clockFace from 'None' to 'Off' for consistency with the Mode

Choose what clock face (if any) is displayed.

```
    {
        "None",
        "Digital",
        "Analog"
    }
```

### drawStyle

Controls the type of reflection style the Kaleidoscope emulates. Currently this includes:

```
    {
        "Six way",
        "Twelve way",
        "Twenty four way"
    }
```        

#### handsColor

TODO: rename this value to "clockColor" as it is used by both the digital and analog clock

Specified the color used to draw the clock. It is a 3 byte RGB value stored as:

```
    handsColor.r << 16 | handsColor.g << 8 | handsColor.b;
```

