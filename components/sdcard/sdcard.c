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
    const char mount_point[] = MOUNTPOINT;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif
        .max_files = 10,
    };

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, change this to 1:
    slot_config.width = 4;

    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG, "Mounting SD card...");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "SD card mount successfully!");
        sdmmc_card_print_info(stdout, card);

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
esp_err_t sdcard_save_buffer_as_file(void *buffer, size_t size, size_t len, const char *filename)
{
    ESP_LOGD(TAG, "Trying to open the file: %s", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fwrite(buffer, size, len, file);
    ESP_LOGI(TAG, "File saved: %s", filename);

    fclose(file);

    return ESP_OK;
}