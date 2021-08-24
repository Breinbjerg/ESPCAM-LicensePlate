/**
 * @file server.h
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "stdint.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#include "system_defines.h"

/** Port and IP are defined in menuconfig */
#define HOST_IP_ADDR CONFIG_IPV4_ADDR
#define PORT CONFIG_PORT

int8_t server_init(int *socket);

int8_t server_connect(int *socket);

int8_t server_disconnect(int *socket);

int8_t server_send_picture(void *buf, size_t len, int *socket);
