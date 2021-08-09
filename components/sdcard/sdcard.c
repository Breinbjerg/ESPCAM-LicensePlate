/**
 * @file sdcard.c
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "sdcard.h"

/** Used for debug messages */
static const char *TAG = "sd_jpg";

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t sdcard_init_mount_as_filesystem()
{
    esp_err_t ret = ESP_FAIL;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 3,
    };

    /** Testing comments */
    ESP_LOGI(TAG, "Mounting SD card...");
    ret = esp_vfs_fat_sdmmc_mount(MOUNTPOINT, &host, &slot_config, &mount_config, &card);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "SD card mount successfully!");
        return ret;
    }
    else
    {
        ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem. Error: %s", esp_err_to_name(ret));
        return ret;
    }
}

/**
 * @brief 
 * 
 * @param buffer Pointer to the buffer to be saved on the sd-card
 * @param size This is the size in bytes of each element to be written.
 * @param len This is the number of elements, each one with a size of size bytes.
 * @param filename  Name of the file to be saved on the sdcard
 * @return esp_err_t 
 */
esp_err_t sdcard_save_buffer_as_file(void *buffer, size_t size, size_t len, char* filename)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
      fwrite(buffer, size, len, file);
      ESP_LOGI(TAG, "File saved: %s", filename);
    }
    else
    {
      ESP_LOGE(TAG, "Could not open file =(");
    }
    fclose(file);
    
    return ESP_OK;
}