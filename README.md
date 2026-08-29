# Waveshare ESPHome Home Assistant Dashboard

Dashboard touchscreen da parete per **Home Assistant**, realizzata per **Waveshare ESP32-S3 Touch LCD 7B 7" 1024×600** e gestita tramite **ESPHome**.

La configurazione principale viene scaricata direttamente da questo repository: l'utente deve mantenere localmente solo il file ESPHome del display, il file con le proprie entità e i normali secrets di ESPHome.

![Dashboard installata](docs/images/dashboard-home.jpg)

## Funzioni principali

- distribuzione energia con flussi animati;
- potenza FV, rete, batteria, abitazione, pompa di calore e Wallbox;
- pagina Energia con riepilogo giornaliero e consumi dispositivi;
- monitoraggio pompa di calore;
- controllo luci e cancello;
- gestione riscaldamento ambiente;
- pagina meteo;
- gestione ricarica auto / Wallbox;
- pagina robot tagliaerba con comandi MQTT;
- touchscreen capacitivo;
- supporto microSD integrato;
- integrazione nativa con Home Assistant tramite ESPHome.

## Hardware

Il progetto è sviluppato per:

**Waveshare ESP32-S3 Touch LCD 7B — 7 pollici — 1024×600**

È disponibile anche la cover/supporto da parete stampabile in 3D:

https://makerworld.com/it/models/3204446-cover-display-waveshare-7-pollici#profileId-3626631

## Installazione

### 1. Copiare i due file ESPHome necessari

Dalla cartella `esphome/` del repository utilizzare:

```text
waveshare-display.example.yaml
user_config.example.yaml
```

Nella cartella ESPHome di Home Assistant creare:

```text
waveshare-display.yaml
.user_config.yaml
```

copiando:

```text
waveshare-display.example.yaml  -> waveshare-display.yaml
user_config.example.yaml        -> .user_config.yaml
```

Il punto iniziale di `.user_config.yaml` è intenzionale: evita che ESPHome Device Builder lo interpreti come un secondo dispositivo.

### 2. Non copiare gli altri file del dashboard

Il file locale `waveshare-display.yaml` carica automaticamente da GitHub:

```text
esphome/dashboard-bundle.yaml
```

Il bundle richiama a sua volta tutti i moduli del dashboard, compreso il supporto microSD.

Le immagini standard del progetto vengono scaricate automaticamente durante la compilazione.

Non è quindi necessario copiare manualmente `dashboard-package.yaml`, i moduli delle singole pagine, immagini, header C++ o componenti microSD.

## Configurare le proprie entità Home Assistant

Tutta la personalizzazione dell'impianto viene effettuata nel file:

```text
.user_config.yaml
```

**Non modificare i nomi a sinistra dei due punti.** Modificare solamente l'entity ID tra virgolette sulla destra.

Esempio:

```yaml
energy_solar_power: "sensor.inverter_potenza_in_ingresso"
```

Se nel proprio Home Assistant il sensore FV si chiama:

```text
sensor.fotovoltaico_potenza
```

modificare solamente così:

```yaml
energy_solar_power: "sensor.fotovoltaico_potenza"
```

Lo stesso principio vale per tutte le altre entità.

### Energia / Home

Le principali associazioni sono:

```yaml
energy_solar_power: "sensor..."       # potenza FV istantanea, W
energy_solar_daily: "sensor..."       # produzione FV giornaliera, kWh
energy_grid_power: "sensor..."        # potenza rete istantanea, W
energy_battery_power: "sensor..."     # carica/scarica batteria, W
energy_battery_soc: "sensor..."       # stato batteria, %
heatpump_power: "sensor..."           # consumo istantaneo PDC, W
```

La Wallbox utilizzata nel flusso energetico usa il sensore di potenza istantanea:

```text
sensor.consumo_istantaneo_wallbox
```

Se non si possiede già un sensore complessivo trifase, è possibile crearne uno in Home Assistant sommando le tre fasi, ad esempio:

```yaml
template:
  - sensor:
      - name: "Consumo istantaneo WALLBOX"
        unique_id: consumo_istantaneo_wallbox
        unit_of_measurement: "W"
        device_class: power
        state: >-
          {{ (states('sensor.FASE_A_POTENZA') | float(0) +
              states('sensor.FASE_B_POTENZA') | float(0) +
              states('sensor.FASE_C_POTENZA') | float(0)) }}
```

Sostituire i tre `sensor.FASE_*_POTENZA` con i propri sensori.

### Luci e cancello

Nel blocco `LIGHTS / GATE` sostituire le entità `switch.*` con quelle del proprio impianto.

Esempio:

```yaml
light_kitchen: "switch.luce_cucina"
light_garage: "switch.luce_garage"
gate_control: "switch.cancello"
```

I sensori di potenza delle luci esterne sono configurabili nello stesso file.

### Riscaldamento

Sono previste quattro zone:

- soggiorno;
- camera matrimoniale;
- camera;
- studio.

Per ogni zona sono disponibili associazioni per `climate`, temperatura, umidità, finestra, stato riscaldamento, connettività e modalità.

Esempio:

```yaml
heating_living_climate: "climate.salotto"
heating_living_temperature: "sensor.salotto_temperatura"
heating_living_humidity: "sensor.salotto_umidita"
```

### Meteo

