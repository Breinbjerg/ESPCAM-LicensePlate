/**
 * @file server.c
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "server.h"

/** TAG Used for debug msg doing runtime */
static const char *TAG_SERVER = "SERVER";

/**
 * @brief Init the server connection. Creates socket and save it in the System construct. 
 * 
 * @param sys 
 * @return int8_t 
 */
int8_t server_init(system_defs **sys)
{
    system_defs *sys_def;
    int addr_family = 0;
    int ip_protocol = 0;
    ip_protocol = IPPROTO_IP;
    addr_family = AF_INET;

    int sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sock < 0)
    {
        ESP_LOGE(TAG_SERVER, "Unable to create socket: errno %d", errno);
        return -1;
    }

    sys_def = (system_defs *)malloc(sizeof(*sys_def));
    if (!sys_def)
    {
        ESP_LOGE(TAG_SERVER, "Malloc Error: Creating System defs");
        return -1;
    }

    /** Assign the newly created socket to the sys struct */
    sys_def->socket = sock;
    *sys = sys_def;

    return 0;
}

/**
 * @brief Connects to the server. 
 * 
 * @param sys System config. 
 * @return int8_t 
 */
int8_t server_connect(system_defs *sys)
{

    char host_ip[] = HOST_IP_ADDR;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    ESP_LOGI(TAG_SERVER, "Connecting to %s:%d", host_ip, PORT);

    int err = connect(sys->socket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
    if (err != 0)
    {
        ESP_LOGE(TAG_SERVER, "Socket unable to connect: errno %d", errno);
        return -2;
    }

    ESP_LOGI(TAG_SERVER, "Successfully connected");
    return 0;
}

/**
 * @brief Disconnect server
 * 
 * @param sys System config struct. 
 * @return int8_t 
 */
int8_t server_disconnect(system_defs *sys)
{

    ESP_LOGI(TAG_SERVER, "Closing the socket");
    close(sys->socket);
    sys->socket = 0;
    return 0;
}

/**
 * @brief Send picture to TCP server
 * 
 * @param buf Pointer to data buffer
 * @param len The length of the data buffer
 * @return int8_t 
 */
int8_t server_send_picture(uint8_t *buf, size_t len, system_defs *sys)
{
    int err = 0;
    ESP_LOGD(TAG_SERVER, "Sending picture");
    send(sys->socket, buf, len);
    if (err < 0)
    {
        ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
        return -1;
    }
    /** Send End of transmissions string */ 
    err = send(sys->socket, EndOfTrans, sizeof(EndOfTrans), 0);
    if (err < 0)
    {
        ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
        return -1;
    }
    
}
