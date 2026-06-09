# Kaleidoscope
A Digital Kaleidoscope maker project.

## Developer Workflow

Build the [env:release] image then flash the entire image (nvs, otadata, safeboot, app) with the command:

```
cd %userprofile%\.platformio\packages\tool-esptoolpy
python .\esptool.py write_flash 0x0 \src\kaleidoscope\.pio\build\release\firmware.factory.bin
```

Then connect to the KaleidoscopeAP with your phone and setup your WiFi credentials. Save and reboot and it will connect to your wifi with the name "kaleidoscope.local." Open http://kaleidoscope.local and configure the UI of the Kaleidoscope (mode, clock, reflection, brightness, speed, clock color).

Subsequent updates of the app partition can be updated using platform.io and the [env:release-ota] or [env:debug-ota] environments. You can also click the 'cloud upload' icon in the bottom left of the Kaleidoscope UI, refresh the page to load the safeboot ui, then drag/drop the firmware.bin file you want to flash

## User Workflow

### First Time Setup
1. Device starts without saved WiFi credentials
2. Fails to connect (timeout after 20 seconds)
3. Enters AP mode with SSID: `kaleidoscopeAP`
4. User connects their phone/computer to `kaleidoscopeAP`
5. Captive portal automatically opens (or user goes to `http://192.168.4.1`)
6. Configure WiFi SSID and password
7. Device saves credentials and reconnects
8. Kaleidoscope page becomes accessible at device's IP or `kaleidoscope.local`

### Subsequent Starts
1. Device boots and loads saved WiFi credentials
2. Connects to WiFi automatically
3. Kaleidoscope page immediately available

### OTA Update Process
1. User clicks cloud upload icon on the kaleidoscope page
2. Device reboots with SafeBoot partition active (refresh page to see)
3. Use the safeboot UI page to upload the new firmware
4. Device restarts with new firmware
5. If failure: SafeBoot recovery mechanism allows fallback

## REST API documentation

The Kaleidoscope connects to the WiFi with the device name "kaleidoscope." The web ui and REST API can be found at http://kaleidoscope.local/. 
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

