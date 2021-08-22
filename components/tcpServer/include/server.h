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
#include "system_defs.h"

/** Port and IP are defined in menuconfig */
#define HOST_IP_ADDR CONFIG_IPV4_ADDR
#define PORT CONFIG_PORT

// End of transmission string. Must be appended to data send from this device.
static const char *EndOfTrans = "\x02\x03\x04\x7F";

int8_t server_init(system_defs **sys);

int8_t server_connect(system_defs *sys);

int8_t server_disconnect(system_defs *sys);

int8_t server_send_picture(uint8_t *buf, size_t len, system_defs *sys);
