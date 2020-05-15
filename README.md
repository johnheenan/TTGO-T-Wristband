# Modifications in this branch by John Heenan

### Hardware

This code is for the T-Wristband from LilyGo at https://www.aliexpress.com/item/4000527495064.html with the previous hardware version with MPU9250 IMU.

The code will be updated for the current T-Wristband on sale with LSM9DS1 IMU.

### Uploading Firmware File

This OTA update method has been tested with the T-Wristband with firmware from source at https://github.com/TioRuben/TTGO-T-Wristband and https://github.com/johnheenan/TTGO-T-Wristband.

It will be tested with firmware form other sources such as https://github.com/Xinyuan-LilyGO/T-Wristband and with original shipped firmware.

If you have got a working version of TioRuben's code on your T-Wristband then you can OTA the firmware file for this source code, firmware.bin, from the release section of this github repo, tagged as v0.1c-alpha.

It can be OTA uploaded to the T-Wristband with: 

```
espota.py --auth=wristbandpass --progress -i <ip address> -f firmware.bin
```
For example if the screen shows IP address  192.168.1.177 then 
```
espota.py --auth=wristbandpass --progress -i 192.168.1.177 -f firmware.bin
```

### Screen changes

 Screen   | Skipping in mod | Long Press Original | Long Press Modifed 
---------------|-----------------|------------------|--------------------
 Orange Local Time | No | Update Time with NTP | Update Time with NTP
 Green UTC Time | No | Update Time with NTP | Set DST Region or UTC Offset
 Battery Level|No|None|Toggle IMU on or off for power saving|
 Orange Compass Degrees|Yes in mod if IMU off|Calibrate Compass|Calibrate Compass
 Red Temperature|Yes in mod if IMU off|None| None
 OTA (upgrade firmware)|No|Update firmware|Update firmware
 Deep Sleep | Yes in mod if plugged in | N/A| N/A

In the original there are two types of key presses

Brief press changes screen and a key press of one second chooses an action

So IMU can be toggled on or off by holding down the the button on the battery screen

## What toggling IMU on or off does

When IMU is toggled on you can view two extra screens (Compass Degrees and Temperature). You can also use movement gestures to wake the watch up from deep sleep.

Currently when the IMU is toggled off battery drain is reduced substantially, increasing the time required between battery recharges. This is for two reasons: battery current in deep sleep is reduced with IMU off and the chance of accidental wake ups is reduced. When woken up the current used is dramatically increased compared to when in deep sleep, so accidental wake ups are a battery drain issue.

When IMU is off you can use the button to wake up.

There is relevant information at https://github.com/TioRuben/TTGO-T-Wristband/issues/5

In May 2020 a new hardware version of the T-Wristband went on sale with a different IMU and without a pullup resistor on an interrupt line. The pullup resistor on the previous version drains current unnecessarily in deep sleep due to keeping the interrupt line low during deep sleep. 

With the hardware changes mentioned above and software changes in the future, toggling the IMU off may not be as big an issue.

## Key Press Behaviour Changes For UTC and DST Menus

The behaviour of key presses change changes in the UTC action to change DST Regions or UTC Offsets

1. A key press advances either the DST Regions or the UTC Offset screen. 
2. A key press of one second choose an option and backs out
3. No key press for five seconds backs out and chooses nothing

### UTC Offsets Menu

There are 49 choices: 48 for each half hour increment in day and a choice to move to DST Regions menu instead. If you choose DST regions then go back in to view after being backed out.

### DST Regions Menu

The sample firmware had four choices and one other to move to UTC Offsets menu

If you choose UTC Offsets then go back to view after being backed out.

The four choices are 
* Europe, Central
* China
* Brisbane
* Sydney

A region does not have to be a DST region to put an entry in (such as China and Brisbane)

You can code up to 100 DST regions in the timezones.cpp file and give each whatever name you want, up to 20 characters (but only the middle 12 or so charcters will be displayed). Each entry only requires one line. An up to 10 character zone code, such as "CST" is shown in the local time screen. If preferred you can use a 10 character code code such as as "+8hr" or "+10:30". You can also leave it blank as in "". 

## NVS (as EEPROM) Settings preserved

If the settings structure has the same size as before then settings are preserved across firmware updates, unless the EEPROM_VER value in platformio.ini is changed

## Additional Documentation and TODOs

There are strings in the code that partially serve as documentation. Just search or grep through code for "doco" and "todo jh", such as:

* grep -r doco *
* grep -r "todo jh" *
  