Nel blocco `WEATHER` impostare le proprie entità per:

- entità `weather` principale;
- temperatura esterna;
- umidità esterna;
- alba e tramonto;
- pioggia;
- qualità aria;
- umidità terreno, se disponibile.

### Ricarica auto / Wallbox

Nel blocco `EV CHARGING` sostituire le entità relative a Wallbox e veicolo.

Sono previste, tra le altre:

```yaml
ev_wallbox_max_current: "number..."
ev_wallbox_connector_status: "sensor..."
ev_wallbox_session_energy: "sensor..."
ev_wallbox_session_time: "sensor..."
ev_wallbox_total_energy: "sensor..."
ev_vehicle_battery: "sensor..."
```

Le funzioni che comandano Home Assistant richiedono che ESPHome sia autorizzato a eseguire azioni.

In Home Assistant aprire:

**Impostazioni → Dispositivi e servizi → ESPHome → waveshare-display → Configura**

e abilitare l'opzione che consente al dispositivo di eseguire azioni Home Assistant.

### Robot tagliaerba

Nel blocco `ROBOT LAWNMOWER` associare i propri sensori.

I topic MQTT sono configurabili:

```yaml
robot_mqtt_control_topic: "home/robot/mower/control"
robot_mqtt_timers_topic: "home/robot/mower/control/timers"
robot_mqtt_joystick_topic: "home/robot/mower/control/joystick"
```

Chi utilizza un robot differente deve adattare sia le entità sia i topic/comandi alla propria implementazione.

## Secrets ESPHome

Nel normale file `secrets.yaml` di ESPHome devono essere presenti almeno:

```yaml
wifi_ssid: "NOME_WIFI"
wifi_password: "PASSWORD_WIFI"
waveshare_display_encryption_key: "CHIAVE_API_ESPHOME"
waveshare_display_ap_password: "PASSWORD_AP_FALLBACK"
```

Il repository contiene `esphome/secrets.example.yaml` come riferimento.

**Non pubblicare mai il proprio `secrets.yaml`.**

## Pacchetti Home Assistant

Nella cartella:

```text
home-assistant/
```

sono presenti i package/helper utilizzati dal progetto, tra cui:

```text
display_weather.yaml
home_assistant_display.yaml
dashboard_update.yaml
```

Per la configurazione leggere anche:

```text
home-assistant/CONFIGURATION.md
```

Alcune pagine utilizzano sensori helper `sensor.display_*`: i sensori sorgente devono essere adattati al proprio impianto seguendo il file di configurazione.

Dopo aver aggiunto per la prima volta package Home Assistant che creano nuove entità, eseguire un **riavvio completo di Home Assistant**.

## Convalida e installazione

Dopo aver configurato `.user_config.yaml` e `secrets.yaml`:

1. aprire ESPHome Device Builder;
2. selezionare `waveshare-display`;
3. eseguire **Convalida**;
4. correggere eventuali entity ID mancanti o errati;
5. eseguire **Installa**;
6. per gli aggiornamenti successivi è possibile utilizzare **Wireless / OTA**.

Al primo flash via USB seguire la normale procedura ESPHome per ESP32-S3.

## Aggiornamenti del dashboard

La configurazione è modulare e viene caricata dal repository GitHub. Il file locale contiene principalmente Wi-Fi, API ESPHome e associazioni personali.

Prima di installare una nuova versione è consigliato eseguire sempre **Convalida**.

Il package opzionale `home-assistant/dashboard_update.yaml` permette a Home Assistant di controllare la disponibilità di nuove versioni. Non installa automaticamente il firmware.

## microSD

Il supporto microSD è integrato come external component e viene caricato automaticamente dal bundle.

La scheda può essere utilizzata per il logging energetico e per conservare le baseline giornaliere utilizzate dalla pagina Energia.

Il dashboard continua a funzionare anche senza microSD; in quel caso le funzioni che dipendono dalla scheda non sono disponibili.

## Immagini personalizzate

Nel file `.user_config.yaml` sono presenti anche gli URL delle immagini standard:

```yaml
robot_image_file: "..."
ev_image_file: "..."
pdc_image_file: "..."
```

Per utilizzare immagini personali è sufficiente sostituire l'URL mantenendo invariato il nome della substitution.

## Struttura del repository

```text
esphome/
  dashboard-bundle.yaml
  dashboard-package.yaml
  waveshare-display.example.yaml
  user_config.example.yaml
  energy-page-v1.2.2.yaml
  wallbox-flow-v1.2.yaml
  pdc-page-v1.2.yaml
  heating-slider-v1.2.yaml
  update-client-v1.2.yaml

components/
  waveshare_sd/

home-assistant/
  display_weather.yaml
  home_assistant_display.yaml
  dashboard_update.yaml
  CONFIGURATION.md

docs/
  images/
```

## Cover 3D

![Supporto a parete](docs/images/wall-support-vimar.jpg)

![Interno cover](docs/images/enclosure-inside-1.jpg)

![Pulsanti RESET e BOOT](docs/images/reset-boot-buttons.jpg)

![Scheda montata](docs/images/board-mounted.jpg)

## Progetto correlato

Robot Arduino 4.0:

https://github.com/Marcobedendo78/Robot-Arduino-4.0

## Autore

Marco Bedendo — `Marcobedendo78`
