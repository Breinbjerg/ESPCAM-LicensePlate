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

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000, //EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
#ifdef CONFIG_PIXEL_FORMAT_JPEG
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_SVGA, //YUV422,GRAYSCALE,RGB565,JPEG
#endif

#ifdef CONFIG_PIXEL_FORMAT_GRAYSCALE
    .pixel_format = PIXFORMAT_GRAYSCALE, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,        //QQVGA-QXGA Do not use sizes above QVGA when not JPEG
#endif
    .jpeg_quality = 12,                 //0-63 lower number means higher quality
    .fb_count = 1,                      //if more than one, i2s runs in continuous mode. Use only with JPEG
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY //CAMERA_GRAB_LATEST. Sets when buffers should be filled
};

#ifdef CONFIG_SD_CARD_CONFIG
/** Used when storing on the SD-card */
static uint8_t picture_count = 0;
#else
/** Used when talking to TCP server */
static int sock = 0;
#endif

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
    esp_err_t ret;
    /** TODO: Power to camera  GPIO */
    /** If SD selected init */
#ifdef CONFIG_SD_CARD_CONFIG
    ret = sdcard_init_mount_as_filesystem();
    if (ret != ESP_OK)
    {
        return ESP_FAIL;
    }
#endif

    /** If Wifi seleceted init */
#ifdef CONFIG_WIFI_CONFIG
    wifi_init_sta();
    /** If TCP-server selected init */
#ifdef CONFIG_CONNECT_TCP_SERVER
    ret = server_init(&sock);
    if (ret != ESP_OK)
    {
        return ESP_FAIL;
    }
    ret = server_connect(&sock);
    if (ret != ESP_OK)
    {
        return ESP_FAIL;
    }
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
    ESP_LOGI(TAG_Camera, "Saving picture");
    camera_save_picture(fb);
    esp_camera_fb_return(fb);

    return ESP_OK;
}

/**
 * @brief Sends the taken picture to the selected destination (menuconfig). 
 *        Currently this function can either save to SD-Card or via Wifi to TCP-server.
 *        This is set in the menuconfig. 
 * 
 * @param fb Struct pointer with relevant info and pointer to data. 
 * @return esp_err_t 
 */
static esp_err_t camera_save_picture(camera_fb_t *fb)
{

#ifdef CONFIG_SD_CARD_CONFIG
    char *filename = malloc(32);
    sprintf(filename, "/sdcard/pic_%d", picture_count);
    filename[strlen(filename)] = '\0';
    ESP_LOGI(TAG_Camera, "Filename: %s with length of %d", filename, strlen(filename));
#ifdef CONFIG_PIXEL_FORMAT_GRAYSCALE
    uint8_t *buf = NULL;
    size_t buf_len = 0;
    bool converted = frame2bmp(fb, &buf, &buf_len);
    if (!converted)
    {
        ESP_LOGE(TAG_Camera, "BMP conversion failed");
        return ESP_FAIL;
    }
    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    sdcard_save_buffer_as_file(buf, 1, buf_len, filename);
    free(buf);
    buf_len = 0;
#endif
    sdcard_save_buffer_as_file(fb->buf, 1, fb->len, filename);
    picture_count++;
    free(filename);
#endif

#ifdef CONFIG_CONNECT_TCP_SERVER
#ifdef CONFIG_PIXEL_FORMAT_GRAYSCALE
    uint8_t *buf = NULL;
    size_t buf_len = 0;
    bool converted = frame2bmp(fb, &buf, &buf_len);
    if (!converted)
    {
        ESP_LOGE(TAG_Camera, "BMP conversion failed");
        return ESP_FAIL;
    }
    /** Free space */
    server_send_picture(buf, buf_len, &sock);
    free(buf);
    buf_len = 0;
#endif
    // If JPEG picture. No conversion is needed.
    server_send_picture(fb->buf, fb->len, &sock);
#endif
    return ESP_OK;
}
