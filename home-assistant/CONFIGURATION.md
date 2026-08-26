# Home Assistant helper package configuration

The ESPHome dashboard uses helper sensors created by the Home Assistant packages in this directory:

- `display_weather.yaml`
- `home_assistant_display.yaml`
- `dashboard_update.yaml` (optional but recommended update checker from v1.2.0)

The weather and energy helper sensors are named `sensor.display_*` and are read by the ESP32 dashboard. You normally **do not change those generated helper entity IDs**.

What must be adapted are the **source entities** used by the weather and energy packages. The values below are the entities from the original installation and must be replaced when your Home Assistant uses different entity IDs.

## 1. Weather package

File: `display_weather.yaml`

Replace every occurrence of the following source entities:

| Original source entity | Replace with |
| --- | --- |
| `weather.marco_e_licia_home` | Your Home Assistant weather entity that supports daily forecasts |
| `sensor.sensore_pioggia_aqara_2` | Your rain/precipitation sensor used for Recorder statistics |

The package calls `weather.get_forecasts` for the daily forecast and `recorder.get_statistics` for the precipitation history.

The generated helpers include the 7 forecast sensors and the precipitation-history sensors used by the display. Their `sensor.display_*` names should remain unchanged unless you also modify the matching ESPHome configuration.

## 2. Energy and EV-history package

File: `home_assistant_display.yaml`

Replace every occurrence of these source entities:

| Original source entity | Meaning |
| --- | --- |
| `sensor.inverter_rendimento_totale` | Cumulative photovoltaic energy production |
| `sensor.power_meter_consumo` | Cumulative energy imported from the grid |
| `sensor.power_meter_esportata` | Cumulative energy exported to the grid |
| `sensor.battery_carica_giornaliera` | Battery charging energy used by the house-energy calculation |
| `sensor.battery_scarica_giornaliera` | Battery discharging energy used by the house-energy calculation |
| `sensor.wallbox_energy_active_import_register_2` | Cumulative wallbox energy register used for the weekly EV graph |

The house-consumption helper is calculated from the source statistics as:

```text
PV production
+ grid import
+ battery discharge
- grid export
- battery charge
```

The package expects the cumulative energy sources used with `recorder.get_statistics` to provide suitable long-term statistics in kWh.

## 3. Automatic update checker

File: `dashboard_update.yaml`

Starting with v1.2.0, this optional package checks the latest **stable GitHub release** every 6 hours.

The installed dashboard version is reported by the ESPHome display through a text sensor named:

```text
Waveshare Dashboard Version
```

Home Assistant may prefix its entity ID with the device name and/or area, for example:

```text
sensor.salotto_waveshare_display_waveshare_dashboard_version
```

For this reason, `dashboard_update.yaml` does **not** require a fixed entity ID. It automatically finds the single sensor whose entity ID ends with:

```text
waveshare_dashboard_version
```

and exposes a stable helper:

```text
sensor.waveshare_dashboard_installed_version
```

The update package therefore creates/uses:

```text
sensor.waveshare_dashboard_installed_version
sensor.waveshare_dashboard_latest_version
binary_sensor.waveshare_dashboard_update_available
```

The installed-version helper includes the detected ESPHome source entity in its `source_entity` attribute.

The automatic discovery expects exactly one Waveshare dashboard version sensor. If more than one dashboard is installed in the same Home Assistant instance, the installed-version helper intentionally becomes unavailable instead of selecting the wrong device. Multi-display installations can customize the package to target a specific version sensor.

When a newer stable release is found, Home Assistant creates a persistent notification containing the installed version, latest version and a link to the GitHub release.

The checker does **not** install firmware automatically. Updating remains manual so a working dashboard cannot be changed without the user's approval.

From v1.2.0 the installed version has a single source of truth: `dashboard_version` in the local ESPHome `user_config.yaml`. The main `waveshare-display.yaml` uses that value both as the GitHub package `ref` and as the version exposed to Home Assistant.

Example:

```yaml
dashboard_version: "v1.2.0"
```

To update to a future release, change only that value to the new release tag and reinstall the display from ESPHome. The reported installed version then updates automatically after the device reconnects.

The GitHub check uses the public releases API and does not require a GitHub token. Its default 6-hour interval is deliberately conservative.

**Important:** after adding `dashboard_update.yaml` for the first time, perform a full Home Assistant restart. Reloading YAML alone may not create the new REST sensor.

## 4. ESPHome entity configuration is separate

The entities used directly by the ESP32 are configured in:

```text
esphome/user_config.yaml
```

Create that file by copying:

```text
esphome/user_config.example.yaml
```

Do not add the generated `sensor.display_*` helper sensors to `user_config.yaml`; they are intentionally fixed because the Home Assistant packages create them for the dashboard.

Do not rename `dashboard_version`. It controls both the remote package release and the version reported by the ESPHome device.

## 5. Installing the packages

Use the package-loading method already used by your Home Assistant installation. A common setup is:

```yaml
homeassistant:
  packages: !include_dir_named packages
```

and the package YAML files are placed in the configured `packages` directory.

Recommended files from v1.2.0:

```text
display_weather.yaml
home_assistant_display.yaml
dashboard_update.yaml
```

After adding or modifying the packages:

1. Check the Home Assistant configuration.
2. Perform a full Home Assistant restart when adding `dashboard_update.yaml` for the first time.
3. Confirm that the expected `sensor.display_*` helper entities are created and have valid values.
4. Confirm `sensor.waveshare_dashboard_latest_version` is available.
5. Confirm the Waveshare device page shows `Waveshare Dashboard Version`.
6. Confirm `sensor.waveshare_dashboard_installed_version` reports the same version and its `source_entity` attribute points to the ESPHome version sensor.
7. Confirm `binary_sensor.waveshare_dashboard_update_available` has a valid state.
8. Only then consider the update-checker installation complete.

## Important

The example source IDs in the weather and energy files are from the original dashboard installation. They are not universal Home Assistant entity names. Keep the generated helper names stable and change only the source entity IDs unless you intentionally modify both the Home Assistant and ESPHome sides.
