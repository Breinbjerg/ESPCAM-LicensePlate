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
static esp_err_t camera_send_picture();

/** Power and init the camera */
esp_err_t camera_init()
{

    /** TODO: Power to camera  GPIO */

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

    camera_send_picture();

    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}

/**
 * @brief Sends to taken picture to the server. 
 * 
 * @param fb Struct pointer with relevant info and pointer to data. 
 * @return esp_err_t 
 */
esp_err_t camera_send_picture(camera_fb_t *fb)
{
    return ESP_OK;
}
