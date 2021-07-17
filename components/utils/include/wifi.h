/**
 * @file wifi.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __WIFI_H__
#define __WIFI_H__

#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "sdkconfig.h"


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define WIFI_SSID  CONFIG_WIFI_SSID
#define WIFI_PASS  CONFIG_WIFI_PASSWORD
#define MAX_RETRY  CONFIG_MAXIMUM_RETRYS


/** Used for Log messages */
static const char *TAG_Wifi = "Wifi_STA";


/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

void wifi_init_sta(void);


#endif /** WIFI_H */