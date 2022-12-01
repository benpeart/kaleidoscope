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

The Kaleidoscope connects to the WiFi with the device name "kaleidoscope." The web ui and REST API can be found at http://kaleidoscope/. 
Alternately, check your router for the IP address.

There are four REST endpoints that make up the REST API:

1. "http://kaleidoscope/api/settings"
1. "http://kaleidoscope/api/modes"
1. "http://kaleidoscope/api/faces"
1. "http://kaleidoscope/api/drawstyles"

## 'Settings' REST API

A GET sent to the Settings endpoint will return a result that includes the following entries:
PUT will allow you to set some or all of the same values.

```
{
    "mode": "Twinkle Fox",
    "drawStyle": "Six way",
    "brightness": 4095,
    "speed": 25,
    "clockFace": "Off",
    "clockColor": "#FFFFFF"
}
```

### Mode

The mode specifies which of the available Kaleidoscope modes is currently active. This can vary as more are added or removed but currently the list of available modes includes:

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
    "Fire"
}    
```        

### drawStyle

The drawStyle controls the type of reflection style the Kaleidoscope emulates. Currently this includes:

```
{
    "Six way",
    "Twelve way",
    "Twenty four way"
}
```        

### Brightness

Brightness is treated as an adjustment to the automatic brightness as computed using the photoresister to determine
the ambient brightness of the room. Greater than zero makes it brighter than default, less than zero makes it darker than default.
The valid range is -255 (dark) to 255 (bright). The default is 0.

### Speed

The Kaleidoscope speed is a value between 0 (slow) to 255 (fast). The default is 127.
Not all modes can honor all speeds; each one is responsible for making a 'best effort.'

### clockFace

Choose what clock face (if any) is displayed.

```
{
    "Off",
    "Digital",
    "Analog"
}
```

### clockColor

A hexadecimal color is specified with: #RRGGBB, where the RR (red), GG (green) and BB (blue) hexadecimal integers specify the components of the color.

## 'Modes' REST API

A GET sent to the /api/modes endpoint will return an array of the available modes.
PUT will allow you to set some or all of the same values.

A sample result would be the following:

```
["Kaleidoscope","Plasma","Ripples","Twinkle Fox","AA Lines","Distortion Waves","Rainbow","Matrix","Pacifica","Snake","Fire"]
```

## 'Faces' REST API

A GET sent to the /api/faces endpoint will return an array of the available clock faces.
PUT will allow you to set some or all of the same values.

A sample result would be the following:

```
["Off","Digital","Analog"]
```

## 'DrawStyles' REST API

A GET sent to the /api/drawstyles endpoint will return an array of the posible draw styles.
PUT will allow you to set some or all of the same values.

A sample result would be the following:

```
["Six way","Twelve way","Twenty four way"]
```

