/**
 * @file Camera.h
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-07-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "driver/gpio.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "string.h"
#include "system_defines.h"

#ifdef CONFIG_SD_CARD_CONFIG
#include "sdcard.h"
#endif

#ifdef CONFIG_WIFI_CONFIG
#include "wifi.h"
#include "tcp_client.h"
#endif

#ifdef CONFIG_BOARD_WROVER_KIT

#define CAM_PIN_PWDN -1  //power down is not used
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 21
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 19
#define CAM_PIN_D2 18
#define CAM_PIN_D1 5
#define CAM_PIN_D0 4
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

#endif

//AItinker pinmap. (https://randomnerdtutorials.com/esp32-cam-ai-thinker-pinout/)
#ifdef CONFIG_BOARD_ESP32CAM_AITHINKER

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

#endif

#ifdef CONFIG_BOARD_M5STACK
    
#define CAM_PIN_PWDN -1  //power down is not used
#define CAM_PIN_RESET 15 //software reset will be performed
#define CAM_PIN_XCLK 27
#define CAM_PIN_SIOD 25
#define CAM_PIN_SIOC 23

#define CAM_PIN_D7 19
#define CAM_PIN_D6 36
#define CAM_PIN_D5 18
#define CAM_PIN_D4 39
#define CAM_PIN_D3 5
#define CAM_PIN_D2 34
#define CAM_PIN_D1 35
#define CAM_PIN_D0 32
#define CAM_PIN_VSYNC 22
#define CAM_PIN_HREF 26
#define CAM_PIN_PCLK 21
#endif

/** Power and init the camera */
esp_err_t camera_init();

/** Capture Frame- This function will send the picture */
esp_err_t camera_capture();