#include "waveshare_sd.h"

namespace esphome {
namespace waveshare_sd {

WaveshareSD *global_waveshare_sd = nullptr;

void WaveshareSD::setup() {
  global_waveshare_sd = this;

  if (this->mount()) {
    ESP_LOGI(TAG, "microSD montata correttamente");
    this->append_log("BOOT", "Waveshare display avviato");
  } else {
    ESP_LOGW(TAG, "microSD non disponibile: il display continua normalmente");
  }
}

void WaveshareSD::dump_config() {
  ESP_LOGCONFIG(TAG, "Waveshare microSD:");
  ESP_LOGCONFIG(TAG, "  Stato: %s", this->mounted_ ? "montata" : "non disponibile");
  if (this->mounted_ && this->card_ != nullptr)
    ESP_LOGCONFIG(TAG, "  Scheda: %s", this->card_->cid.name);
}

bool WaveshareSD::mount() {
  if (this->mounted_)
    return true;

  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI2_HOST;
  host.max_freq_khz = 10000;

  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.host_id = SPI2_HOST;
  slot_config.gpio_cs = SDSPI_SLOT_NO_CS;
  slot_config.gpio_cd = SDSPI_SLOT_NO_CD;
  slot_config.gpio_wp = SDSPI_SLOT_NO_WP;

  esp_vfs_fat_mount_config_t mount_config = {};
  mount_config.format_if_mount_failed = false;
  mount_config.max_files = 8;
  mount_config.allocation_unit_size = 16 * 1024;

  esp_err_t ret = esp_vfs_fat_sdspi_mount(
      MOUNT_POINT, &host, &slot_config, &mount_config, &this->card_);

  if (ret != ESP_OK) {
    ESP_LOGW(TAG, "Mount microSD fallito: %s", esp_err_to_name(ret));
    this->mounted_ = false;
    this->card_ = nullptr;
    return false;
  }

  this->mounted_ = true;
  mkdir("/sdcard/waveshare", 0775);
  ESP_LOGI(TAG, "microSD montata: %s", this->card_->cid.name);
  return true;
}

void WaveshareSD::append_log(const char *level, const char *message) {
  if (!this->mounted_)
    return;

  FILE *f = fopen("/sdcard/waveshare/display.log", "a");
  if (f == nullptr)
    return;

  fprintf(f, "%s;%s\n", level ? level : "INFO", message ? message : "");
  fflush(f);
  fclose(f);
}

void WaveshareSD::append_energy(int64_t timestamp, float solar_w, float grid_w,
                                float battery_w, float home_w, float pdc_w,
                                float battery_soc) {
  if (!this->mounted_)
    return;

  const char *path = "/sdcard/waveshare/energy_history.csv";
  bool create_header = false;
  struct stat st;
  if (stat(path, &st) != 0 || st.st_size == 0)
    create_header = true;

  FILE *f = fopen(path, "a");
  if (f == nullptr)
    return;

  if (create_header)
    fprintf(f, "timestamp,solar_w,grid_w,battery_w,home_w,pdc_w,battery_soc_pct\n");

  fprintf(f, "%lld,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n",
          (long long) timestamp, solar_w, grid_w, battery_w, home_w, pdc_w,
          battery_soc);
  fflush(f);
  fclose(f);
}

bool WaveshareSD::save_energy_baseline(int day_key, float grid_import,
                                       float grid_export, float wallbox,
                                       float pdc, float gate_lights,
                                       float path_lights) {
  if (!this->mounted_)
    return false;

  const char *path = "/sdcard/waveshare/energy_baseline.csv";
  FILE *f = fopen(path, "w");
  if (f == nullptr) {
    ESP_LOGW(TAG, "Impossibile salvare baseline energia su microSD");
    return false;
  }

  fprintf(f, "day_key,grid_import,grid_export,wallbox,pdc,gate_lights,path_lights\n");
  fprintf(f, "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", day_key,
          grid_import, grid_export, wallbox, pdc, gate_lights, path_lights);
  fflush(f);
  fclose(f);
  ESP_LOGI(TAG, "Baseline energia salvata su microSD per %d", day_key);
  return true;
}

bool WaveshareSD::load_energy_baseline(int expected_day_key,
                                       float *grid_import, float *grid_export,
                                       float *wallbox, float *pdc,
                                       float *gate_lights,
                                       float *path_lights) {
  if (!this->mounted_ || grid_import == nullptr || grid_export == nullptr ||
      wallbox == nullptr || pdc == nullptr || gate_lights == nullptr ||
      path_lights == nullptr)
    return false;

  const char *path = "/sdcard/waveshare/energy_baseline.csv";
  FILE *f = fopen(path, "r");
  if (f == nullptr)
    return false;

  char header[160];
  if (fgets(header, sizeof(header), f) == nullptr) {
    fclose(f);
    return false;
  }

  int day_key = -1;
  float gi = 0.0f, ge = 0.0f, wb = 0.0f, hp = 0.0f, gl = 0.0f, pl = 0.0f;
  const int count = fscanf(f, "%d,%f,%f,%f,%f,%f,%f", &day_key, &gi, &ge,
                           &wb, &hp, &gl, &pl);
  fclose(f);

  if (count != 7 || day_key != expected_day_key)
    return false;

  *grid_import = gi;
  *grid_export = ge;
  *wallbox = wb;
  *pdc = hp;
  *gate_lights = gl;
  *path_lights = pl;
  ESP_LOGI(TAG, "Baseline energia ripristinata da microSD per %d", day_key);
  return true;
}

}  // namespace waveshare_sd
}  // namespace esphome
