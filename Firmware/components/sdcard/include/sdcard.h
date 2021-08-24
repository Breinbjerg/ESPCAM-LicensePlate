/**
 * @file sdcard.h
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"


#define MOUNTPOINT "/sdcard"

sdmmc_card_t *card;

/** Init SD card usage */
esp_err_t sdcard_init_mount_as_filesystem();


/** Saves buffer as file onto the sdcard */
esp_err_t sdcard_save_buffer_as_file(void *buffer, size_t size, size_t len, const char *filename);
