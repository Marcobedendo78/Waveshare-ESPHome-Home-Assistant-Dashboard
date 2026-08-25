# Changelog

All notable changes to this project will be documented in this file.

## [1.1.0] - In development

### Configuration and installation

- Added a remote ESPHome dashboard package architecture.
- Added `esphome/waveshare-display.example.yaml` as the recommended v1.1 local entry configuration.
- Added `esphome/user_config.example.yaml` so installation-specific Home Assistant entities can be changed without editing the large dashboard package.
- Added `esphome/secrets.example.yaml` with safe placeholder values for Wi-Fi, ESPHome API encryption and fallback AP credentials.
- Kept `sd_storage.h`, `Robot.jpg` and `capture_card.png` as clearly documented local ESPHome assets.
- Added `home-assistant/CONFIGURATION.md` documenting the source entities required by the Home Assistant helper packages.

### Dashboard configuration

- Moved robot image and EV image file names to user substitutions.
- Moved Shelly path/gate power sensors to user substitutions.
- Made robot MQTT control, timer and joystick topics configurable through `user_config.yaml`.
- Kept generated `sensor.display_*` Home Assistant helper entity names stable for compatibility with the dashboard.

### Documentation

- Reworked the README with a v1.1 quick-start procedure.
- Documented the distinction between the remote reusable package and the required local ESPHome files.
- Documented the Home Assistant action permission required by dashboard controls.
- Retained `waveshare-dashboard.yaml` as a legacy/reference v1.0 configuration while v1.1 is validated.

### Status

v1.1.0 is still under development on `dev-v1.1.0` and should be validated on the real Waveshare display before merging into `main` and publishing a release.

## [1.0.0] - 2026-08-25

### Initial public release

- First public GitHub version of the working dashboard.
- Waveshare ESP32-S3 Touch LCD 7B 1024×600 support.
- Home energy distribution dashboard.
- Solar production and home consumption charts.
- Heat-pump page.
- Lighting page.
- Room heating controls.
- Weather page.
- EV charging dashboard.
- Robot lawnmower dashboard and controls.
- Energy monitoring page.
- Bottom touchscreen navigation.
- microSD logging support.
- Home Assistant helper packages.
- Local image assets for the robot and EV charging pages.
- 3D printed wall-enclosure documentation and MakerWorld link.

### Notes

The v1.0.0 ESPHome YAML is intentionally kept functionally aligned with the working installation. Entity IDs are still specific to the original Home Assistant instance. v1.1.0 progressively moves installation-specific values to documented configuration variables.
