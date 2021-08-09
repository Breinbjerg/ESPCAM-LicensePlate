/**
 * @file Camera.c
 * @author Steffen Breinbjerg (steffenpb@outlook.dk)
 * @brief 
 * @version 0.1
 * @date 2021-07-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "camera.h"

/** Used for Log messages */
static const char *TAG_Camera = "Camera";

/** Send picture to server - Remember to connect to server first */
static esp_err_t camera_save_picture();

/**
 * @brief Init the camera unit on the board. 
 *        Depending on the settings in menuconfig i will load the right GPIO
 *        Futhermore it will init the SDCard or Wifi and if selected the TCP-server init aswell. 
 *        Afterwards you can call camera_capture() and taken picture will be saved to seleceted save location. 
 * 
 * @return esp_err_t 
 */
esp_err_t camera_init()
{

    /** TODO: Power to camera  GPIO */

    /** If SD selected init */
#ifdef CONFIG_SD_CARD_CONFIG
    sdcard_init();
#endif

    /** If Wifi seleceted init */
#ifdef CONFIG_WIFI_CONFIG
    wifi_init_sta();

    /** If TCP-server selected init */
#ifdef CONFIG_CONNECT_TCP_SERVER
    server_init();
#endif

#endif

    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_Camera, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

/** Take picture TODO: Return pointer to picture to send to server */
esp_err_t camera_capture()
{
    //acquire a frame
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        ESP_LOGE(TAG_Camera, "Camera Capture Failed");
        return ESP_FAIL;
    }
    /** Save the taken picture */ 
    camera_save_picture(fb);

    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}

/**
 * @brief Sends to taken picture to the server. 
 *        Currently this function can either save to SD-Card or via Wifi to TCP-server.
 *        This is set in the menuconfig. 
 * 
 * @param fb Struct pointer with relevant info and pointer to data. 
 * @return esp_err_t 
 */
static esp_err_t camera_save_picture(camera_fb_t *fb)
{

    char *pic_name = malloc(MOUNTPOINT+CONFIG_FILENAME_PICTURE + sizeof(uint16_t));
    sprintf(pic_name, "/sdcard/pic_%lli.jpg", timestamp);
#ifdef CONFIG_SD_CARD_CONFIG
    sdcard_save_buffer_as_file(fb->buf, 1, fb->len, filename);
#endif

#ifdef CONFIG_CONNECT_TCP_SERVER
    /** TODO: Implement server function to send picture. */
#endif

    return ESP_OK;
}
