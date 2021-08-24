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

#include "tcp_client.h"

/** TAG Used for debug msg doing runtime */
static const char *TAG_SERVER = "SERVER";

// End of transmission string.
static const char *EndOfTrans = "\x02\x03\x04\x7F";

/**
 * @brief Init the server connection. Creates socket and save it in the System construct. 
 * 
 * @param sys 
 * @return int8_t 
 */
int8_t server_init(int *sock)
{
    int addr_family = 0;
    int ip_protocol = 0;
    ip_protocol = IPPROTO_IP;
    addr_family = AF_INET;
    int socks = 0;

    socks = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (socks < 0)
    {
        ESP_LOGE(TAG_SERVER, "Unable to create socket: errno %d", errno);
        return -1;
    }
    *sock = socks;
    return 0;
}

/**
 * @brief Connects to the server. 
 * 
 * @param sys System config. 
 * @return int8_t 
 */
int8_t server_connect(int *socket)
{

    char host_ip[] = HOST_IP_ADDR;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    ESP_LOGI(TAG_SERVER, "Connecting to %s:%d", host_ip, PORT);

    int err = connect(*socket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
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
int8_t server_disconnect(int *socket)
{

    ESP_LOGI(TAG_SERVER, "Closing the socket");
    close(*socket);
    *socket = 0;
    return 0;
}

/**
 * @brief Send picture to TCP server
 * 
 * @param buf Pointer to data buffer
 * @param len The length of the data buffer
 * @return int8_t 
 */
int8_t server_send_picture(void *buf, size_t len, int *socket)
{
    int err = 0;
    ESP_LOGD(TAG_SERVER, "Sending picture");
    send(*socket, buf, len, 0);
    if (err < 0)
    {
        ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
        return -1;
    }
    /** Send End of transmissions string */
    err = send(*socket, EndOfTrans, sizeof(EndOfTrans), 0);
    if (err < 0)
    {
        ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
        return -1;
    }
    return 0;
}
