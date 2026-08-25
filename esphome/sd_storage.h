#pragma once

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/sdspi_host.h"

static const char *SD_STORAGE_TAG = "sd_storage";
static const char *SD_STORAGE_MOUNT = "/sdcard";

static bool sd_storage_mounted_ = false;
static sdmmc_card_t *sd_storage_card_ = nullptr;

inline bool sd_storage_mount() {
  if (sd_storage_mounted_)
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
      SD_STORAGE_MOUNT,
      &host,
      &slot_config,
      &mount_config,
      &sd_storage_card_);

  if (ret != ESP_OK) {
    ESP_LOGW(SD_STORAGE_TAG, "Mount microSD fallito: %s", esp_err_to_name(ret));
    sd_storage_mounted_ = false;
    sd_storage_card_ = nullptr;
    return false;
  }

  sd_storage_mounted_ = true;
  mkdir("/sdcard/waveshare", 0775);

  ESP_LOGI(
      SD_STORAGE_TAG,
      "microSD montata: %s",
      sd_storage_card_->cid.name);

  return true;
}

inline bool sd_storage_is_mounted() {
  return sd_storage_mounted_;
}

inline void sd_storage_append_log(const char *level, const char *message) {
  if (!sd_storage_mounted_)
    return;

  FILE *f = fopen("/sdcard/waveshare/display.log", "a");
  if (f == nullptr)
    return;

  fprintf(f, "%s;%s\n", level ? level : "INFO", message ? message : "");
  fflush(f);
  fclose(f);
}

inline void sd_storage_append_energy(
    int64_t timestamp,
    float solar_w,
    float grid_w,
    float battery_w,
    float home_w,
    float pdc_w,
    float battery_soc) {

  if (!sd_storage_mounted_)
    return;

  const char *path = "/sdcard/waveshare/energy_history.csv";

  bool create_header = false;
  struct stat st;
  if (stat(path, &st) != 0 || st.st_size == 0)
    create_header = true;

  FILE *f = fopen(path, "a");
  if (f == nullptr)
    return;

  if (create_header) {
    fprintf(
        f,
        "timestamp,solar_w,grid_w,battery_w,home_w,pdc_w,battery_soc_pct\n");
  }

  fprintf(
      f,
      "%lld,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n",
      (long long) timestamp,
      solar_w,
      grid_w,
      battery_w,
      home_w,
      pdc_w,
      battery_soc);

  fflush(f);
  fclose(f);
}
