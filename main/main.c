/**
 * @file main.c
 * @author Steffen Breinbjerg
 * @brief Main function.
 * @version 0.1
 * @date 2021-08-01
 * 
 * 
 */

#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "camera.h"


/** Pointer to system struct */
system_defs *sys_defs;

void app_main(void)
{

   //Initialize NVS - Only mandatory when using wifi
#ifdef CONFIG_WIFI_CONFIG
   esp_err_t ret = nvs_flash_init();
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
   }
   ESP_ERROR_CHECK(ret);

#endif
   /** Init the camera 
    * This function will init either Wifi or SDCard depending on settings in menuconfig.
    * No Further actions is needed. If succssfull you can take pictures. */
   ret = camera_init();
   if (ret != ESP_OK)
   {

      ESP_LOGE("MAIN", "Error: Camera init function");
      return -1;
   }

   while (1)
   {
      /** Take a picture -This will automatically store the picture depending on settings in menuconfig. */
      camera_capture();
      vTaskDelay((CONFIG_PICTURE_INTERVAL_TIME * 1000) / portTICK_RATE_MS);
   }
}
