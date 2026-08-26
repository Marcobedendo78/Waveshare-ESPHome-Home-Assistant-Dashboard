# Changelog

All notable changes to this project will be documented in this file.

## [1.2.0] - Unreleased

### Update notifications

- Added `home-assistant/dashboard_update.yaml` as an optional but recommended Home Assistant package.
- Added a REST sensor that checks the latest stable GitHub release every 6 hours.
- Added `binary_sensor.waveshare_dashboard_update_available` for update status.
- Added a Home Assistant persistent notification when a newer stable dashboard release is available.
- The notification contains the installed version, latest version and a direct link to the GitHub release.
- Update installation remains manual by design; the checker never installs firmware automatically.
- Added automatic dismissal of the persistent notification when the installed version is current.
- Added startup and periodic safety checks so update notifications are also created correctly after a Home Assistant restart.

### ESPHome version reporting

- Added `dashboard_version` to `user_config.yaml`.
- The ESPHome package `ref` now uses `${dashboard_version}`, so the release reference and reported dashboard version cannot drift apart.
- Added the ESPHome `Waveshare Dashboard Version` text sensor, visible directly on the Waveshare device page in Home Assistant.
- Added automatic discovery of the ESPHome version entity by the stable entity-ID suffix `waveshare_dashboard_version`, so Home Assistant device/area prefixes do not need to be known in advance.
- Added `sensor.waveshare_dashboard_installed_version` as a stable helper that mirrors the detected ESPHome version sensor and exposes the detected source entity as an attribute.
- Automatic discovery requires exactly one matching dashboard version sensor; ambiguous multi-display installations become unavailable instead of selecting the wrong device.
- Updating to a new release therefore requires changing only `dashboard_version` in the local `user_config.yaml`, followed by an ESPHome reinstall.

### Documentation

- Updated the configuration model for v1.2.0 so the dashboard version has a single source of truth.
- Documented automatic version-entity discovery and Home Assistant entity-ID prefixes.
- Documented that a full Home Assistant restart is required after adding the REST update-checker package for the first time.
- Updated the README quick-start, repository structure and update-checker instructions.

### Validation

- Validated the updated ESPHome configuration successfully.
- Installed the updated configuration OTA on the real Waveshare ESP32-S3 Touch LCD 7B.
- Confirmed `Waveshare Dashboard Version` appears on the Home Assistant ESPHome device page.
- Confirmed the GitHub REST sensor reads the latest stable release correctly.
- Confirmed the update comparison changes to `on` when a newer release is simulated.
- Confirmed the persistent Home Assistant update notification is generated with the expected installed version, latest version and release link.

## [1.1.1] - 2026-08-26

### Fixes

- Pinned the remote ESPHome dashboard package to the matching release tag instead of `main`, preventing existing installations from silently picking up future dashboard changes during a later compile.
- Updated `waveshare-display.example.yaml` and the README to document release-tag pinning.
- Added a repository `.gitignore` to reduce the risk of committing local credentials and installation-specific configuration files.
- Removed the obsolete `UPLOAD_INSTRUCTIONS.txt` file that referred to the original v1.0.0 repository upload procedure.

### Upgrade note

Users who installed v1.1.0 should use the v1.1.1 `waveshare-display.example.yaml` or change the package reference in their local `waveshare-display.yaml` to:

```yaml
ref: v1.1.1
```

## [1.1.0] - 2026-08-26

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
- Retained `waveshare-dashboard.yaml` as a legacy/reference v1.0 configuration.

### Validation

- Validated the new ESPHome configuration with ESPHome 2026.8.1.
- Successfully compiled the complete firmware for the Waveshare ESP32-S3 Touch LCD 7B.
- Successfully installed the firmware OTA on the real display.
- Confirmed the dashboard, touch navigation, Home Assistant entities and existing functions operate as before with the new package architecture.

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

The v1.0.0 ESPHome YAML is intentionally kept functionally aligned with the working installation. Entity IDs are still specific to the original Home Assistant instance. v1.1.0 moves installation-specific values to documented configuration variables.
