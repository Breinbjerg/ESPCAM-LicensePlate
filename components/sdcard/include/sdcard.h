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



/** Init SD card usage */ 
esp_err_t init_sdcard();
