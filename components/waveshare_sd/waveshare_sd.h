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

 protected:
  bool mounted_{false};
  sdmmc_card_t *card_{nullptr};
};

}  // namespace waveshare_sd
}  // namespace esphome
