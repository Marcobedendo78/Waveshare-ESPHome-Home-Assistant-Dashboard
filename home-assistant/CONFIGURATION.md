# Home Assistant helper package configuration

The ESPHome dashboard uses helper sensors created by the two Home Assistant packages in this directory:

- `display_weather.yaml`
- `home_assistant_display.yaml`

These helper sensors are named `sensor.display_*` and are read by the ESP32 dashboard. You normally **do not change those generated helper entity IDs**.

What must be adapted are the **source entities** used by the packages. The values below are the entities from the original installation and must be replaced when your Home Assistant uses different entity IDs.

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

## 3. ESPHome entity configuration is separate

The entities used directly by the ESP32 are configured in:

```text
esphome/user_config.yaml
```

Create that file by copying:

```text
esphome/user_config.example.yaml
```

Do not add the generated `sensor.display_*` helper sensors to `user_config.yaml`; they are intentionally fixed because the Home Assistant packages create them for the dashboard.

## 4. Installing the packages

Use the package-loading method already used by your Home Assistant installation. A common setup is to enable packages in `configuration.yaml` and place these YAML files under the configured packages directory.

After adding or modifying the packages:

1. Check the Home Assistant configuration.
2. Restart Home Assistant.
3. Confirm that the expected `sensor.display_*` helper entities are created and have valid values.
4. Only then validate/compile the ESPHome display configuration.

## Important

The example source IDs in these files are from the original dashboard installation. They are not universal Home Assistant entity names. Keep the generated helper names stable and change only the source entity IDs unless you intentionally modify both the Home Assistant and ESPHome sides.
