#pragma once

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/sdspi_host.h"

namespace esphome {
namespace waveshare_sd {

static const char *const TAG = "waveshare_sd";
static const char *const MOUNT_POINT = "/sdcard";

class WaveshareSD : public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::LATE; }

  bool mount();
  bool is_mounted() const { return mounted_; }
  void append_log(const char *level, const char *message);
  void append_energy(int64_t timestamp, float solar_w, float grid_w,
                     float battery_w, float home_w, float pdc_w,
                     float battery_soc);

  // Persistent daily baselines used by the Energy page. NVS remains the
  // primary fast store, while the microSD provides a second copy that can be
  // restored after firmware changes or loss of ESP32 preferences.
  bool save_energy_baseline(int day_key, float grid_import, float grid_export,
                            float wallbox, float pdc, float gate_lights,
                            float path_lights);
  bool load_energy_baseline(int expected_day_key, float *grid_import,
                            float *grid_export, float *wallbox, float *pdc,
                            float *gate_lights, float *path_lights);

 protected:
  bool mounted_{false};
  sdmmc_card_t *card_{nullptr};
};

// Pointer used by compatibility wrappers while the dashboard package is
// migrated from the old sd_storage.h helper API to this ESPHome component.
extern WaveshareSD *global_waveshare_sd;

}  // namespace waveshare_sd
}  // namespace esphome

// Backward-compatible helper names still referenced by dashboard-package.yaml.
// They now forward to the remote waveshare_sd component, so no local header is
// needed and existing dashboard code keeps compiling during the migration.
inline bool sd_storage_is_mounted() {
  return esphome::waveshare_sd::global_waveshare_sd != nullptr &&
         esphome::waveshare_sd::global_waveshare_sd->is_mounted();
}

inline void sd_storage_append_log(const char *level, const char *message) {
  if (esphome::waveshare_sd::global_waveshare_sd != nullptr)
    esphome::waveshare_sd::global_waveshare_sd->append_log(level, message);
}

inline void sd_storage_append_energy(int64_t timestamp, float solar_w,
                                     float grid_w, float battery_w,
                                     float home_w, float pdc_w,
                                     float battery_soc) {
  if (esphome::waveshare_sd::global_waveshare_sd != nullptr) {
    esphome::waveshare_sd::global_waveshare_sd->append_energy(
        timestamp, solar_w, grid_w, battery_w, home_w, pdc_w, battery_soc);
  }
}
