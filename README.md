# Waveshare ESPHome Home Assistant Dashboard

Touchscreen wall dashboard for **Home Assistant** based on the **Waveshare ESP32-S3 Touch LCD 7B (1024×600)** and **ESPHome**.

**Current version: v1.0.0**

![Installed dashboard](docs/images/dashboard-home.jpg)

## Features

- Home energy distribution with animated power-flow visualization
- Solar production and home-consumption charts
- Heat-pump monitoring and control
- Lighting control
- Room heating controls
- Weather page
- EV charging dashboard
- Robot lawnmower dashboard and controls
- Energy monitoring
- Capacitive touchscreen navigation
- microSD logging support
- Native Home Assistant integration through ESPHome

## Hardware

The project is designed for the **Waveshare ESP32-S3 Touch LCD 7B**, 7-inch 1024×600 capacitive touchscreen.

A custom 3D-printed enclosure and wall mounting system is available on MakerWorld:

**MakerWorld:**  
https://makerworld.com/it/models/3204446-cover-display-waveshare-7-pollici#profileId-3626631

The enclosure provides:
- wall mounting on a standard 503 box;
- integration with a Vimar Plana USB power module;
- hidden USB cable routing;
- external access to RESET and BOOT;
- ventilation openings.

## Dashboard examples

### Home
![Home](docs/images/dashboard-home.jpg)

### Lights
![Lights](docs/images/dashboard-lights.jpg)

### Robot lawnmower
![Robot](docs/images/dashboard-robot.jpg)

### EV charging
![EV charging](docs/images/dashboard-ev-charging.jpg)

## Repository structure

```text
esphome/
  waveshare-dashboard.yaml
  sd_storage.h
  Robot.jpg
  capture_card.png

home-assistant/
  display_weather.yaml
  home_assistant_display.yaml

docs/
  images/
```

The images `Robot.jpg` and `capture_card.png` are kept in the same `esphome/` directory as the main YAML because the v1.0.0 configuration references them as local files.

## ESPHome secrets

The project does **not** contain Wi-Fi passwords or API encryption keys in clear text.

The current YAML expects these ESPHome secrets:

```yaml
wifi_ssid: "YOUR_WIFI_SSID"
wifi_password: "YOUR_WIFI_PASSWORD"

waveshare_display__encryption_key: "YOUR_ESPHOME_API_ENCRYPTION_KEY"
waveshare_display__ap_password: "YOUR_FALLBACK_AP_PASSWORD"
```

> Note: the double underscore in `waveshare_display__encryption_key` and `waveshare_display__ap_password` matches the current working ESPHome configuration and is intentionally preserved in v1.0.0.

## Home Assistant permission

In Home Assistant open:

**Settings → Devices & services → ESPHome → waveshare-display → Configure**

and enable:

**Allow the device to perform Home Assistant actions**

This is required for dashboard controls that call Home Assistant actions, including the MQTT commands used by the robot lawnmower controls.

## Home Assistant helper packages

The repository includes:

- `home-assistant/display_weather.yaml`
- `home-assistant/home_assistant_display.yaml`

These create helper sensors used by the display, including weather and energy/charging history data.

Install them according to your Home Assistant package structure. Entity IDs in the current v1.0.0 dashboard are specific to the original installation and may need to be adapted.

## ESPHome installation

For v1.0.0, keep these four files together in your ESPHome configuration directory:

```text
waveshare-dashboard.yaml
sd_storage.h
Robot.jpg
capture_card.png
```

The YAML uses ESPHome `!secret` references, so configure the required secrets before validating or compiling.

## microSD

`sd_storage.h` mounts the onboard microSD and can write:

```text
/sdcard/waveshare/display.log
/sdcard/waveshare/energy_history.csv
```

The energy history CSV includes:

```text
timestamp,solar_w,grid_w,battery_w,home_w,pdc_w,battery_soc_pct
```

## 3D printed enclosure

![Wall support](docs/images/wall-support-vimar.jpg)

![Enclosure](docs/images/enclosure-inside-1.jpg)

![RESET and BOOT](docs/images/reset-boot-buttons.jpg)

![Board mounted](docs/images/board-mounted.jpg)

## Versioning

This repository starts with **v1.0.0**.

- `v1.0.x` — bug fixes
- `v1.x.0` — new backward-compatible features
- `v2.0.0` — major/incompatible changes

## Related project

Robot Arduino 4.0:  
https://github.com/Marcobedendo78/Robot-Arduino-4.0

## Author

Marco Bedendo — `Marcobedendo78`