## Miscellaneous Notes

platformio.ini documents various build time choices.

The WiFi Manager screen that appears if a WiFi connection fails shows name of AP to connect to (T-Wristband) and when connected what IP to use in a web browser. The screen now times out after two minutes

Internally, since the NTP library code tries many times to get an answer, the code only makes one attempt to get time the time through the library. If there is a failure, just try again later.

The NTP library code was modified minimally.

A firmware ID screen such as "200515,jh,branch,tr" appears in the IMU on/off screen. 200513 means 13 May 2020. "jh,branch,tr" means branch of John Heenan from TioRuben.

# From the version cloned from:

# WIP: TTGO T-Wristband Example

First steps with TTGO T-Wristband. [Product page](https://es.aliexpress.com/item/4000527495064.html).

Developed with PlatformIO

Includes a paged interface framework.

**Disclaimer:** Still work in progress. Currently I'm facing issues regarding battery duration (You can follow the issue in the [manufacturer's repo](https://github.com/Xinyuan-LilyGO/LilyGO-T-Wristband/issues/2)). PRs are welcome.

## User interface

- Single touch: change page
- Long touch: page action
- Led: Charging status

## Currently available pages

- CET/CEST time, action: connect to wifi and update via NTP
- UTC time, action: connect to wifi and update via NTP
- Battery status. Action: OTA mode.
- Compass, action: calibrate magnetometer.
- Temperature: from the MPU9250 temp register.
- OTA: long touch to enter OTA mode, to load firmware from PlatformIO.

## Build and Flashing

First of all, install PlatformIO with your favourite IDE (i.e. VSCode). Follow [this](https://platformio.org/platformio-ide) instructions.

Clone the repo

```sh
git clone https://github.com/TioRuben/TTGO-T-Wristband.git
```

Open cloned folder with your PlatformIO IDE (in the example, with VSCode)

```shell
cd TTGO-T-Wristband
code .
```

In the first flash, edit `platformio.ini` and comment all upload config (upload_protocol, upload_port, upload_flags):

Connect wristband via USB with the supplied daughter board. In Windows 10, drivers are installed automatically. I guess with other OS will be automatically installed too.

After plugging wristband, please select `env` variant **esp32dev** on VisualCode and build it, `PlatformIO icon->env:esp32dev->Build`:

![Upload Button](https://raw.githubusercontent.com/hpsaturn/TTGO-T-Wristband/av/pio_config_envs/resources/vcode_env_usb_build.jpg)

then, in the same menu, plase select `upload`.

or build and upload it with the `pio` command line:

```bash
pio run -e esp32dev --target upload
```

PlatformIO will build and upload the binaries to the TTGO T-Wristband via USB.

## OTA Upload

After first flashing, you can use OTA upload. First of all, go to battery or OTA page in the wristband. Then maintain the wristband button pressed. If your Wifi is still not configured, you will see an screen asking you to connect to the wristband AP. Connect to It and follow the instructions in the captive portal to configure wifi (more info: [WifiManager](https://github.com/tzapu/WiFiManager#development)).

Once configured, you will see the "waiting for ota" message in the screen. Find your wristband IP Address, in your `platformio.ini` please enter the IP address in the `upload_port` config:

```config
[...]
[env:esp32devOTA]
platform = espressif32
board = esp32dev
framework = ${common_env_data.framework}
monitor_speed = ${common_env_data.monitor_speed}
upload_protocol = espota
upload_port = [YOUR_IP_ADDRESS]
[...]
```

It's supposed to work with hostnames, but sometimes fails.

Press the upload button in PlatformIO

![Upload Button](https://docs.platformio.org/en/latest/_images/platformio-ide-vscode-build-project.png)

or build and upload it with the `pio` command line:

```bash
pio run --target upload
```

The project will compile and upload to the wristband via WiFi (OTA).

## Follow repo to get more updates

## ToDo list

- [x] UTC Time
- [x] Port ADC calibration from Lilygo example
- [x] Finish compass
- [x] Add magnetometer calibration action
- [x] Add MPU temperature measurement page
- [x] Wake on motion interrupt. Wakes the wristband if big movements (ie when moving the arm to watch the time).
- [ ] Refactor page framework to work with RTOS tasks
- [ ] Charging animation
- [ ] Prevent deep sleep when charging to feedback the charging status (via LED)
- [ ] Bluetooth
- [ ] "Next appointment" page
- [ ] Change lcd brightness depending on day/night time
